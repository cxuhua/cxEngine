//
//  cxUdp.cpp
//  cxEngineCore
//
//  Created by xuhua on 1/12/16.
//  Copyright © 2016 xuhua. All rights reserved.
//


#include "cxUdp.h"

CX_CPP_BEGIN

CX_IMPLEMENT(cxPacket)

cxPacket::cxPacket()
{
    
}

cxPacket::~cxPacket()
{
    
}

CX_IMPLEMENT(cxUdp);

cxUdp::cxUdp()
{
    packets = cxArray::Alloc();
    host = nullptr;
    port = 0;
    uv_loop_init(&looper);
    uv_udp_init(&looper, &handle);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_protocol = IPPROTO_UDP;
    hints.ai_flags = 0;
}

cxUdp::~cxUdp()
{
    packets->Release();
    cxObject::release(&host);
    uv_loop_close(&looper);
}

void cxUdp::OnStep(cxFloat dt)
{
    uv_run(&looper, UV_RUN_NOWAIT);
}

cxBool cxUdp::Init(cchars ahost,cxInt aport)
{
    cxObject::swap(&host, cxStr::Create(ahost));
    port = aport;
    return true;
}

cxUdp *Create(cchars host,cxInt port)
{
    cxUdp *udp = cxUdp::Create();
    if(!udp->Init(host, port)){
        CX_ERROR("init udp error");
        return nullptr;
    }
    return udp;
}

CX_CPP_END

