//
//  cxPoint2I.cpp
//  cxEngineCore
//
//  Created by xuhua on 6/1/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//


#include <core/cxJson.h>
#include "cxPoint2F.h"
#include "cxPoint2I.h"

CX_CPP_BEGIN

cxPoint2I::cxPoint2I()
{
    x = 0;
    y = 0;
}

cxPoint2I::cxPoint2I(const cxPoint2F &v)
{
    x = v.x;
    y = v.y;
}

cxPoint2I::cxPoint2I(cxInt ax,cxInt ay)
{
    x = ax;
    y = ay;
}

cxPoint2I::cxPoint2I(const cxJson *json)
{
    CX_ASSERT(json != nullptr && json->IsObject(), "json format error");
    x = json->Get("x", x);
    y = json->Get("y", y);
}

cxJson *cxPoint2I::ToJson()
{
    cxJson *json = cxJson::Create()->Object();
    json->Set("x", x);
    json->Set("y", y);
    return json;
}

cxPoint2I cxPoint2I::LV() const
{
    return cxPoint2I(x-1,y);
}

cxPoint2I cxPoint2I::RV() const
{
    return cxPoint2I(x+1,y);
}

cxPoint2I cxPoint2I::TV() const
{
    return cxPoint2I(x,y+1);
}

cxPoint2I cxPoint2I::BV() const
{
    return cxPoint2I(x,y-1);
}

cxPoint2I cxPoint2I::LT() const
{
    return cxPoint2I(x-1,y+1);
}

cxPoint2I cxPoint2I::LB() const
{
    return cxPoint2I(x-1,y-1);
}

cxPoint2I cxPoint2I::RT() const
{
    return cxPoint2I(x+1,y+1);
}

cxPoint2I cxPoint2I::RB() const
{
    return cxPoint2I(x+1,y-1);
}

cxPoint2I::cxPoint2I(const cxPoint2I &v)
{
    x = v.x;
    y = v.y;
}

cxBool cxPoint2I::IsZero() const
{
    return x ==0 && y == 0;
}

cxFloat cxPoint2I::Length() const
{
    return sqrtf(x*x + y*y);
}

cxBool cxPoint2I::operator==(const cxPoint2I &v) const
{
    return x == v.x && y == v.y;
}

cxBool cxPoint2I::operator!=(const cxPoint2I &v) const
{
    return x != v.x || y != v.y;
}

cxPoint2I cxPoint2I::operator+(const cxPoint2I &v) const
{
    return cxPoint2I(x+v.x, y+v.y);
}

cxPoint2I cxPoint2I::operator+(const cxInt v) const
{
    return cxPoint2I(x+v, y+v);
}

cxPoint2I cxPoint2I::operator-(const cxPoint2I &v) const
{
    return cxPoint2I(x-v.x, y-v.y);
}

cxPoint2I cxPoint2I::operator-(const cxInt v) const
{
    return cxPoint2I(x-v, y-v);
}

cxPoint2I& cxPoint2I::operator+=(const cxPoint2I &v)
{
    x += v.x;
    y += v.y;
    return *this;
}

cxPoint2I& cxPoint2I::operator+=(const cxInt v)
{
    x += v;
    y += v;
    return *this;
}

cxPoint2I& cxPoint2I::operator-=(const cxPoint2I &v)
{
    x -= v.x;
    y -= v.y;
    return *this;
}

cxPoint2I& cxPoint2I::operator-=(const cxInt v)
{
    x -= v;
    y -= v;
    return *this;
}

cxPoint2I cxPoint2I::operator*(const cxInt v) const
{
    return cxPoint2I(x * v, y * v);
}

cxPoint2I cxPoint2I::operator*(const cxPoint2I &v) const
{
    return cxPoint2I(x * v.x, y * v.y);
}

cxPoint2I& cxPoint2I::operator*=(const cxInt v)
{
    x *= v;
    y *= v;
    return *this;
}

cxPoint2I& cxPoint2I::operator*=(const cxPoint2I &v)
{
    x *= v.x;
    y *= v.y;
    return *this;
}

cxPoint2I cxPoint2I::operator/(const cxInt v) const
{
    return cxPoint2I(x/v, y/v);
}

cxPoint2I cxPoint2I::operator/(const cxPoint2I &v) const
{
    return cxPoint2I(x/v.x, y/v.y);
}

cxPoint2I &cxPoint2I::operator/=(const cxInt v)
{
    x /= v;
    y /= v;
    return *this;
}

cxPoint2I &cxPoint2I::operator/=(const cxPoint2I &v)
{
    x /= v.x;
    y /= v.y;
    return *this;
}

cxPoint2I cxPoint2I::operator-() const
{
    return cxPoint2I(-x,-y);
}

cxRange2I::cxRange2I()
{
    min = 0;
    max = 0;
}

cxRange2I::cxRange2I(cxInt amin,cxInt amax)
{
    min = amin;
    max = amax;
}

cxPoint2IArray::cxPoint2IArray()
{
    
}

cxPoint2IArray::~cxPoint2IArray()
{
    
}

void cxPoint2IArray::Append(cxInt n)
{
    for(cxInt i=0;i<n;i++){
        push_back(cxPoint2F());
    }
}

void cxPoint2IArray::Append(const cxPoint2I &v)
{
    push_back(v);
}

void cxPoint2IArray::Remove(cxInt idx)
{
    erase(begin() + idx);
}

void cxPoint2IArray::Remove(cxInt idx,cxInt n)
{
    erase(begin() + idx, begin() + idx + n);
}

void cxPoint2IArray::Clear()
{
    clear();
}

const cxPoint2I *cxPoint2IArray::Buffer() const
{
    return data();
}

cxPoint2I &cxPoint2IArray::At(cxInt idx)
{
    return at(idx);
}

const cxInt cxPoint2IArray::Size() const
{
    return (cxInt)size();
}

cxBool cxPoint2IArray::IsEmpty() const
{
    return empty();
}

CX_CPP_END

