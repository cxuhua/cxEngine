//
//  cxBezier.cpp
//  cxEngineCore
//
//  Created by xuhua on 7/4/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#include "cxView.h"
#include "cxBezier.h"

CX_CPP_BEGIN

CX_IMPLEMENT(cxBezier);

cxBezier::cxBezier()
{
    a = 0.0;
    b = 0.0;
    c = 0.0;
}

cxBezier::~cxBezier()
{
    
}

cxBezier *cxBezier::Create(const cxPoint2F &e,const cxPoint2F &c,cxFloat time)
{
    cxBezier *rv = cxBezier::Create();
    rv->SetB(e);
    rv->SetC(c);
    rv->SetTime(time);
    return rv;
}

void cxBezier::OnInit()
{
    prev = View()->Position();
    if(a.IsZero()){
        a = prev;
    }
}

void cxBezier::OnStep(cxFloat dt)
{
    cxPoint2F cpos = View()->Position();
    cxPoint2F npos = cxBezier2(a, b, c, Progress());
    cxPoint2F diff = npos - prev;
    prev = npos;
    cpos += diff;
    View()->SetPosition(cpos);
}

cxBezier *cxBezier::SetA(const cxPoint2F &aa)
{
    a = aa;
    return this;
}

cxBezier *cxBezier::SetB(const cxPoint2F &ab)
{
    b = ab;
    return this;
}

cxBezier *cxBezier::SetC(const cxPoint2F &ac)
{
    c = ac;
    return this;
}

CX_CPP_END

