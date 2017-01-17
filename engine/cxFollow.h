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

#define CX_FOLLOW_MAX   4

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
    cxFloat speed;
public:
    cxView *GetTarget();
    cxFollow *SetOffset(const cxPoint2F &off);
    // not imp Reverse
    // cxAction *Reverse();
    cxAction *Clone();
public:
    cxEvent<cxFollow> onMoving;
    cxEvent<cxFollow> onMiss;
public:
    static cxFollow *Create(cxView *target,cxFloat speed);
};

CX_CPP_END

#endif


