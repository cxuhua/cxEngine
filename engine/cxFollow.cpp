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
    offset = 0.0f;
    speed = 0;
}

cxFollow::~cxFollow()
{
    
}

cxAction *cxFollow::Clone()
{
    cxView *target = GetTarget();
    CX_ASSERT(target != nullptr, "target miss,can't clone");
    return cxFollow::Create(target, speed);
}

cxFollow *cxFollow::SetOffset(const cxPoint2F &off)
{
    offset = off;
    return this;
}

void cxFollow::OnStep(cxFloat dt)
{
    if(cxFloatIsEqual(speed, 0)){
        Exit(true);
        return;
    }
    cxView *target = GetTarget();
    // target miss
    if(target == nullptr){
        onMiss.Fire(this);
        Exit(true);
        return;
    }
    // current pos
    cxPoint2F cpos = View()->Position();
    cxPoint2F tpos = target->Position() + offset;
    cxFloat angle = cpos.Angle(tpos);
    if(!cxFloatIsOK(angle)){
        return;
    }
    // new pos
    cxFloat dv = dt * speed;
    cpos.x += cosf(angle) * dv;
    cpos.y += sinf(angle) * dv;
    View()->SetPosition(cpos);
    if(View()->SetPosition(cpos, tpos)){
        onArrive.Fire(this);
        Exit(true);
        return;
    }
    // if position changed
    if(View()->IsDirtyMode(cxView::DirtyModePosition)){
        onMoving.Fire(this);
    }
    cxAction::OnStep(dt);
}

cxView *cxFollow::GetTarget()
{
    cxObject *pobj = GetBindes(1);
    if(pobj == nullptr){
        return nullptr;
    }
    return pobj->To<cxView>();
}

cxFollow *cxFollow::Create(cxView *target,cxFloat speed)
{
    cxFollow *rv = cxFollow::Create();
    rv->Bind(target,1);
    rv->Forever();
    rv->speed = speed;
    return rv;
}

CX_CPP_END

