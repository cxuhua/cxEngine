//
//  cxForward.cpp
//  cxEngineCore
//
//  Created by xuhua on 29/03/2017.
//  Copyright © 2017 xuhua. All rights reserved.
//


#include "cxView.h"
#include "cxForward.h"

CX_CPP_BEGIN

CX_IMPLEMENT(cxForward);

cxForward::cxForward()
{
    Forever();
}

cxForward::~cxForward()
{
    
}

void cxForward::SetSpeed(const cxFloat &v)
{
    speed = v;
}

void cxForward::SetAngle(const cxFloat &v)
{
    angle.x = sinf(v);
    angle.y = cosf(v);
}

void cxForward::SetDegrees(const cxFloat &v)
{
    SetAngle(cxDegreesToRadians(v));
}

void cxForward::OnStep(cxFloat dt)
{
    cxAction::OnStep(dt);
    cxPoint2F cp = View()->Position() + angle * speed * dt;
    View()->SetPosition(cp);
}

void cxForward::OnInit()
{
    cxAction::OnInit();
}

CX_CPP_END

