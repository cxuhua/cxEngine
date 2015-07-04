//
//  cxMove.cpp
//  cxEngineCore
//
//  Created by xuhua on 6/28/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#include "cxView.h"
#include "cxMove.h"

CX_CPP_BEGIN

CX_IMPLEMENT(cxMoveBy);

cxMoveBy::cxMoveBy()
{
    
}

cxMoveBy::~cxMoveBy()
{
    
}

void cxMoveBy::OnStep(cxFloat dt)
{
    const cxPoint2F &cpos = View()->Position();
    cxPoint2F pos = delta * dt + cpos;
    View()->SetPosition(pos);
}

cxAction *cxMoveBy::Reverse()
{
    return cxMoveBy::Create(-delta, Time());
}

cxAction *cxMoveBy::Clone()
{
    return cxMoveBy::Create(delta, Time());
}

cxMoveBy *cxMoveBy::Create(const cxPoint2F &delta,cxFloat time)
{
    cxMoveBy *v = cxMoveBy::Create();
    v->SetTime(time);
    v->delta = delta;
    return v;
}

CX_IMPLEMENT(cxMoveTo);

cxMoveTo::cxMoveTo()
{
    
}

cxMoveTo::~cxMoveTo()
{
    
}

void cxMoveTo::OnInit()
{
    from = View()->Position();
    delta = (to - from) / Time();
}

void cxMoveTo::OnStep(cxFloat dt)
{
    const cxPoint2F &cpos = View()->Position();
    cxPoint2F pos = delta * dt + cpos;
    View()->SetPosition(pos);
}

cxAction *cxMoveTo::Reverse()
{
    return cxMoveTo::Create(from, Time());
}

cxAction *cxMoveTo::Clone()
{
    return cxMoveTo::Create(to, Time());
}

cxMoveTo *cxMoveTo::Create(const cxPoint2F &to,cxFloat time)
{
    cxMoveTo *v = cxMoveTo::Create();
    v->SetTime(time);
    v->to = to;
    return v;
}

CX_CPP_END

