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
    c = nullptr;
}

cxUdp::~cxUdp()
{
    cxObject::release(&c);
}

void cxUdp::OnStep(cxFloat dt)
{
    CX_ASSERT(c != nullptr, "c null");
    c->Update();
}

void cxUdp::OnData(cxUdpHost *h,const cxUdpData *d)
{
    
}

void cxUdp::OnMiss(cxUdpHost *h,const cxUdpData *d)
{
    
}

void cxUdp::OnHostActived(cxUdpHost *h)
{
    
}

void cxUdp::OnHostClosed(cxUdpHost *h)
{
    
}

cxUdpHost *cxUdp::ConnectHost(cchars ip,cxInt port,cxUInt64 uid)
{
    return c->ConnectHost(ip, port, uid);
}

cxUdp *cxUdp::Create(cchars host,cxInt port,cxUInt64 uid)
{
    cxUdp *udp = cxUdp::Create();
    cxUdpClient *c = cxUdpClient::Create();
    if(c->Init(host, port, uid) != 0){
        return nullptr;
    }
    cxObject::swap(&udp->c, c);
    udp->Forever();
    c->onData+=[udp](cxUdpBase *c,cxUdpHost *h,const cxUdpData *data){
        udp->OnData(h,data);
    };
    c->onMiss+=[udp](cxUdpBase *c,cxUdpHost *h,const cxUdpData *data){
        udp->OnMiss(h,data);
    };
    c->onActived+=[udp](cxUdpBase *c,cxUdpHost *h){
        udp->OnHostActived(h);
    };
    c->onClosed+=[udp](cxUdpBase *c,cxUdpHost *h){
        udp->OnHostClosed(h);
    };
    return udp;
}

CX_CPP_END

