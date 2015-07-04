//
//  cxColor4F.h
//  cxEngineCore
//
//  Created by xuhua on 6/11/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#ifndef cxEngineCore_cxColor4F_h
#define cxEngineCore_cxColor4F_h

#include "cxMath.h"

CX_CPP_BEGIN

struct cxColor4F;
struct cxJson;

struct cxColor4B
{
    cxByte r;
    cxByte g;
    cxByte b;
    cxByte a;
    cxColor4B();
    cxColor4B(const cxColor4F &v);
};

struct cxColor4F
{
    cxFloat r;
    cxFloat g;
    cxFloat b;
    cxFloat a;
    cxColor4F();
    cxColor4F(const cxColor4F &v);
    cxColor4F(const cxJson *json);
    cxColor4F(cxUInt32 color);
    cxColor4F(cxFloat ar,cxFloat ag,cxFloat ab,cxFloat aa);
    cxColor4F(cxFloat ar,cxFloat ag,cxFloat ab);
    void SetAlpha(cxFloat aa);
    void SetRGB(cxFloat ar,cxFloat ag,cxFloat ab);
    cxColor4B ToColor4B();
    cxUInt32 ToInt() const;
    cxJson *ToJson();
    
    cxBool operator==(const cxColor4F &v) const;
    cxBool operator!=(const cxColor4F &v) const;
    
    cxColor4F operator+(const cxColor4F &v) const;
    cxColor4F operator+(const cxFloat v) const;
    
    cxColor4F operator-(const cxColor4F &v) const;
    cxColor4F operator-(const cxFloat v) const;
    
    cxColor4F operator*(const cxColor4F &v) const;
    cxColor4F operator*(const cxFloat v) const;
    
    cxColor4F operator/(const cxColor4F &v) const;
    cxColor4F operator/(const cxFloat v) const;
    
    cxColor4F &operator+=(const cxColor4F &v);
    cxColor4F &operator+=(const cxFloat v);
    
    cxColor4F &operator-=(const cxColor4F &v);
    cxColor4F &operator-=(const cxFloat v);
    
    cxColor4F &operator*=(const cxColor4F &v);
    cxColor4F &operator*=(const cxFloat v);
    
    cxColor4F &operator/=(const cxColor4F &v);
    cxColor4F &operator/=(const cxFloat v);
    
    cxColor4F operator-() const;
    
    static const cxColor4F WHITE;
    static const cxColor4F YELLOW;
    static const cxColor4F BLUE;
    static const cxColor4F GREEN;
    static const cxColor4F RED;
    static const cxColor4F MAGENTA;
    static const cxColor4F BLACK;
    static const cxColor4F ORANGE;
    static const cxColor4F GRAY;
    static const cxColor4F PURPLE;
};

struct cxColor4FRange
{
    cxColor4F v;
    cxColor4F r;
    cxColor4FRange();
    cxColor4FRange(const cxColor4FRange &av);
    cxColor4FRange(const cxColor4F &av);
    cxColor4FRange(const cxColor4F &av,const cxColor4F &ar);
    const cxColor4F ToValue() const;
};

CX_CPP_END

#endif


