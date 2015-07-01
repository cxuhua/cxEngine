//
//  cxScale.cpp
//  cxEngineCore
//
//  Created by xuhua on 6/29/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#include "cxView.h"
#include "cxScale.h"

CX_CPP_BEGIN

CX_IMPLEMENT(cxScaleBy);

cxScaleBy::cxScaleBy()
{
    
}

cxScaleBy::~cxScaleBy()
{
    
}

void cxScaleBy::OnStep(cxFloat dt)
{
    const cxPoint2F &scale = View()->Scale();
    cxPoint2F v = delta * dt + scale;
    View()->SetScale(v);
}

cxAction *cxScaleBy::Reverse()
{
    return cxScaleBy::Create(-delta, Time());
}

cxAction *cxScaleBy::Clone()
{
    return cxScaleBy::Create(delta, Time());
}

cxScaleBy *cxScaleBy::Create(const cxPoint2F &d,cxFloat time)
{
    cxScaleBy *rv = cxScaleBy::Create();
    rv->delta = d;
    rv->SetTime(time);
    return rv;
}

CX_IMPLEMENT(cxScaleTo);

cxScaleTo::cxScaleTo()
{
    
}

cxScaleTo::~cxScaleTo()
{
    
}

void cxScaleTo::OnInit()
{
    from = View()->Scale();
    delta = (to - from)/Time();
}

void cxScaleTo::OnStep(cxFloat dt)
{
    const cxPoint2F &scale = View()->Scale();
    cxPoint2F v = delta * dt + scale;
    View()->SetScale(v);
}

cxAction *cxScaleTo::Reverse()
{
    return cxScaleTo::Create(from, Time());
}

cxAction *cxScaleTo::Clone()
{
    return cxScaleTo::Create(to, Time());
}

cxScaleTo *cxScaleTo::Create(const cxPoint2F &to,cxFloat time)
{
    cxScaleTo *rv = cxScaleTo::Create();
    rv->to = to;
    rv->SetTime(time);
    return rv;
}

CX_CPP_END

