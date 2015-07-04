//
//  cxFollow.cpp
//  cxEngineCore
//
//  Created by xuhua on 7/4/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#include "cxView.h"
#include "cxFollow.h"

CX_CPP_BEGIN

CX_IMPLEMENT(cxFollow);

cxFollow::cxFollow()
{
    target = nullptr;
    speed = 0;
}

cxFollow::~cxFollow()
{
    cxObject::release(&target);
}

void cxFollow::OnStep(cxFloat dt)
{
    CX_ASSERT(target != nullptr, "not set target position");
    if(cxFloatIsEqual(speed, 0)){
        SetExit(true);
        return;
    }
    cxPoint2F cpos = View()->Position();
    cxPoint2F tpos = target->Position();
    cxFloat angle = cpos.Angle(tpos);
    if(!cxFloatIsOK(angle)){
        return;
    }
    cxFloat dv = dt * speed;
    cpos.x += cosf(angle) * dv;
    cpos.y += sinf(angle) * dv;
    View()->SetPosition(cpos);
    if(cpos.Distance(tpos) < dv){
        onCollide.Fire(this);
    }
}

cxFollow *cxFollow::Create(cxView *target,cxFloat speed)
{
    cxFollow *rv = cxFollow::Create();
    cxObject::swap(&rv->target, target);
    rv->Forever();
    rv->speed = speed;
    return rv;
}

CX_CPP_END

