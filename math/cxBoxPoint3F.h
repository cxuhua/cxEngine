//
//  cxBoxPoint3F.h
//  cxEngineCore
//
//  Created by xuhua on 6/17/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#ifndef cxEngineCore_cxBoxPoint3F_h
#define cxEngineCore_cxBoxPoint3F_h

#include "cxPoint3F.h"

CX_CPP_BEGIN

struct cxMatrixF;
struct cxBox4F;
struct cxSize2F;
struct cxRect4F;
struct cxBoxPoint3F
{
    cxPoint3F lt;
    cxPoint3F lb;
    cxPoint3F rt;
    cxPoint3F rb;
    cxBoxPoint3F();
    cxBoxPoint3F(const cxBoxPoint3F &v);
    cxBoxPoint3F(const cxPoint3F &alt,const cxPoint3F &alb,const cxPoint3F &art,const cxPoint3F &arb);
    cxBoxPoint3F(const cxBox4F &v);
    void Set(cxFloat l,cxFloat r,cxFloat t,cxFloat b);
    const cxBoxPoint3F ToLoop() const;
    const cxBox4F ToBox4F() const;
    const cxBox4F ToMaxBox() const;
    const cxSize2F Size() const;
    cxBoxPoint3F operator*(const cxMatrixF &mat);
    cxBoxPoint3F &operator*=(const cxMatrixF &mat);
};

CX_CPP_END

#endif


