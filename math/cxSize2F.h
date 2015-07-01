//
//  cxSize2F.h
//  cxEngineCore
//
//  Created by xuhua on 6/11/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#ifndef cxEngineCore_cxSize2F_h
#define cxEngineCore_cxSize2F_h

#include "cxMath.h"

CX_CPP_BEGIN

struct cxPoint2F;
struct cxJson;
struct cxSize2F
{
    cxFloat w;
    cxFloat h;
    
    cxSize2F();
    cxSize2F(cxFloat v);
    cxSize2F(const cxJson *json);
    cxSize2F(cxFloat aw,cxFloat ah);
    cxSize2F(const cxSize2F &v);
    
    cxJson *ToJson();
    cxBool IsINF() const;
    cxBool IsNAN() const;
    cxBool IsZero() const;
    
    cxBool operator==(const cxSize2F &v) const;
    cxBool operator!=(const cxSize2F &v) const;
    
    cxSize2F operator+(const cxSize2F &v) const;
    cxSize2F operator+(const cxFloat v) const;
    
    cxSize2F operator-(const cxSize2F &v) const;
    cxSize2F operator-(const cxFloat v) const;
    
    cxSize2F operator*(const cxSize2F &v) const;
    cxSize2F operator*(const cxPoint2F &v) const;
    cxSize2F operator*(const cxFloat v) const;
    
    cxSize2F operator/(const cxSize2F &v) const;
    cxSize2F operator/(const cxFloat v) const;
    
    cxSize2F &operator+=(const cxSize2F &v);
    cxSize2F &operator+=(const cxFloat v);
    
    cxSize2F &operator-=(const cxSize2F &v);
    cxSize2F &operator-=(const cxFloat v);
    
    cxSize2F &operator*=(const cxSize2F &v);
    cxSize2F &operator*=(const cxPoint2F &v);
    cxSize2F &operator*=(const cxFloat v);
    
    cxSize2F &operator/=(const cxSize2F &v);
    cxSize2F &operator/=(const cxFloat v);
    
    cxSize2F operator-() const;
};

CX_CPP_END

#endif


