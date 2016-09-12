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
#include "TCPInterface.h"
#include "GetTime.h"

CX_CPP_BEGIN

CX_IMPLEMENT(TcpServer)

TcpServer::TcpServer()
{
    
}

TcpServer::~TcpServer()
{
    
}

bool TcpServer::HasConnection(RakNet::SystemAddress addr)
{
    bool ret = false;
    for(cxInt i=0; i < remoteClientsLength; i++){
        remoteClients[i].isActiveMutex.Lock();
        ret = remoteClients[i].isActive && remoteClients[i].systemAddress == addr;
        remoteClients[i].isActiveMutex.Unlock();
        if(ret){
            break;
        }
    }
    return ret;
}

cxInt TcpServer::MaxConnection()
{
    return remoteClientsLength;
}

CX_IMPLEMENT(cxServer);

cxServer::cxServer()
{
    threads = nullptr;
    uv_mutex_init(&mutex);
    tcp = TcpServer::Alloc();
}

TcpServer *cxServer::GetTcp()
{
    return tcp;
}

cxServer::~cxServer()
{
    tcp->Release();
    uv_mutex_destroy(&mutex);
    delete []threads;
}

void cxServer::Lock()
{
    uv_mutex_lock(&mutex);
}

void cxServer::UnLock()
{
    uv_mutex_unlock(&mutex);
}

bool cxServer::Init(cxInt nt,cxInt port,cxInt max,cchars pass)
{
    if(!initKey()){
        CX_ERROR("create public private key error");
        return false;
    }
    if(!peer->InitializeSecurity(publicKey, privateKey)){
        CX_ERROR("init key error");
        return false;
    }
    thread = nt;
    exitFlags = false;
    threads = new uv_thread_t[nt];
    socket = RakNet::SocketDescriptor(port,0);
    peer->Startup(max, &socket, 1);
    peer->SetMaximumIncomingConnections(max);
    peer->SetOccasionalPing(false);
    peer->SetIncomingPassword(pass, (int)strlen(pass));
    CX_LOGGER("server start port=%d max=%d thread=%d",port,max,thread);
    return tcp->Start(port, max/8);
}

void cxServer::OnNewConnect(RakNet::RakNetGUID clientId)
{
    CX_LOGGER("OnNewConnect %s",clientId.ToString());
}

void cxServer::OnLost(RakNet::RakNetGUID clientId)
{
    CX_LOGGER("OnLost %s",clientId.ToString());
}

void cxServer::OnUDPPackage(RakNet::Packet *packet,cchars data,cxInt size)
{
    
}

void cxServer::OnPacket(RakNet::Packet *packet)
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
        default:{
            cxRaknet::OnPacket(packet);
            break;
        }
    }
}

void cxServer::OnTCPPacket(RakNet::Packet *packet)
{
    CX_LOGGER("on tcp packet length=%d",packet->length);
}

void cxServer::OnTcpNewConnect(RakNet::SystemAddress addr)
{
    CX_LOGGER("tcp new conn %s",addr.ToString());
}

void cxServer::OnTcpLost(RakNet::SystemAddress addr)
{
    CX_LOGGER("tcp lost conn %s",addr.ToString());
}

void cxServer::OnTcpConnected(RakNet::SystemAddress addr)
{
    CX_LOGGER("client connected to %s",addr.ToString());
}

void cxServer::OnTcpFailed(RakNet::SystemAddress addr)
{
    CX_LOGGER("client connected to %s failed",addr.ToString());
}

RakNet::SystemAddress cxServer::Connect(cchars host,cxInt port)
{
    return tcp->Connect(host, port);
}

void cxServer::CloseTcp(RakNet::SystemAddress addr)
{
    tcp->CloseConnection(addr);
}

void cxServer::TcpWrite(RakNet::SystemAddress addr,const cxStr *data,bool broadcast)
{
    if(broadcast){
        tcp->Send(data->Data(), data->Size(), RakNet::UNASSIGNED_SYSTEM_ADDRESS, broadcast);
    }else{
        tcp->Send(data->Data(), data->Size(), addr, broadcast);
    }
}

void cxServer::TcpWrite(RakNet::SystemAddress addr,cxPackHeader *pack,bool broadcast)
{
    SetHashHeader(pack);
    if(broadcast){
        tcp->Send((cchars)pack, pack->Size, RakNet::UNASSIGNED_SYSTEM_ADDRESS, broadcast);
    }else{
        tcp->Send((cchars)pack, pack->Size, addr, broadcast);
    }
}

cxInt cxServer::TcpCount()
{
    return tcp->GetConnectionCount();
}

cxInt cxServer::TcpMax()
{
    return tcp->MaxConnection();
}

bool cxServer::HasConnection(RakNet::SystemAddress addr)
{
    return tcp->HasConnection(addr);
}

void cxServer::Update()
{
    RakNet::SystemAddress addr;
    //有连接成功的
    while((addr=tcp->HasCompletedConnectionAttempt())!=RakNet::UNASSIGNED_SYSTEM_ADDRESS){
        OnTcpConnected(addr);
    }
    //连接失败的
    while((addr=tcp->HasFailedConnectionAttempt())!=RakNet::UNASSIGNED_SYSTEM_ADDRESS){
        OnTcpFailed(addr);
    }
    //有新的连接
    while((addr=tcp->HasNewIncomingConnection())!=RakNet::UNASSIGNED_SYSTEM_ADDRESS){
        OnTcpNewConnect(addr);
    }
    //连接断开
    while((addr=tcp->HasLostConnection())!=RakNet::UNASSIGNED_SYSTEM_ADDRESS){
        OnTcpLost(addr);
    }
    //
    RakNet::Packet *packet = nullptr;
    for(packet=tcp->Receive(); packet; tcp->DeallocatePacket(packet), packet=tcp->Receive()) {
        OnTCPPacket(packet);
    }
    cxRaknet::Update();
}

void cxServer::ThreadLoop()
{
    
}

bool cxServer::initKey()
{
    cat::EasyHandshake::Initialize();
    cat::EasyHandshake handshake;
    return handshake.GenerateServerKey(publicKey, privateKey);
}

void cxServer::runEntry(void *a)
{
    cxAutoPool::Start();
    CX_LOGGER("%p process thread start",uv_thread_self());
    cxServer *server = (cxServer *)a;
    server->ThreadBegin();
    while(!server->exitFlags){
        server->ThreadLoop();
        server->Update();
        cxAutoPool::Update();
        RakSleep(1);
    }
    CX_LOGGER("%p process thread stop",uv_thread_self());
    server->ThreadExit();
    cxAutoPool::Stop();
}

void cxServer::Wait()
{
    //等待线程结束
    for(cxInt i=0;i<thread;i++){
        uv_thread_join(&threads[i]);
    }
    //关闭tcp连接
    tcp->Stop();
}

void cxServer::Stop()
{
    //设置结束标识
    exitFlags = true;
}

void cxServer::Run()
{
    for(cxInt i=0;i<thread;i++){
        uv_thread_create(&threads[i], cxServer::runEntry, this);
        RakSleep(200);
    }
}

CX_CPP_END

