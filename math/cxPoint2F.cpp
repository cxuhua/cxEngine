//
//  cxPoint.cpp
//  cxEngineCore
//
//  Created by xuhua on 6/1/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#include <core/cxJson.h>
#include "cxPoint2I.h"
#include "cxPoint2F.h"
#include "cxSize2F.h"

CX_CPP_BEGIN

cxPoint2F cxPoint2F::Lerp(const cxPoint2F &from,const cxPoint2F &to,cxFloat t)
{
    cxPoint2F rv;
    rv.x = cxFloatLerp(from.x, to.x, t);
    rv.y = cxFloatLerp(from.y, to.y, t);
    return rv;
}

cxPoint2F cxPoint2F::Clamp(const cxPoint2F &min,const cxPoint2F &max,const cxPoint2F &v)
{
    cxPoint2F rv;
    rv.x = cxFloatClamp(v.x, min.x, max.x);
    rv.y = cxFloatClamp(v.y, min.y, max.y);
    return rv;
}

cxFloat cxPoint2F::Distance(const cxPoint2F &p1,const cxPoint2F &p2)
{
    cxPoint2F v = p1 - p2;
    return v.Length();
}

cxPoint2F cxPoint2F::Center(const cxPoint2F &p1,const cxPoint2F &p2)
{
    return (p1 + p2)/2.0f;
}

cxPoint2F::cxPoint2F(const cxPoint2I &v)
{
    x = v.x;
    y = v.y;
}

cxPoint2F::cxPoint2F()
{
    x = 0;
    y = 0;
}

cxPoint2F::cxPoint2F(cxFloat a)
{
    x = a;
    y = a;
}

cxPoint2F::cxPoint2F(const cxJson *json)
{
    CX_ASSERT(json != nullptr && json->IsObject(), "json format error");
    x = json->Get("x", x);
    y = json->Get("y", y);
}

cxJson *cxPoint2F::ToJson()
{
    cxJson *json = cxJson::Create()->Object();
    json->Set("x", x);
    json->Set("y", y);
    return json;
}

cxPoint2F::cxPoint2F(cxFloat ax,cxFloat ay)
{
    x = ax;
    y = ay;
}

cxPoint2F::cxPoint2F(const cxPoint2F &v)
{
    x = v.x;
    y = v.y;
}

cxPoint2F &cxPoint2F::Normalize()
{
    cxFloat factor = 1.0f / Length();
    x *= factor;
    y *= factor;
    return *this;
}

const cxPoint2I cxPoint2F::ToInt() const
{
    return cxPoint2I(x+cxEqualFloat, y+cxEqualFloat);
}

const cxPoint2F cxPoint2F::ToInt(const cxPoint2F &add) const
{
    cxPoint2I ip = ToInt();
    return cxPoint2F(ip) + add;
}

cxBool cxPoint2F::IsINF() const
{
    return cxFloatIsINF(x) || cxFloatIsINF(y);
}
cxBool cxPoint2F::IsNAN() const
{
    return cxFloatIsNAN(x) || cxFloatIsNAN(y);
}

cxBool cxPoint2F::IsZero() const
{
    return cxFloatIsEqual(x, 0) && cxFloatIsEqual(y, 0);
}

cxFloat cxPoint2F::Length() const
{
    return sqrtf(x*x + y*y);
}

cxFloat cxPoint2F::Distance(const cxPoint2F &d) const
{
    return Distance(*this, d);
}

cxFloat cxPoint2F::Angle(const cxPoint2F &d) const
{
    cxPoint2F sub = d - *this;
    return atan2f(sub.y, sub.x);
}

cxFloat cxPoint2F::Angle() const
{
    return atan2f(y, x);
}

cxFloat cxPoint2F::operator[] (cxInt index) const
{
    CX_ASSERT(index >= 0 && index < 2, "index out bound");
    cxFloat *ptr = (cxFloat *)this;
    return ptr[index];
}

cxBool cxPoint2F::operator==(const cxPoint2F &v) const
{
    return cxFloatIsEqual(x,v.x) && cxFloatIsEqual(y,v.y);
}

cxBool cxPoint2F::operator!=(const cxPoint2F &v) const
{
    return !cxFloatIsEqual(x,v.x) || !cxFloatIsEqual(y,v.y);
}

cxPoint2F cxPoint2F::operator+(const cxPoint2F &v) const
{
    return cxPoint2F(x+v.x, y+v.y);
}

cxPoint2F cxPoint2F::operator+(const cxSize2F &v) const
{
    return cxPoint2F(x+v.w, y+v.h);
}

cxPoint2F cxPoint2F::operator+(const cxFloat v) const
{
    return cxPoint2F(x+v, y+v);
}

cxPoint2F cxPoint2F::operator-(const cxPoint2F &v) const
{
    return cxPoint2F(x-v.x, y-v.y);
}

cxPoint2F cxPoint2F::operator-(const cxSize2F &v) const
{
    return cxPoint2F(x-v.w, y-v.h);
}

cxPoint2F cxPoint2F::operator-(const cxFloat v) const
{
    return cxPoint2F(x-v, y-v);
}

cxPoint2F& cxPoint2F::operator+=(const cxPoint2F &v)
{
    x += v.x;
    y += v.y;
    return *this;
}

cxPoint2F& cxPoint2F::operator+=(const cxFloat v)
{
    x += v;
    y += v;
    return *this;
}

cxPoint2F &cxPoint2F::operator+=(const cxSize2F &v)
{
    x += v.w;
    y += v.h;
    return *this;
}

