//
//  cxMath.h
//  cxEngineCore
//
//  Created by xuhua on 6/1/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#ifndef cxEngineCore_cxMath_h
#define cxEngineCore_cxMath_h

#include <vector>
#include <core/cxObject.h>
#include "kazmath/kazmath.h"

CX_CPP_BEGIN

#ifndef MATH_PI
#define MATH_PI 3.14159265358979323846f
#endif

#ifndef MATH_PIOVER2
#define MATH_PIOVER2 1.57079632679489661923f
#endif

#ifndef MATH_PIX2
#define MATH_PIX2 6.28318530717958647693f
#endif

// -1.0f <-> 1.0f
#define CX_RAND_11f() ((2.0f*((cxFloat)(rand()%RAND_MAX)/(cxFloat)RAND_MAX))-1.0f)

// 0.0f <-> 1.0f
#define CX_RAND_01f() ((cxFloat)(rand()%RAND_MAX)/(cxFloat)RAND_MAX)

struct cxPoint2F;
struct cxSize2F;

const cxFloat cxEqualFloat = cxFloat(0.0001f);

struct cxFloatRange
{
    cxFloat v;
    cxFloat r;
    cxFloatRange();
    cxFloatRange(const cxFloatRange &av);
    cxFloatRange(cxFloat av,cxFloat ar);
    const cxFloat ToValue() const;
    const cxFloat ToRadians() const;
};

cxPoint2F cxTileIdxToPos(const cxPoint2F &idx,const cxSize2F &size);

cxPoint2F cxTilePosToIdx(const cxPoint2F &pos,const cxSize2F &size);

cxInt cxAngleToIndex(cxFloat angle,cxInt split,cxFloat *off);

cxPoint2F cxCardinalSplineAt(const cxPoint2F &p0,const cxPoint2F &p1,const cxPoint2F &p2,const cxPoint2F &p3, cxFloat tension, cxFloat t);

CX_INLINE cxFloat cxDegreesToRadians(cxFloat degrees)
{
    return kmDegreesToRadians(degrees);
}

CX_INLINE cxFloat cxRadiansToDegrees(cxFloat radians)
{
    return kmRadiansToDegrees(radians);
}

CX_INLINE cxInt cxCharToInt(cxUInt8 c)
{
    cxInt v = tolower(c);
    return v >= 'a' ? (v - 'a' + 10) : (v - '0');
}

CX_INLINE cxBool cxFloatIsEqual(cxFloat a, cxFloat b)
{
    return fabsf(a - b) < cxEqualFloat;
}

CX_INLINE cxBool cxFloatIsZero(cxFloat a)
{
    return fabsf(a) < cxEqualFloat;
}

CX_INLINE cxBool cxFloatIsINF(cxFloat a)
{
    return isinf(a);
}

CX_INLINE cxBool cxFloatIsNAN(cxFloat a)
{
    return isnan(a);
}

CX_INLINE cxFloat cxFloatClamp(cxFloat x, cxFloat min, cxFloat max)
{
    return x < min ? min : (x > max ? max : x);
}

CX_INLINE cxFloat cxFloatLerp(cxFloat from, cxFloat to, cxFloat t )
{
    return from + t * ( to - from );
}

CX_INLINE cxFloat cxFloatBezier(cxFloat eq0, cxFloat eq1, cxFloat eq2, cxFloat eq3, cxFloat from, cxFloat vout, cxFloat to, cxFloat vin)
{
    return from * eq0 + vout * eq1 + vin * eq2 + to * eq3;
}

CX_INLINE cxFloat cxFloatSpline(cxFloat eq0, cxFloat eq1, cxFloat eq2, cxFloat eq3, cxFloat c0, cxFloat c1, cxFloat c2, cxFloat c3)
{
    return c0 * eq0 + c1 * eq1 + c2 * eq2 + c3 * eq3;
}

CX_INLINE cxFloat cxFloatHermite(cxFloat h00, cxFloat h01, cxFloat h10, cxFloat h11, cxFloat from, cxFloat vout, cxFloat to, cxFloat vin)
{
    return h00 * from + h01 * to + h10 * vout + h11 * vin;
}

CX_INLINE cxFloat cxFloatHermiteFlat(cxFloat h00, cxFloat h01, cxFloat from, cxFloat to)
{
    return h00 * from + h01 * to;
}

CX_INLINE cxFloat cxFloatHermiteSmooth(cxFloat h00, cxFloat h01, cxFloat h10, cxFloat h11, cxFloat from, cxFloat vout, cxFloat to, cxFloat vin)
{
    return h00 * from + h01 * to + h10 * vout + h11 * vin;
}

CX_CPP_END

#endif


