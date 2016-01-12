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
    uv_loop_t looper;
    uv_udp_t handle;
    struct addrinfo hints;
protected:
    void OnStep(cxFloat dt);
public:
    cxBool Init(cchars host,cxInt port);
    static cxUdp *Create(cchars host,cxInt port);
};

CX_CPP_END

#endif