cxPoint2F& cxPoint2F::operator-=(const cxPoint2F &v)
{
    x -= v.x;
    y -= v.y;
    return *this;
}

cxPoint2F& cxPoint2F::operator-=(const cxFloat v)
{
    x -= v;
    y -= v;
    return *this;
}

cxPoint2F cxPoint2F::operator*(const cxFloat v) const
{
    return cxPoint2F(x * v, y * v);
}

cxPoint2F cxPoint2F::operator*(const cxSize2F &v) const
{
    return cxPoint2F(x * v.w, y * v.h);
}

cxPoint2F cxPoint2F::operator*(const cxPoint2F &v) const
{
    return cxPoint2F(x * v.x, y * v.y);
}

cxPoint2F& cxPoint2F::operator*=(const cxFloat v)
{
    x *= v;
    y *= v;
    return *this;
}

cxPoint2F& cxPoint2F::operator*=(const cxPoint2F &v)
{
    x *= v.x;
    y *= v.y;
    return *this;
}

cxPoint2F cxPoint2F::operator/(const cxFloat v) const
{
    return cxPoint2F(x/v, y/v);
}

cxPoint2F cxPoint2F::operator/(const cxPoint2F &v) const
{
    return cxPoint2F(x/v.x, y/v.y);
}

cxPoint2F cxPoint2F::operator/(const cxSize2F &v) const
{
    return cxPoint2F(x/v.w, y/v.h);
}

cxPoint2F &cxPoint2F::operator/=(const cxFloat v)
{
    x /= v;
    y /= v;
    return *this;
}

cxPoint2F &cxPoint2F::operator/=(const cxPoint2F &v)
{
    x /= v.x;
    y /= v.y;
    return *this;
}

cxPoint2F cxPoint2F::operator-() const
{
    return cxPoint2F(-x,-y);
}

cxPoint2FRange::cxPoint2FRange()
{
    v = 0.0f;
    r = 0.0f;
}

cxPoint2FRange::cxPoint2FRange(const cxPoint2FRange &av)
{
    v = av.v;
    r = av.r;
}

cxPoint2FRange::cxPoint2FRange(const cxPoint2F &av,const cxPoint2F &ar)
{
    v = av;
    r = ar;
}

const cxPoint2F cxPoint2FRange::ToValue() const
{
    cxPoint2F pos;
    pos.x = v.x + r.x * CX_RAND_11f();
    pos.y = v.y + r.y * CX_RAND_11f();
    return pos;
}

cxPoint2FArray::cxPoint2FArray()
{
    
}

cxPoint2FArray::~cxPoint2FArray()
{
    
}

void cxPoint2FArray::Append(cxInt n)
{
    for(cxInt i=0;i<n;i++){
        push_back(cxPoint2F());
    }
}

cxFloat cxPoint2FArray::Distance(const cxPoint2F &start)
{
    cxFloat d = 0;
    cxPoint2F s = start;
    for(iterator it=begin();it != end();it++){
        d += s.Distance(*it);
        s = *it;
    }
    return d;
}

void cxPoint2FArray::Append(const cxPoint2F &v)
{
    push_back(v);
}

void cxPoint2FArray::Append(const cxPoint2FArray &v)
{
    cxInt n = v.Size();
    for(cxInt i=0;i<n;i++){
        push_back(v.At(i));
    }
}

void cxPoint2FArray::Remove(cxInt idx)
{
    erase(begin() + idx);
}

void cxPoint2FArray::Remove(cxInt idx,cxInt n)
{
    erase(begin() + idx, begin() + idx + n);
}

void cxPoint2FArray::Clear()
{
    clear();
}

const cxPoint2F *cxPoint2FArray::Buffer() const
{
    return data();
}

const cxPoint2F &cxPoint2FArray::At(cxInt idx) const
{
    return at(idx);
}

cxPoint2F &cxPoint2FArray::At(cxInt idx)
{
    return at(idx);
}

const cxPoint2F& cxPoint2FArray::operator [](cxInt idx) const
{
    return at(idx);
}

cxPoint2F& cxPoint2FArray::operator [](cxInt idx)
{
    return at(idx);
}

const cxInt cxPoint2FArray::Size() const
{
    return (cxInt)size();
}

cxBool cxPoint2FArray::IsEmpty() const
{
    return empty();
}

cxRange2F::cxRange2F()
{
    
}

cxRange2F::cxRange2F(cxFloat amin,cxFloat amax)
{
    min = amin;
    max = amax;
}

cxRange2F::cxRange2F(const cxRange2F &v)
{
    min = v.min;
    max = v.max;
}

cxBool cxRange2F::At(cxFloat v)
{
    if(cxFloatIsEqual(v, min) || cxFloatIsEqual(v, max)){
        return true;
    }
    if(v > min && v < max){
        return true;
    }
    return false;
}

cxRange2F cxRange2F::Clamp(const cxRange2F &min,const cxRange2F &max,const cxRange2F &v)
{
    cxRange2F rv;
    rv.min = cxFloatClamp(v.min, min.min, max.min);
    rv.max = cxFloatClamp(v.max, min.max, max.max);
    return rv;
}

cxBool cxRange2F::operator==(const cxRange2F &v) const
{
    return cxFloatIsEqual(min, v.min) && cxFloatIsEqual(max, v.max);
}

cxBool cxRange2F::operator!=(const cxRange2F &v) const
{
    return !cxFloatIsEqual(min, v.min) || !cxFloatIsEqual(max, v.max);
}

CX_CPP_END

