//
//  cxMath.cpp
//  cxEngineCore
//
//  Created by xuhua on 6/1/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//


#include "cxSize2F.h"
#include "cxPoint2F.h"
#include "cxMath.h"

CX_CPP_BEGIN

cxTile::cxTile()
{
    cxTile(cxSize2F(40, 30));
}

cxTile::cxTile(const cxSize2F &size)
{
    w2 = size.w / 2;
    h2 = size.h / 2;
    aa = atan2f(h2, w2);
    sa = sinf(aa);
    ca = cosf(aa);
    m = sqrtf(w2*w2 + h2*h2);
}

cxPoint2F cxTile::ToPos(const cxPoint2F &idx)
{
    cxFloat dx = m * idx.x;
    cxFloat dy = m * idx.y;
    cxPoint2F p1 = cxPoint2F(0, 0);
    p1.x = ca * dx;
    p1.y = sa * dx;
    cxPoint2F p2 = cxPoint2F(0, 0);
    p2.x = ca * dy;
    p2.y = sa * dy;
    return cxPoint2F(p1.x - p2.x,p1.y + p2.y);
}

cxPoint2F cxTile::ToIdx(const cxPoint2F &pos)
{
    cxFloat dx = (pos.y / sa + pos.x / ca) / 2.0f;
    cxFloat dy = dx - pos.x / ca;
    return cxPoint2F(dx/m, dy/m);
}

cxInt cxAngleToIndex(cxFloat angle,cxInt split,cxFloat *off)
{
    cxFloat avalue = cxRadiansToDegrees(angle);
    avalue = fmodf(avalue,360.0f);
    avalue = fmodf(avalue+360.0f,360.0f);
    cxFloat v = (cxFloat)split/2.0f;
    cxInt max = 360/split;
    cxInt idx = avalue/v;
    cxInt m = (idx/2) + (idx%2);
    cxInt ret = m>=max?0:m;
    if(off != NULL){
        *off = cxDegreesToRadians(avalue - ret * 10);
    }
    return ret;
}

cxPoint2F cxCardinalSplineAt(const cxPoint2F &p0,const cxPoint2F &p1,const cxPoint2F &p2,const cxPoint2F &p3, cxFloat tension, cxFloat t)
{
    cxFloat t2 = t * t;
    cxFloat t3 = t2 * t;
    cxFloat s = (1.0f - tension) / 2.0f;
    cxFloat b1 = s * ((-t3 + (2.0f * t2)) - t);
    cxFloat b2 = s * (-t3 + t2) + (2.0f * t3 - 3.0f * t2 + 1.0f);
    cxFloat b3 = s * (t3 - 2.0f * t2 + t) + (-2.0f * t3 + 3.0f * t2);
    cxFloat b4 = s * (t3 - t2);
    cxPoint2F rv = cxPoint2F(0.0f, 0.0f);
    rv.x = (p0.x*b1 + p1.x*b2 + p2.x*b3 + p3.x*b4);
    rv.y = (p0.y*b1 + p1.y*b2 + p2.y*b3 + p3.y*b4);
    return rv;
}

cxFloatRange::cxFloatRange()
{
    v = 0;
    r = 0;
}

cxFloatRange::cxFloatRange(const cxFloatRange &av)
{
    v = av.v;
    r = av.r;
}

cxFloatRange::cxFloatRange(cxFloat av,cxFloat ar)
{
    v = av;
    r = ar;
}

const cxFloat cxFloatRange::ToValue() const
{
    return v + r * CX_RAND_11f();
}

const cxFloat cxFloatRange::ToRadians() const
{
    return cxDegreesToRadians(ToValue());
}

CX_CPP_END

