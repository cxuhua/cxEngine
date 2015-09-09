//
//  cxBoxCoord2F.cpp
//  cxEngineCore
//
//  Created by xuhua on 6/17/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//


#include "cxBoxCoord2F.h"

CX_CPP_BEGIN

cxCoord2F::cxCoord2F()
{
    
}

cxCoord2F::cxCoord2F(cxFloat au,cxFloat av)
{
    u = au;
    v = av;
}

cxCoord2F::cxCoord2F(const cxCoord2F &av)
{
    u = av.u;
    v = av.v;
}

const cxFloat cxBoxCoord2F::X() const
{
    return lt.u;
}

const cxFloat cxBoxCoord2F::Y() const
{
    return lt.v;
}

const cxFloat cxBoxCoord2F::W() const
{
    return rt.u - lt.u;
}

const cxFloat cxBoxCoord2F::H() const
{
    return lb.v - lt.v;
}

cxBoxCoord2F cxBoxCoord2F::Default = cxBoxCoord2F(cxCoord2F(0.0f,0.0f),cxCoord2F(0.0f,1.0f),cxCoord2F(1.0f,0.0f),cxCoord2F(1.0f,1.0f));

cxBoxCoord2F::cxBoxCoord2F(const cxBoxCoord2F &v)
{
    lt = v.lt;
    lb = v.lb;
    rt = v.rt;
    rb = v.rb;
}

cxBoxCoord2F::cxBoxCoord2F(cxFloat l,cxFloat r,cxFloat t,cxFloat b)
{
    lt = cxCoord2F(l,t);
    lb = cxCoord2F(l,b);
    rt = cxCoord2F(r,t);
    rb = cxCoord2F(r,b);
}

cxBoxCoord2F::cxBoxCoord2F()
{
    lt = cxCoord2F(0,0);
    lb = cxCoord2F(0,1);
    rt = cxCoord2F(1,0);
    rb = cxCoord2F(1,1);
}

cxBoxCoord2F::cxBoxCoord2F(const cxCoord2F &alt,const cxCoord2F &alb,const cxCoord2F &art,const cxCoord2F &arb)
{
    lt = alt;
    lb = alb;
    rt = art;
    rb = arb;
}

CX_CPP_END

