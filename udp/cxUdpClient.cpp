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

void cxUdpClient::OnRecvFrame(UdpAddr *addr,cxAny data,cxInt size)
{
    DecodeData(addr, data, size);
}

CX_CPP_END

