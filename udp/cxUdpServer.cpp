//
//  cxUdpServer.cpp
//  udp
//
//  Created by xuhua on 16/03/2017.
//  Copyright © 2017 xuhua. All rights reserved.
//

#include <unistd.h>
#include <core/cxAutoPool.h>
#include "cxUdpData.h"
#include "cxUdpServer.h"

CX_CPP_BEGIN

CX_IMPLEMENT(UdpFrame)

UdpFrame::UdpFrame()
{
    buffer = nullptr;
}

UdpFrame::~UdpFrame()
{
    cxObject::release(&buffer);
}

cxStr *UdpFrame::Buffer()
{
    return buffer;
}

const UdpAddr *UdpFrame::Addr()
{
    return &addr;
}

UdpFrame *UdpFrame::Alloc(UdpAddr *addr, cxAny data,cxInt size)
{
    UdpFrame *ret = UdpFrame::Alloc();
    ret->addr = *addr;
    ret->buffer = cxStr::Alloc(data, size);
    return ret;
}

CX_IMPLEMENT(cxUdpServer);

cxUdpServer::cxUdpServer(cxInt anum) : bar(anum + 1)
{
    num = anum;
    isexit = false;
    pid = (uv_thread_t *)malloc(sizeof(uv_thread_t) * num);
    rQueue = cxList::Alloc();
}

cxUdpServer::~cxUdpServer()
{
    free(pid);
    rQueue->Release();
}

void cxUdpServer::OnRecvData(cxUdpHost *h,const cxUdpData *d)
{
    CX_ERROR("%llu RECV %llu data %s SEQ:%llu %p",UID(), h->UID(),d->Data()->ToString(),d->Seq(),uv_thread_self());
    cxUdpBase::OnRecvData(h, d);
}

void cxUdpServer::OnRecvFrame(UdpAddr *addr,cxAny data,cxInt size)
{
    dMutex.Lock();
    UdpFrame *d = UdpFrame::Alloc(addr, data, size);
    rQueue->Append(d);
    d->Release();
    dCond.Broadcast();
    dMutex.Unlock();
}

void cxUdpServer::workRunFunc(void *arg)
{
    cxUdpServer *srv = (cxUdpServer *)arg;
    srv->bar.Wait();
    while(!srv->isexit){
        cxAutoPool::Start();
        srv->WorkRun();
        cxAutoPool::Update();
        usleep(100);
    }
}

void cxUdpServer::workUpdateFunc(void *arg)
{
    cxUdpServer *srv = (cxUdpServer *)arg;
    srv->bar.Wait();
    while(!srv->isexit){
        cxAutoPool::Start();
        srv->Update();
        cxAutoPool::Update();
        usleep(100);
    }
}

void cxUdpServer::Stop()
{
    isexit = true;
    for(cxInt i=0;i<num;i++){
        uv_thread_join(&pid[i]);
    }
    uv_thread_join(&mpid);
}

void cxUdpServer::Start()
{
    uv_thread_create(&mpid, workUpdateFunc, this);
    for(cxInt i=0;i<num;i++){
        uv_thread_create(&pid[i], workRunFunc, this);
    }
}

void cxUdpServer::WorkRun()
{
    dMutex.Lock();
    while(rQueue->Size() == 0){
        dCond.Wait(dMutex);
    }
    UdpFrame *d = rQueue->Front()->To<UdpFrame>();
    CX_ASSERT(d != nullptr, "queue empty");
    d->Retain();
    rQueue->PopFront();
    dMutex.Unlock();
    
    DecodeData(d->Addr(),d->Buffer());
    d->Release();
}

CX_CPP_END

