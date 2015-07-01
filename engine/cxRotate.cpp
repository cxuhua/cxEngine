//
//  cxRotate.cpp
//  cxEngineCore
//
//  Created by xuhua on 6/29/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#include "cxView.h"
#include "cxRotate.h"

CX_CPP_BEGIN

CX_IMPLEMENT(cxRotateBy);

cxRotateBy::cxRotateBy()
{
    
}

cxRotateBy::~cxRotateBy()
{
    
}

void cxRotateBy::OnStep(cxFloat dt)
{
    cxFloat angle = View()->Angle();
    angle += dt * delta;
    View()->SetAngle(angle);
}

cxAction *cxRotateBy::Reverse()
{
    return cxRotateBy::Create(-delta, Time());
}

cxAction *cxRotateBy::Clone()
{
    return cxRotateBy::Create(delta, Time());
}

cxRotateBy *cxRotateBy::Create(cxFloat delta,cxFloat time)
{
    cxRotateBy *rv = cxRotateBy::Create();
    rv->delta = delta;
    rv->SetTime(time);
    return rv;
}

CX_IMPLEMENT(cxRotateTo);

cxRotateTo::cxRotateTo()
{
    
}

cxRotateTo::~cxRotateTo()
{
    
}

void cxRotateTo::OnInit()
{
    from = View()->Angle();
    delta = (to - from)/Time();
}

void cxRotateTo::OnStep(cxFloat dt)
{
    cxFloat angle = View()->Angle();
    angle += dt * delta;
    View()->SetAngle(angle);
}

cxAction *cxRotateTo::Reverse()
{
    return cxRotateTo::Create(from, Time());
}

cxAction *cxRotateTo::Clone()
{
    return cxRotateTo::Create(to, Time());
}

cxRotateTo *cxRotateTo::Create(cxFloat to,cxFloat time)
{
    cxRotateTo *rv = cxRotateTo::Create();
    rv->to = to;
    rv->SetTime(time);
    return rv;
}

CX_CPP_END

