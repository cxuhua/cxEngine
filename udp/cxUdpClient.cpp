//
//  cxUdpClient.cpp
//  udp
//
//  Created by xuhua on 17/03/2017.
//  Copyright © 2017 xuhua. All rights reserved.
//

#include "cxUdpData.h"
#include "cxUdpClient.h"

CX_CPP_BEGIN

CX_IMPLEMENT(cxUdpClient);

cxUdpClient::cxUdpClient()
{
    
}

cxUdpClient::~cxUdpClient()
{
    
}

void cxUdpClient::RecvData(cxUdpHost *h,const cxUdpData *d)
{
    CX_LOGGER("%llu RECV %llu DATA SEQ=%u",d->Dst(),d->Src(), d->Seq());
    cxUdpBase::RecvData(h, d);
}

void cxUdpClient::RecvFrame(UdpAddr *addr,cxAny data,cxInt size)
{
    DecodeData(addr, data, size);
}

CX_CPP_END

