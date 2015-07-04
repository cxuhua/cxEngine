//
//  cxFade.cpp
//  cxEngineCore
//
//  Created by xuhua on 7/4/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#include "cxView.h"
#include "cxFade.h"

CX_CPP_BEGIN

CX_IMPLEMENT(cxFadeBy);

cxFadeBy::cxFadeBy()
{
    
}

cxFadeBy::~cxFadeBy()
{
    
}

void cxFadeBy::OnStep(cxFloat dt)
{
    cxFloat alpha = View()->Alpha();
    alpha += delta * dt;
    View()->SetAlpha(alpha);
}

cxFadeBy *cxFadeBy::Create(cxFloat delta,cxFloat time)
{
    cxFadeBy *rv = cxFadeBy::Create();
    rv->delta = delta;
    rv->SetTime(time);
    return rv;
}

cxAction *cxFadeBy::Reverse()
{
    return cxFadeBy::Create(-delta, Time());
}

cxAction *cxFadeBy::Clone()
{
    return cxFadeBy::Create(delta, Time());
}

CX_IMPLEMENT(cxFadeTo);

cxFadeTo::cxFadeTo()
{
    
}

cxFadeTo::~cxFadeTo()
{
    
}

void cxFadeTo::OnStep(cxFloat dt)
{
    cxFloat alpha = View()->Alpha();
    alpha += (delta * dt);
    View()->SetAlpha(alpha);
}

void cxFadeTo::OnInit()
{
    from = View()->Alpha();
    delta = (to - from)/Time();
}

cxFadeTo *cxFadeTo::Create(cxFloat to,cxFloat time)
{
    cxFadeTo *rv = cxFadeTo::Create();
    rv->to = to;
    rv->SetTime(time);
    return rv;
}

cxAction *cxFadeTo::Reverse()
{
    return cxFadeTo::Create(from, Time());
}

cxAction *cxFadeTo::Clone()
{
    return cxFadeTo::Create(to, Time());
}

CX_CPP_END

