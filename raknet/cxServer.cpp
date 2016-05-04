//
//  cxRaknetServer.cpp
//  RaknetServer
//
//  Created by xuhua on 5/4/16.
//  Copyright © 2016 xuhua. All rights reserved.
//


#include <core/cxAutoPool.h>
#include "cxServer.h"

CX_CPP_BEGIN

CX_IMPLEMENT(cxServer);

cxServer::cxServer()
{
    threads = nullptr;
}

cxServer::~cxServer()
{
    delete []threads;
}

void cxServer::Init(cxInt nt,cxInt port,cxInt max,cchars pass)
{
    thread = nt;
    exitFlags = false;
    threads = new uv_thread_t[nt];
    
    socket = RakNet::SocketDescriptor(port,0);
    peer->Startup(max, &socket, 1);
    
    peer->SetMaximumIncomingConnections(max);
    peer->SetIncomingPassword(pass, (int)strlen(pass));
}

void cxServer::OnNewConnect(RakNet::RakNetGUID clientId)
{
    CX_LOGGER("OnNewConnect %s",clientId.ToString());
}

void cxServer::OnLost(RakNet::RakNetGUID clientId)
{
    CX_LOGGER("OnLost %s",clientId.ToString());
}

void cxServer::OnMessage(RakNet::RakNetGUID clientId,const cxStr *message)
{
    CX_LOGGER("onMessage %s,%d",clientId.ToString(),message->Size());
    Broadcast(cxStr::UTF8("board"));
}

void cxServer::OnPacket(RakNet::Packet *packet,void *data)
{
    RakNet::MessageID type = packet->data[0];
    switch (type) {
        case ID_NEW_INCOMING_CONNECTION:{
            OnNewConnect(packet->guid);
            break;
        }
        case ID_CONNECTION_LOST:{
            OnLost(packet->guid);
            break;
        }
        case ID_MESSAGE_PACKET:{
            ReadMessage(packet);
            break;
        }
        default:{
            CX_LOGGER("onMessageType %d not process",type);
            break;
        }
    }
}

void cxServer::runEntry(void *a)
{
    CX_LOGGER("%p process thread start",uv_thread_self());
    cxServer *server = (cxServer *)a;
    cxAutoPool::Start();
    void *data = server->ThreadData();
    while(!server->exitFlags){
        server->Process(data);
        cxAutoPool::Update();
        RakSleep(5);
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

