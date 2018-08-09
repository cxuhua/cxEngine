//
//  cxMath.h
//  cxEngineCore
//
//  Created by xuhua on 6/1/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#ifndef cxEngineCore_cxMath_h
#define cxEngineCore_cxMath_h

#include <core/cxObject.h>
#include <float.h>
#include <math.h>
#include <vector>
#include <ext/kazmath.h>

CX_CPP_BEGIN

#ifndef MATH_PI
#define MATH_PI 3.14159265358979323846f
#endif

#ifndef MATH_PIO2
#define MATH_PIO2 1.57079632679489661923f
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

class cxTile
{
private:
    cxFloat w2;
    cxFloat h2;
    cxFloat aa;
    cxFloat sa;
    cxFloat ca;
    cxFloat m;
public:
    cxTile();
    cxTile(const cxSize2F &size);
    cxPoint2F ToPos(const cxPoint2F &idx);
    cxPoint2F ToIdx(const cxPoint2F &pos);
};

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

cxFloat cxDegreesToRadians(cxFloat degrees);

cxFloat cxRadiansToDegrees(cxFloat radians);

cxFloat cxModDegrees(cxFloat v);

cxBool cxRadiansEqu(cxFloat a1,cxFloat a2);

cxInt cxHexCharToInt(cxUInt8 c);

cxBool cxFloatIsEqual(cxFloat a, cxFloat b, cxFloat equ=cxEqualFloat);

cxBool cxFloatIsZero(cxFloat a);

cxBool cxFloatIsINF(cxFloat a);

cxBool cxFloatIsOK(cxFloat a);

cxBool cxFloatIsNAN(cxFloat a);

cxFloat cxFloatClamp(cxFloat x, cxFloat min, cxFloat max);

cxFloat cxFloatLerp(cxFloat from, cxFloat to, cxFloat t);

cxFloat cxBezier2(cxFloat a, cxFloat b, cxFloat c, cxFloat t);

cxPoint2F cxBezier2(cxPoint2F a, cxPoint2F b, cxPoint2F c, cxFloat t);

cxFloat cxBezier3(cxFloat a, cxFloat b, cxFloat c, cxFloat d, cxFloat t);

cxPoint2F cxBezier3(cxPoint2F a, cxPoint2F b, cxPoint2F c, cxPoint2F d, cxFloat t);

cxFloat cxFloatBezier(cxFloat eq0, cxFloat eq1, cxFloat eq2, cxFloat eq3, cxFloat from, cxFloat vout, cxFloat to, cxFloat vin);

cxFloat cxFloatSpline(cxFloat eq0, cxFloat eq1, cxFloat eq2, cxFloat eq3, cxFloat c0, cxFloat c1, cxFloat c2, cxFloat c3);

cxFloat cxFloatHermite(cxFloat h00, cxFloat h01, cxFloat h10, cxFloat h11, cxFloat from, cxFloat vout, cxFloat to, cxFloat vin);

cxFloat cxFloatHermiteFlat(cxFloat h00, cxFloat h01, cxFloat from, cxFloat to);

cxFloat cxFloatHermiteSmooth(cxFloat h00, cxFloat h01, cxFloat h10, cxFloat h11, cxFloat from, cxFloat vout, cxFloat to, cxFloat vin);

CX_CPP_END

#endif


