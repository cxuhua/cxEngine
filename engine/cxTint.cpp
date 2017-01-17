//
//  cxTint.cpp
//  cxEngineCore
//
//  Created by xuhua on 7/4/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#include "cxView.h"
#include "cxTint.h"

CX_CPP_BEGIN

CX_IMPLEMENT(cxTintBy);

cxTintBy::cxTintBy()
{
    
}

cxTintBy::~cxTintBy()
{
    
}

void cxTintBy::OnStep(cxFloat dt)
{
    cxColor4F color = View()->Color();
    color += delta * dt;
    View()->SetColor(color);
}

cxTintBy *cxTintBy::Create(const cxColor4F &delta,cxFloat time)
{
    cxTintBy *rv = cxTintBy::Create();
    rv->delta = delta;
    rv->SetTime(time);
    return rv;
}

cxTintBy *cxTintBy::Create(cxFloat delta,cxFloat time)
{
    cxTintBy *rv = cxTintBy::Create();
    rv->delta = cxColor4F(delta, delta, delta, 0.0f);
    rv->SetTime(time);
    return rv;
}

cxAction *cxTintBy::Reverse()
{
    return cxTintBy::Create(-delta, Time());
}

cxAction *cxTintBy::Clone()
{
    return cxTintBy::Create(delta, Time());
}

CX_IMPLEMENT(cxTintTo);

cxTintTo::cxTintTo()
{
    
}

cxTintTo::~cxTintTo()
{
    
}

void cxTintTo::OnStep(cxFloat dt)
{
    cxColor4F color = View()->Color();
    color += delta * dt;
    View()->SetColor(color);
}

void cxTintTo::OnInit()
{
    cxFloat t = Time();
    from = View()->Color();
    to.a = from.a;
    delta = (to - from)/Time();
}

cxTintTo *cxTintTo::Create(cxFloat to,cxFloat time)
{
    cxTintTo *rv = cxTintTo::Create();
    rv->to = cxColor4F(to, to, to, 0.0f);
    rv->SetTime(time);
    return rv;
}

cxTintTo *cxTintTo::Create(const cxColor4F &to,cxFloat time)
{
    cxTintTo *rv = cxTintTo::Create();
    rv->to = to;
    rv->SetTime(time);
    return rv;
}

cxAction *cxTintTo::Reverse()
{
    return cxTintTo::Create(from, Time());
}

cxAction *cxTintTo::Clone()
{
    return cxTintTo::Create(to, Time());
}

CX_CPP_END

