//
//  cxBoxPoint3F.cpp
//  cxEngineCore
//
//  Created by xuhua on 6/17/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#include "cxMatrixF.h"
#include "cxBox4F.h"
#include "cxBoxPoint3F.h"
#include "cxRect4F.h"

CX_CPP_BEGIN

cxBoxPoint3F::cxBoxPoint3F()
{
    
}

cxBoxPoint3F::cxBoxPoint3F(const cxBox4F &v)
{
    lt = cxPoint3F(v.l, v.t, 0);
    lb = cxPoint3F(v.l, v.b, 0);
    rt = cxPoint3F(v.r, v.t, 0);
    rb = cxPoint3F(v.r, v.b, 0);
}

cxBoxPoint3F::cxBoxPoint3F(const cxBoxPoint3F &v)
{
    lt = v.lt;
    lb = v.lb;
    rt = v.rt;
    rb = v.rb;
}

void cxBoxPoint3F::Set(cxFloat l,cxFloat r,cxFloat t,cxFloat b)
{
    lt.x = l;lt.y = t;lt.z = 0;
    lb.x = l;lb.y = b;lb.z = 0;
    rt.x = r;rt.y = t;rt.z = 0;
    rb.x = r;rb.y = b;rb.z = 0;
}

cxBoxPoint3F::cxBoxPoint3F(const cxPoint3F &alt,const cxPoint3F &alb,const cxPoint3F &art,const cxPoint3F &arb)
{
    lt = alt;
    lb = alb;
    rt = art;
    rb = arb;
}

const cxBox4F cxBoxPoint3F::ToMaxBox() const
{
    cxFloat minx = INT_MAX;
    cxFloat maxx = INT_MIN;
    cxFloat miny = INT_MAX;
    cxFloat maxy = INT_MIN;
    cxPoint3F *p = (cxPoint3F *)this;
    for(cxInt i=0;i < 4; i++,p++){
        if(p->x <= minx){
            minx = p->x;
        }
        if(p->x >= maxx){
            maxx = p->x;
        }
        if(p->y <= miny){
            miny = p->y;
        }
        if(p->y >= maxy){
            maxy = p->y;
        }
    }
    return cxBox4F(minx, maxx, maxy, miny);
}

const cxBox4F cxBoxPoint3F::ToBox4F() const
{
    return cxBox4F(lt.x, rt.x, lt.y, lb.y);
}

const cxSize2F cxBoxPoint3F::Size() const
{
    cxFloat w = cxPoint3F::Distance(lt, rt);
    cxFloat h = cxPoint3F::Distance(lt, rb);
    return cxSize2F(w, h);
}

cxBoxPoint3F &cxBoxPoint3F::operator*=(const cxMatrixF &mat)
{
    lt *= mat;
    lb *= mat;
    rt *= mat;
    rb *= mat;
    return *this;
}

cxBoxPoint3F cxBoxPoint3F::operator*(const cxMatrixF &mat)
{
    return cxBoxPoint3F(lt*mat,lb*mat,rt*mat,rb*mat);
}

const cxBoxPoint3F cxBoxPoint3F::ToLoop() const
{
    cxBoxPoint3F ret;
    ret.lt = lt;
    ret.lb = lb;
    ret.rt = rb;
    ret.rb = rt;
    return ret;
}

CX_CPP_END

