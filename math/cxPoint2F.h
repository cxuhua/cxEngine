//
//  cxPoint.h
//  cxEngineCore
//
//  Created by xuhua on 6/1/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#ifndef cxEngineCore_cxPoint2F_h
#define cxEngineCore_cxPoint2F_h

#include <vector>
#include "cxMath.h"

CX_CPP_BEGIN

struct cxJson;
struct cxSize2F;
struct cxPoint2I;
struct cxPoint2F
{
    cxFloat x;
    cxFloat y;
    
    cxPoint2F();
    cxPoint2F(const cxJson *json);
    cxPoint2F(cxFloat ax,cxFloat ay);
    cxPoint2F(cxFloat a);
    cxPoint2F(const cxPoint2I &v);
    cxPoint2F(const cxPoint2F &v);

    cxJson *ToJson();
    cxBool IsINF() const;
    cxBool IsNAN() const;
    cxBool IsZero() const;
    cxFloat Length() const;
    cxFloat Angle() const;
    cxFloat Angle(const cxPoint2F &d) const;
    cxFloat Distance(const cxPoint2F &d) const;
    cxPoint2F &Normalize();
    const cxPoint2I ToInt() const;
    const cxPoint2F ToInt(const cxPoint2F &add) const;
    
    static cxPoint2F Center(const cxPoint2F &p1,const cxPoint2F &p2);
    static cxPoint2F Lerp(const cxPoint2F &from,const cxPoint2F &to,cxFloat t);
    static cxPoint2F Clamp(const cxPoint2F &min,const cxPoint2F &max,const cxPoint2F &v);
    static cxFloat Distance(const cxPoint2F &p1,const cxPoint2F &p2);
    
    cxBool operator==(const cxPoint2F &v) const;
    cxBool operator!=(const cxPoint2F &v) const;
    
    cxPoint2F operator+(const cxPoint2F &v) const;
    cxPoint2F operator+(const cxSize2F &v) const;
    cxPoint2F operator+(const cxFloat v) const;

    cxPoint2F operator-(const cxPoint2F &v) const;
    cxPoint2F operator-(const cxSize2F &v) const;
    cxPoint2F operator-(const cxFloat v) const;

    cxPoint2F operator*(const cxPoint2F &v) const;
    cxPoint2F operator*(const cxSize2F &v) const;
    cxPoint2F operator*(const cxFloat v) const;
    
    cxPoint2F operator/(const cxPoint2F &v) const;
    cxPoint2F operator/(const cxSize2F &v) const;
    cxPoint2F operator/(const cxFloat v) const;
    
    cxPoint2F &operator+=(const cxPoint2F &v);
    cxPoint2F &operator+=(const cxSize2F &v);
    cxPoint2F &operator+=(const cxFloat v);
    
    cxPoint2F &operator-=(const cxPoint2F &v);
    cxPoint2F &operator-=(const cxFloat v);

    cxPoint2F &operator*=(const cxPoint2F &v);
    cxPoint2F &operator*=(const cxFloat v);
    
    cxPoint2F &operator/=(const cxPoint2F &v);
    cxPoint2F &operator/=(const cxFloat v);
    
    cxPoint2F operator-() const;
};

struct cxPoint2FRange
{
    cxPoint2F v;
    cxPoint2F r;
    cxPoint2FRange();
    cxPoint2FRange(const cxPoint2FRange &av);
    cxPoint2FRange(const cxPoint2F &av,const cxPoint2F &ar);
    const cxPoint2F ToValue() const;
};

class cxPoint2FArray : private std::vector<cxPoint2F>
{
public:
    explicit cxPoint2FArray();
    virtual ~cxPoint2FArray();
public:
    void Clear();
    void Append(cxInt n);
    void Append(const cxPoint2F &v);
    void Remove(cxInt idx);
    void Remove(cxInt idx,cxInt n);
    cxFloat Distance(const cxPoint2F &start);
    const cxPoint2F *Buffer()const ;
    cxPoint2F &At(cxInt idx);
    const cxInt Size() const;
    cxBool IsEmpty() const;
    cxPoint2FArray &operator=(const cxPoint2FArray &v) const;
};

struct cxRange2F
{
    cxFloat min;
    cxFloat max;
    
    cxRange2F();
    cxRange2F(cxFloat amin,cxFloat amax);
    cxRange2F(const cxRange2F &v);
    
    static cxRange2F Clamp(const cxRange2F &min,const cxRange2F &max,const cxRange2F &v);
};

CX_CPP_END

#endif


