//
//  cxBox4F.h
//  cxEngineCore
//
//  Created by xuhua on 6/11/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#ifndef cxEngineCore_cxBox4F_h
#define cxEngineCore_cxBox4F_h

#include "cxPoint2F.h"

CX_CPP_BEGIN

class cxJson;
struct cxPoint2F;
struct cxPoint3F;
struct cxRect4F;
struct cxBoxPoint3F;
struct cxSize2F;
struct cxBox4F
{
    cxFloat l;
    cxFloat r;
    cxFloat t;
    cxFloat b;
    cxBox4F();
    cxBox4F(const cxBox4F &v);
    cxBox4F(const cxJson *json);
    cxBox4F(cxFloat a);
    cxBox4F(cxFloat al,cxFloat ar,cxFloat at,cxFloat ab);
    cxBox4F(const cxRect4F &rect);
    cxBool Intersect(const cxBoxPoint3F &v);
    cxBool Contain(const cxPoint2F &p);
    cxBool Contain(const cxPoint3F &p);
    cxBool Contain(const cxBox4F &p);
    cxBool Contain(const cxBoxPoint3F &v);
    cxRect4F ToRect4F() const;
    cxPoint2F Center() const;
    cxJson *ToJson();
    
    cxPoint2F LT() const;
    cxPoint2F RT() const;
    cxPoint2F LB() const;
    cxPoint2F RB() const;
    
    cxFloat X() const;
    cxFloat Y() const;
    cxFloat W() const;
    cxFloat H() const;
    
    cxSize2F Size() const;
    
    cxBool operator==(const cxBox4F &v) const;
    cxBool operator!=(const cxBox4F &v) const;
    
    cxBox4F operator+(const cxPoint2F &v) const;
    
    cxBox4F &operator+=(const cxPoint2F &v);
    
    cxBox4F operator*(const cxFloat v);
    cxBox4F operator*(const cxPoint2F &v) const;
    cxBox4F operator*(const cxPoint2F &v);
    
    cxBox4F operator/(const cxFloat v);
    cxBox4F operator/(const cxPoint2F &v) const;
    cxBox4F operator/(const cxPoint2F &v);
};

CX_CPP_END

#endif


