//
//  cxFollow.h
//  cxEngineCore
//
//  Created by xuhua on 7/4/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#ifndef cxEngineCore_cxFollow_h
#define cxEngineCore_cxFollow_h

#include "cxAction.h"

CX_CPP_BEGIN

class cxFollow : public cxAction
{
public:
    CX_DECLARE(cxFollow);
protected:
    explicit cxFollow();
    virtual ~cxFollow();
protected:
    void OnStep(cxFloat dt);
private:
    cxPoint2F offset;
    cxView *target;
    cxFloat speed;
public:
    cxFollow *SetOffset(const cxPoint2F &off);
    cxEvent<cxFollow,cxFloat> onDistance;
    static cxFollow *Create(cxView *target,cxFloat speed);
};

CX_CPP_END

#endif


