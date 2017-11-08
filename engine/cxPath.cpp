//
//  cxPath.cpp
//  cxEngineCore
//
//  Created by xuhua on 7/21/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#include "cxView.h"
#include "cxPath.h"

CX_CPP_BEGIN

CX_IMPLEMENT(cxPath);

cxPath::cxPath()
{
    speed = 100;
}

cxPath::~cxPath()
{
    
}

void cxPath::OnInit()
{
    if(points.Size() < 2){
        Exit(true);
        return;
    }
    cxTimeLine::OnInit();
    next = points.At(0);
    View()->SetPosition(next);
}

void cxPath::OnStep(cxFloat dt)
{
    cxTimeLine::OnStep(dt);
    
    cxPoint2F cpos = View()->Position();
    cxFloat a = cpos.Angle(next);
    if(!cxFloatIsOK(a)){
        return;
    }
    cpos += cxPoint2F(cosf(a), sinf(a)) * dt * speed;
    View()->SetPosition(cpos);
}

void cxPath::OnTime(const cxTimePoint *tp)
{
    cxInt idx = Index();
    if(idx == points.Size() - 1){
        View()->SetPosition(points.At(idx));
        Exit(true);
        return;
    }
    next = points.At(idx+1);
    cxTimeLine::OnTime(tp);
}

cxPath *cxPath::Create(cxFloat speed)
{
    cxPath *rv = cxPath::Create();
    rv->speed = speed;
    return rv;
}

void cxPath::SetMoveSpeed(cxFloat v)
{
    speed = v;
}

cxPath *cxPath::Append(const cxPoint2F &pos)
{
    CX_ASSERT(speed > 0, "speed not set");
    cxInt num = points.Size();
    if(num == 0){
        points.Append(pos);
        return this;
    }
    cxPoint2F prev = points.At(num - 1);
    if(pos == prev){
        return this;
    }
    cxFloat dis = pos.Distance(prev);
    cxTimeLine::Append(dis/speed);
    points.Append(pos);
    return this;
}

CX_CPP_END

