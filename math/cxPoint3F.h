//
//  cxPoint3F.h
//  cxEngineCore
//
//  Created by xuhua on 6/14/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#ifndef cxEngineCore_cxPoint3F_h
#define cxEngineCore_cxPoint3F_h

#include "cxMath.h"

CX_CPP_BEGIN

struct cxPoint2F;
struct cxMatrixF;
struct cxPoint3F
{
    cxFloat x;
    cxFloat y;
    cxFloat z;
    cxPoint3F();
    cxPoint3F(const cxJson *json);
    cxPoint3F(const cxPoint3F &v);
    cxPoint3F(cxFloat ax,cxFloat ay,cxFloat az);
    cxPoint2F ToPoint2F() const;
    cxFloat Length() const;
    cxPoint3F &Normalize();
    cxBool operator==(const cxPoint3F &v) const;
    cxBool operator!=(const cxPoint3F &v) const;
    cxPoint3F operator-(const cxPoint3F &v) const;
    cxPoint3F operator+(const cxPoint3F &v) const;
    cxPoint3F operator*(const cxMatrixF &mat);
    cxPoint3F &operator*=(const cxMatrixF &mat);
    static const cxPoint3F AxisX;
    static const cxPoint3F AxisY;
    static const cxPoint3F AxisZ;
    static cxFloat Distance(const cxPoint3F &p1,const cxPoint3F &p2);
};

CX_CPP_END

#endif


