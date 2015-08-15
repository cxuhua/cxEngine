//
//  cxSize2F.cpp
//  cxEngineCore
//
//  Created by xuhua on 6/11/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#include <core/cxJson.h>
#include "cxSize2F.h"
#include "cxPoint2F.h"

CX_CPP_BEGIN

cxSize2F::cxSize2F()
{
    w = 0;
    h = 0;
}

cxSize2F::cxSize2F(cxFloat aw,cxFloat ah)
{
    w = aw;
    h = ah;
}

cxSize2F::cxSize2F(cxFloat v)
{
    w = v;
    h = v;
}

cxSize2F::cxSize2F(const cxJson *json)
{
    CX_ASSERT(json != nullptr && json->IsObject(), "json format error");
    w = json->Get("w", w);
    h = json->Get("h", h);
}

cxJson *cxSize2F::ToJson()
{
    cxJson *json = cxJson::Create()->Object();
    json->Set("w", w);
    json->Set("h", h);
    return json;
}

cxSize2F::cxSize2F(const cxSize2F &v)
{
    w = v.w;
    h = v.h;
}

cxBool cxSize2F::IsINF() const
{
    return cxFloatIsINF(w) || cxFloatIsINF(h);
}

cxBool cxSize2F::IsNAN() const
{
    return cxFloatIsNAN(w) || cxFloatIsNAN(h);
}

cxBool cxSize2F::IsZero() const
{
    return !(w > 0 && h > 0);
}

cxBool cxSize2F::operator==(const cxSize2F &v) const
{
    return cxFloatIsEqual(w, v.w) && cxFloatIsEqual(h, v.h);
}

cxBool cxSize2F::operator!=(const cxSize2F &v) const
{
    return !cxFloatIsEqual(w, v.w) || !cxFloatIsEqual(h, v.h);
}

cxSize2F cxSize2F::operator+(const cxSize2F &v) const
{
    return cxSize2F(w + v.w, h + v.h);
}

cxSize2F cxSize2F::operator+(const cxFloat v) const
{
    return cxSize2F(w + v, h + v);
}

cxSize2F cxSize2F::operator-(const cxSize2F &v) const
{
    return cxSize2F(w - v.w, h - v.h);
}

cxSize2F cxSize2F::operator-(const cxFloat v) const
{
    return cxSize2F(w - v, h - v);
}

cxSize2F cxSize2F::operator*(const cxSize2F &v) const
{
    return cxSize2F(w * v.w, h * v.h);
}

cxSize2F cxSize2F::operator*(const cxFloat v) const
{
    return cxSize2F(w * v, h * v);
}

cxSize2F cxSize2F::operator*(const cxPoint2F &v) const
{
    return cxSize2F(w * v.x, h * v.y);
}

cxSize2F cxSize2F::operator/(const cxSize2F &v) const
{
    return cxSize2F(w/v.w, h/v.h);
}

cxSize2F cxSize2F::operator/(const cxFloat v) const
{
    return cxSize2F(w/v, h/v);
}

cxSize2F &cxSize2F::operator+=(const cxSize2F &v)
{
    w += v.w;
    h += v.h;
    return *this;
}

cxSize2F &cxSize2F::operator+=(const cxFloat v)
{
    w += v;
    h += v;
    return *this;
}

cxSize2F &cxSize2F::operator-=(const cxSize2F &v)
{
    w -= v.w;
    h -= v.h;
    return *this;
}

cxSize2F &cxSize2F::operator-=(const cxFloat v)
{
    w -= v;
    h -= v;
    return *this;
}

cxSize2F &cxSize2F::operator*=(const cxSize2F &v)
{
    w *= v.w;
    h *= v.h;
    return *this;
}

cxSize2F &cxSize2F::operator*=(const cxFloat v)
{
    w *= v;
    h *= v;
    return *this;
}

cxSize2F &cxSize2F::operator*=(const cxPoint2F &v)
{
    w *= v.x;
    h *= v.y;
    return *this;
}

cxSize2F &cxSize2F::operator/=(const cxSize2F &v)
{
    w /= v.w;
    h /= v.h;
    return *this;
}

cxSize2F &cxSize2F::operator/=(const cxFloat v)
{
    w /= v;
    h /= v;
    return *this;
}

cxSize2F cxSize2F::operator-() const
{
    return cxSize2F(-w, -h);
}

CX_CPP_END

