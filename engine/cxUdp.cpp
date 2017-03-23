//
//  cxUdp.cpp
//  cxEngineCore
//
//  Created by xuhua on 1/12/16.
//  Copyright © 2016 xuhua. All rights reserved.
//


#include "cxUdp.h"

CX_CPP_BEGIN

CX_IMPLEMENT(cxUdp);

cxUdp::cxUdp()
{
    c = cxUdpClient::Alloc();
    c->onData = [this](cxUdpBase *v,cxUdpHost *h,const cxUdpData *d){
        OnData(h,d);
    };
    c->onMiss = [this](cxUdpBase *v,cxUdpHost *h,const cxUdpData *d){
        OnMiss(h,d);
    };
    c->onActived =[this](cxUdpBase *v,cxUdpHost *h){
        OnActived(h);
    };
    c->onClosed =[this](cxUdpBase *v,cxUdpHost *h){
        OnClosed(h);
    };
    Forever();
}

cxUdp::~cxUdp()
{
    c->Release();
}

void cxUdp::OnStep(cxFloat dt)
{
    c->Update();
}

void cxUdp::OnData(cxUdpHost *h,const cxUdpData *d)
{
    
}

void cxUdp::OnMiss(cxUdpHost *h,const cxUdpData *d)
{
    
}

void cxUdp::OnActived(cxUdpHost *h)
{
    
}

void cxUdp::OnClosed(cxUdpHost *h)
{
    
}

cxUdpHost *cxUdp::ConnectHost(cchars ip,cxInt port,cxUInt64 uid)
{
    return c->ConnectHost(ip, port, uid);
}

cxUdpHost *cxUdp::FindHost(cxUInt64 uid)
{
    return c->FindHost(uid);
}

cxUdp *cxUdp::Create(cchars host,cxInt port,cxUInt64 uid)
{
    cxUdp *udp = cxUdp::Create();
    if(udp->c->Init(host, port, uid) != 0){
        CX_ASSERT(false, "udp init error");
    }
    return udp;
}

CX_CPP_END

