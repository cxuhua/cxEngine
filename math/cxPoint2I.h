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
    cxPoint2I(const cxJson *json);
    cxPoint2I(const cxPoint2F &v);
    cxPoint2I(cxInt ax,cxInt ay);
    cxPoint2I(const cxPoint2I &v);
    
    cxJson *ToJson();
    
    cxBool IsZero() const;
    cxFloat Length() const;
    
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

CX_CPP_END

#endif


