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
    onData.Fire(this, h, d);
    cxUdpBase::OnRecvData(h, d);
}

void cxUdpServer::OnRecvFrame(UdpAddr *addr,cxAny data,cxInt size)
{
    dMutex.Lock();
    UdpFrame *d = UdpFrame::Alloc(addr, data, size);
    rQueue->Append(d);
    d->Release();
    dCond.Signal();
    dMutex.Unlock();
}

void cxUdpServer::WorkRun()
{
    dMutex.Lock();
    while(rQueue->Size() == 0){
        dCond.Wait(dMutex);
    }
    UdpFrame *d = rQueue->Front()->To<UdpFrame>();
    CX_ASSERT(d != nullptr, "d nullptr");
    DecodeData(d->Addr(),d->Buffer());
    rQueue->PopFront();
    dMutex.Unlock();
}

CX_CPP_END

