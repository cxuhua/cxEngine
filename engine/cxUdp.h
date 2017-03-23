//
//  cxUdp.h
//  cxEngineCore
//
//  Created by xuhua on 1/12/16.
//  Copyright © 2016 xuhua. All rights reserved.
//

#ifndef cxEngineCore_cxUdp_h
#define cxEngineCore_cxUdp_h

#include <uv/uv.h>
#include <core/cxStr.h>
#include <udp/cxUdpClient.h>
#include "cxAction.h"

CX_CPP_BEGIN

class cxUdp : public cxAction
{
public:
    CX_DECLARE(cxUdp);
protected:
    explicit cxUdp();
    virtual ~cxUdp();
private:
    cxUdpClient *c;
protected:
    void OnStep(cxFloat dt);
    virtual void OnData(cxUdpHost *h,const cxUdpData *d);
    virtual void OnMiss(cxUdpHost *h,const cxUdpData *d);
    virtual void OnHostActived(cxUdpHost *h);
    virtual void OnHostClosed(cxUdpHost *h);
public:
    cxUdpHost *ConnectHost(cchars ip,cxInt port,cxUInt64 uid);
public:
    static cxUdp *Create(cchars host,cxInt port,cxUInt64 uid);
};

CX_CPP_END

#endif


