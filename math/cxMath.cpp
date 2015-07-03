//
//  cxMath.cpp
//  cxEngineCore
//
//  Created by xuhua on 6/1/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//


#include "cxPoint2F.h"
#include "cxMath.h"

CX_CPP_BEGIN

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

