//
//  cxResize.cpp
//  cxEngineCore
//
//  Created by xuhua on 7/7/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#include "cxView.h"
#include "cxResize.h"

CX_CPP_BEGIN

CX_IMPLEMENT(cxResizeBy);

cxResizeBy::cxResizeBy()
{
    
}

cxResizeBy::~cxResizeBy()
{
    
}

cxAction *cxResizeBy::Reverse()
{
    return cxResizeBy::Create(-delta, Time());
}

cxAction *cxResizeBy::Clone()
{
    return cxResizeBy::Create(delta, Time());
}

void cxResizeBy::OnStep(cxFloat dt)
{
    cxSize2F size = View()->Size();
    size += delta *dt;
    View()->SetSize(size);
}

cxResizeBy *cxResizeBy::Create(const cxSize2F &delta,cxFloat time)
{
    cxResizeBy *rv = cxResizeBy::Create();
    rv->delta = delta;
    rv->SetTime(time);
    return rv;
}

CX_IMPLEMENT(cxResizeTo);

cxResizeTo::cxResizeTo()
{
    
}

void cxResizeTo::OnInit()
{
    from = View()->Size();
    delta = (to - from)/Time();
}

void cxResizeTo::OnStep(cxFloat dt)
{
    cxSize2F size = View()->Size();
    size += delta *dt;
    View()->SetSize(size);
}

cxResizeTo::~cxResizeTo()
{
    
}

cxResizeTo *cxResizeTo::Create(const cxSize2F &to,cxFloat time)
{
    cxResizeTo *rv = cxResizeTo::Create();
    rv->to = to;
    rv->SetTime(time);
    return rv;
}

cxAction *cxResizeTo::Reverse()
{
    return Create(from, Time());
}

cxAction *cxResizeTo::Clone()
{
    return Create(to, Time());
}

CX_CPP_END

