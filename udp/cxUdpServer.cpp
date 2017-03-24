//
//  cxUdpServer.cpp
//  udp
//
//  Created by xuhua on 16/03/2017.
//  Copyright © 2017 xuhua. All rights reserved.
//


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

cxUdpServer::cxUdpServer()
{
    rQueue = cxList::Alloc();
}

cxUdpServer::~cxUdpServer()
{
    rQueue->Release();
}

void cxUdpServer::OnRecvData(cxUdpHost *h,const cxUdpData *d)
{
    CX_LOGGER("%llu RECV %llu data %s SEQ:%llu %p",UID(), h->UID(),d->Data()->ToString(),d->Seq(),uv_thread_self());
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

