//
//  cxPoint2I.h
//  cxEngineCore
//
//  Created by xuhua on 6/1/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#ifndef cxEngineCore_cxPoint2I_h
#define cxEngineCore_cxPoint2I_h

#include "cxMath.h"

CX_CPP_BEGIN

struct cxJson;
struct cxPoint2F;
struct cxPoint2I
{
    cxInt x;
    cxInt y;
    
    cxPoint2I();
    cxPoint2I(cxInt v);
    cxPoint2I(const cxJson *json);
    cxPoint2I(const cxPoint2F &v);
    cxPoint2I(cxInt ax,cxInt ay);
    cxPoint2I(const cxPoint2I &v);
    
    cxPoint2I LV() const;
    cxPoint2I RV() const;
    cxPoint2I TV() const;
    cxPoint2I BV() const;
    cxPoint2I LT() const;
    cxPoint2I LB() const;
    cxPoint2I RT() const;
    cxPoint2I RB() const;
    
    cxFloat Slope(const cxPoint2I &p) const;
    
    cxJson *ToJson();
    const cxPoint2F ToFloat() const;
    const cxPoint2F ToFloat(const cxFloat &add) const;
    const cxFloat Angle() const;
    const cxFloat Angle(const cxPoint2I &p) const;
    
    cxBool IsZero() const;
    cxFloat Length() const;
    
    cxInt operator[](cxInt index) const;
    
    cxBool operator==(const cxPoint2I &v) const;
    cxBool operator!=(const cxPoint2I &v) const;
    
    cxPoint2I operator+(const cxPoint2I &v) const;
    cxPoint2I operator+(const cxInt v) const;
    
    cxPoint2I operator-(const cxPoint2I &v) const;
    cxPoint2I operator-(const cxInt v) const;
    
    cxPoint2I operator*(const cxPoint2I &v) const;
    cxPoint2I operator*(const cxInt v) const;
    
    cxPoint2I operator/(const cxPoint2I &v) const;
    cxPoint2I operator/(const cxInt v) const;
    
    cxPoint2I &operator+=(const cxPoint2I &v);
    cxPoint2I &operator+=(const cxInt v);
    
    cxPoint2I &operator-=(const cxPoint2I &v);
    cxPoint2I &operator-=(const cxInt v);
    
    cxPoint2I &operator*=(const cxPoint2I &v);
    cxPoint2I &operator*=(const cxInt v);

    cxPoint2I &operator/=(const cxPoint2I &v);
    cxPoint2I &operator/=(const cxInt v);
    
    cxPoint2I operator-() const;
};

struct cxRange2I
{
    cxInt min;
    cxInt max;
    cxRange2I();
    cxRange2I(cxInt amin,cxInt amax);
};

class cxPoint2IArray : private std::vector<cxPoint2I>
{
public:
    explicit cxPoint2IArray();
    virtual ~cxPoint2IArray();
public:
    void Clear();
    void Append(cxInt n);
    void Append(const cxPoint2I &v);
    void Append(const cxPoint2IArray &v);
    void Remove(cxInt idx);
    void Remove(cxInt idx,cxInt n);
    const cxPoint2I *Buffer()const ;
    cxPoint2I &At(cxInt idx);
    const cxPoint2I &At(cxInt idx) const;
    const cxInt Size() const;
    cxBool IsEmpty() const;
    //combine angle equ point
    cxPoint2IArray Combine(cxFloat equa = cxEqualFloat) const;
public:
    const cxPoint2I& operator [](cxInt idx) const;
    cxPoint2I& operator [](cxInt idx);
};

CX_CPP_END

#endif


