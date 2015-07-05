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
    a = INFINITY;
    b = 0.0;
    c = 0.0;
    d = INFINITY;
}

cxBezier::~cxBezier()
{
    
}

cxBezier *cxBezier::Create(const cxPoint2F &b,const cxPoint2F &c,cxFloat time)
{
    cxBezier *rv = cxBezier::Create();
    rv->SetB(b);
    rv->SetC(c);
    rv->SetTime(time);
    return rv;
}

cxBezier *cxBezier::Create(const cxPoint2F &b,const cxPoint2F &c,const cxPoint2F &d,cxFloat time)
{
    cxBezier *rv = cxBezier::Create();
    rv->SetB(b);
    rv->SetC(c);
    rv->SetD(d);
    rv->SetTime(time);
    return rv;
}

cxAction *cxBezier::Reverse()
{
    cxBezier *rv = cxBezier::Create();
    rv->SetTime(Time());
    rv->a = d;
    rv->b = c;
    rv->c = b;
    rv->d = a;
    return rv;
}

cxAction *cxBezier::Clone()
{
    cxBezier *rv = cxBezier::Create();
    rv->SetTime(Time());
    rv->a = a;
    rv->b = b;
    rv->c = c;
    rv->d = d;
    return rv;
}

void cxBezier::OnInit()
{
    prev = View()->Position();
    if(a.IsINF())a = prev;
}

void cxBezier::OnStep(cxFloat dt)
{
    cxPoint2F npos;
    if(d.IsINF()){
        npos = cxBezier2(a, b, c, Progress());
    }else{
        npos = cxBezier3(a, b, c, d, Progress());
    }
    cxPoint2F diff = npos - prev;
    prev = npos;
    View()->AddPosition(diff);
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

cxBezier *cxBezier::SetD(const cxPoint2F &ad)
{
    d = ad;
    return this;
}

CX_CPP_END

