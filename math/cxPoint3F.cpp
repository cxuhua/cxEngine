//
//  cxPoint3F.cpp
//  cxEngineCore
//
//  Created by xuhua on 6/14/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#include <core/cxJson.h>
#include "kazmath/kazmath.h"
#include "cxPoint3F.h"
#include "cxPoint2F.h"
#include "cxMatrixF.h"

CX_CPP_BEGIN

const cxPoint3F cxPoint3F::AxisX = cxPoint3F(1,0,0);
const cxPoint3F cxPoint3F::AxisY = cxPoint3F(0,1,0);
const cxPoint3F cxPoint3F::AxisZ = cxPoint3F(0,0,1);

cxPoint3F::cxPoint3F()
{
    x = 0;
    y = 0;
    z = 0;
}

cxPoint3F::cxPoint3F(const cxFloat v)
{
    x = v;
    y = v;
    z = v;
}

cxPoint3F::cxPoint3F(const cxPoint3F &v)
{
    x = v.x;
    y = v.y;
    z = v.z;
}

cxPoint3F::cxPoint3F(const cxJson *json)
{
    CX_ASSERT(json != nullptr && json->IsObject(), "json format error");
    x = json->Get("x", x);
    y = json->Get("y", y);
    z = json->Get("z", z);
}

cxJson *cxPoint3F::ToJson()
{
    cxJson *json = cxJson::Create()->Object();
    json->Set("x", x);
    json->Set("y", y);
    json->Set("z", z);
    return json;
}

cxPoint3F::cxPoint3F(cxFloat ax,cxFloat ay,cxFloat az)
{
    x = ax;
    y = ay;
    z = az;
}

const cxBool cxPoint3F::IsZero() const
{
    return cxFloatIsZero(x) && cxFloatIsZero(y) && cxFloatIsZero(z);
}

cxPoint3F cxPoint3F::operator*(const cxMatrixF &mat)
{
    cxPoint3F out;
    kmVec3MultiplyMat4((kmVec3 *)&out, (kmVec3 *)this, &mat.mat4);
    return out;
}

cxPoint3F &cxPoint3F::operator*=(const cxMatrixF &mat)
{
    kmVec3MultiplyMat4((kmVec3 *)this, (kmVec3 *)this, &mat.mat4);
    return *this;
}

cxPoint3F cxPoint3F::operator+(const cxPoint3F &v) const
{
    return cxPoint3F(x+v.x, y+v.y, z+v.z);
}

cxFloat cxPoint3F::Distance(const cxPoint3F &p1,const cxPoint3F &p2)
{
    cxPoint3F v = p1 - p2;
    return v.Length();
}

cxFloat cxPoint3F::Length() const
{
    return sqrtf(x*x + y*y + z*z);
}

cxPoint3F &cxPoint3F::Normalize()
{
    cxFloat factor = 1.0f / Length();
    x *= factor;
    y *= factor;
    z *= factor;
    return *this;
}

cxPoint2F cxPoint3F::ToPoint2F() const
{
    return cxPoint2F(x, y);
}

cxPoint3F cxPoint3F::operator-(const cxPoint3F &v) const
{
    return cxPoint3F(x-v.x, y-v.y, z-v.z);
}

cxBool cxPoint3F::operator==(const cxPoint3F &v) const
{
    if(!cxFloatIsEqual(x, v.x)){
        return false;
    }
    if(!cxFloatIsEqual(y, v.y)){
        return false;
    }
    if(!cxFloatIsEqual(z, v.z)){
        return false;
    }
    return true;
}

cxBool cxPoint3F::operator!=(const cxPoint3F &v) const
{
    return !(*this == v);
}

CX_CPP_END

