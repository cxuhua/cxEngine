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

cxPoint2F cxBezier2(cxPoint2F a, cxPoint2F b, cxPoint2F c, cxFloat t)
{
    cxFloat x = cxBezier2(a.x, b.x, c.x, t);
    cxFloat y = cxBezier2(a.y, b.y, c.y, t);
    return cxPoint2F(x, y);
}

cxPoint2F cxBezier3(cxPoint2F a, cxPoint2F b, cxPoint2F c, cxPoint2F d, cxFloat t)
{
    cxFloat x = cxBezier3(a.x, b.x, c.x, d.x, t);
    cxFloat y = cxBezier3(a.y, b.y, c.y, d.y, t);
    return cxPoint2F(x, y);
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
    cxInt ret = (m>=max?0:m);
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
    return cxPoint2F(p0.x*b1+p1.x*b2+p2.x*b3+p3.x*b4,p0.y*b1+p1.y*b2+p2.y*b3+p3.y*b4);
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

cxFloat cxDegreesToRadians(cxFloat degrees)
{
    return kmDegreesToRadians(degrees);
}

cxFloat cxRadiansToDegrees(cxFloat radians)
{
    return kmRadiansToDegrees(radians);
}

cxFloat cxModDegrees(cxFloat v)
{
    return fmod(v + 360, 360);
}

cxBool cxRadiansEqu(cxFloat a1,cxFloat a2)
{
    cxFloat d1 = cxRadiansToDegrees(a1) + 360.0f;
    cxFloat d2 = cxRadiansToDegrees(a2) + 360.0f;
    d1 = fmodf(d1, 360.0f);
    d2 = fmodf(d2, 360.0f);
    return cxFloatIsEqual(d1, d2);
}

cxInt cxHexCharToInt(cxUInt8 c)
{
    cxInt v = tolower(c);
    if(v >= 'a' && v <= 'f'){
        return (v - 'a' + 10);
    }
    if(v >= 'A' && v <= 'F'){
        return (v - 'A' + 10);
    }
    if(v >= '0' && v <= '9'){
        return (v - '0');
    }
    return 0;
}

cxBool cxFloatIsEqual(cxFloat a, cxFloat b, cxFloat equ)
{
    return fabsf(a - b) < equ;
}

cxBool cxFloatIsZero(cxFloat a)
{
    return fabsf(a) < cxEqualFloat;
}

cxBool cxFloatIsINF(cxFloat a)
{
    return isinf(a);
}

cxBool cxFloatIsOK(cxFloat a)
{
    return !isinf(a) && !isnan(a);
}

cxBool cxFloatIsNAN(cxFloat a)
{
    return isnan(a);
}

cxFloat cxFloatClamp(cxFloat x, cxFloat min, cxFloat max)
{
    return x < min ? min : (x > max ? max : x);
}

cxFloat cxFloatLerp(cxFloat from, cxFloat to, cxFloat t)
{
    return from + t * ( to - from );
}

cxFloat cxBezier2(cxFloat a, cxFloat b, cxFloat c, cxFloat t)
{
    return powf(1.0f-t,2.0f)*a+2.0f*t*(1.0f-t)*b+powf(t,2.0f)*c;
}

cxPoint2F cxBezier2(cxPoint2F a, cxPoint2F b, cxPoint2F c, cxFloat t);

cxFloat cxBezier3(cxFloat a, cxFloat b, cxFloat c, cxFloat d, cxFloat t)
{
    return powf(1.0f-t,3.0f)*a+3.0f*t*(powf(1.0f-t,2.0f))*b+3.0f*powf(t,2.0f)*(1.0f-t)*c+powf(t,3.0f)*d;
}

cxFloat cxFloatBezier(cxFloat eq0, cxFloat eq1, cxFloat eq2, cxFloat eq3, cxFloat from, cxFloat vout, cxFloat to, cxFloat vin)
{
    return from * eq0 + vout * eq1 + vin * eq2 + to * eq3;
}

cxFloat cxFloatSpline(cxFloat eq0, cxFloat eq1, cxFloat eq2, cxFloat eq3, cxFloat c0, cxFloat c1, cxFloat c2, cxFloat c3)
{
    return c0 * eq0 + c1 * eq1 + c2 * eq2 + c3 * eq3;
}

cxFloat cxFloatHermite(cxFloat h00, cxFloat h01, cxFloat h10, cxFloat h11, cxFloat from, cxFloat vout, cxFloat to, cxFloat vin)
{
    return h00 * from + h01 * to + h10 * vout + h11 * vin;
}

cxFloat cxFloatHermiteFlat(cxFloat h00, cxFloat h01, cxFloat from, cxFloat to)
{
    return h00 * from + h01 * to;
}

cxFloat cxFloatHermiteSmooth(cxFloat h00, cxFloat h01, cxFloat h10, cxFloat h11, cxFloat from, cxFloat vout, cxFloat to, cxFloat vin)
{
    return h00 * from + h01 * to + h10 * vout + h11 * vin;
}


CX_CPP_END

