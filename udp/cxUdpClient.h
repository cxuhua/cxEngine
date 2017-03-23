//
//  cxUdpClient.h
//  udp
//
//  Created by xuhua on 17/03/2017.
//  Copyright © 2017 xuhua. All rights reserved.
//

#ifndef udp_cxUdpClient_h
#define udp_cxUdpClient_h

#include "cxUdpBase.h"

CX_CPP_BEGIN

class cxUdpClient : public cxUdpBase
{
public:
    CX_DECLARE(cxUdpClient);
protected:
    explicit cxUdpClient();
    virtual ~cxUdpClient();
protected:
    void OnRecvFrame(UdpAddr *addr,cxAny data,cxInt size);
public:
   
};

CX_CPP_END

#endif


