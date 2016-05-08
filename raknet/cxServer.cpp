//
//  cxRaknetServer.cpp
//  RaknetServer
//
//  Created by xuhua on 5/4/16.
//  Copyright © 2016 xuhua. All rights reserved.
//


#include <core/cxAutoPool.h>
#include <core/cxUtil.h>
#include "cxServer.h"

CX_CPP_BEGIN

CX_IMPLEMENT(cxServer);

cxServer::cxServer()
{
    threads = nullptr;
    uv_mutex_init(&mutex);
    uv_loop_init(&loop);
}

cxServer::~cxServer()
{
    uv_loop_close(&loop);
    uv_mutex_destroy(&mutex);
    delete []threads;
}

uv_loop_t *cxServer::Looper()
{
    return &loop;
}

bool cxServer::Init(cxInt nt,cxInt port,cxInt max,cchars pass)
{
    if(!initKey()){
        CX_LOGGER("create public private key error");
        return false;
    }
    if(!peer->InitializeSecurity(publicKey, privateKey)){
        CX_LOGGER("init key error");
        return false;
    }
    thread = nt;
    exitFlags = false;
    threads = new uv_thread_t[nt];
    socket = RakNet::SocketDescriptor(port,0);
    peer->Startup(max, &socket, 1);
    peer->SetMaximumIncomingConnections(max);
    peer->SetIncomingPassword(pass, (int)strlen(pass));
    return true;
}

void cxServer::OnNewConnect(RakNet::RakNetGUID clientId,void *data)
{
    CX_LOGGER("OnNewConnect %s",clientId.ToString());
}

void cxServer::OnLost(RakNet::RakNetGUID clientId,void *data)
{
    CX_LOGGER("OnLost %s",clientId.ToString());
}

void cxServer::OnMessage(RakNet::RakNetGUID clientId,const cxStr *message,void *data)
{
    CX_LOGGER("onMessage %s,%d",clientId.ToString(),message->Size());
}

void cxServer::OnPacket(RakNet::Packet *packet,void *data)
{
    RakNet::MessageID type = packet->data[0];
    switch (type) {
        case ID_NEW_INCOMING_CONNECTION:{
            OnNewConnect(packet->guid,data);
            break;
        }
        case ID_CONNECTION_LOST:{
            OnLost(packet->guid,data);
            break;
        }
        default:{
            cxRaknet::OnPacket(packet, data);
            break;
        }
    }
}

void cxServer::Loop(void *data)
{
    uv_mutex_lock(&mutex);
    uv_run(&loop, UV_RUN_NOWAIT);
    uv_mutex_unlock(&mutex);
}

bool cxServer::initKey()
{
    cat::EasyHandshake::Initialize();
    cat::EasyHandshake handshake;
    
    if(handshake.GenerateServerKey(publicKey, privateKey)){
        cxUtil::Instance()->WriteDocument("key", cxStr::Create()->Init(publicKey, cat::EasyHandshake::PUBLIC_KEY_BYTES), true);
        cxUtil::Instance()->WriteDocument("key.pub", cxStr::Create()->Init(privateKey, cat::EasyHandshake::PRIVATE_KEY_BYTES), true);
        return true;
    }
    return false;
}

void cxServer::runEntry(void *a)
{
    cxAutoPool::Start();
    CX_LOGGER("%p process thread start",uv_thread_self());
    cxServer *server = (cxServer *)a;
    void *data = server->ThreadData();
    while(!server->exitFlags){
        server->Loop(data);
        server->Process(data);
        cxAutoPool::Update();
        RakSleep(1);
    }
    CX_LOGGER("%p process thread stop",uv_thread_self());
    cxAutoPool::Stop();
}

void cxServer::Stop()
{
    exitFlags = true;
}

void cxServer::Run()
{
    for(cxInt i=0;i<thread;i++){
        uv_thread_create(&threads[i], cxServer::runEntry, this);
    }
    for(cxInt i=0;i<thread;i++){
        uv_thread_join(&threads[i]);
    }
}

CX_CPP_END

