//
//  cxRect4F.h
//  cxEngineCore
//
//  Created by xuhua on 6/11/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#ifndef cxEngineCore_cxRect4F_h
#define cxEngineCore_cxRect4F_h

#include "cxSize2F.h"
#include "cxPoint2F.h"

CX_CPP_BEGIN

struct cxJson;
struct cxBox4F;
struct cxRect4F
{
    cxFloat x;
    cxFloat y;
    cxFloat w;
    cxFloat h;
    cxRect4F();
    cxRect4F(const cxRect4F &v);
    cxRect4F(const cxJson *json);
    cxRect4F(cxFloat ax,cxFloat ay,cxFloat aw,cxFloat ah);
    cxRect4F(const cxPoint2F &acpos,const cxSize2F &asize);
    cxRect4F(const cxBox4F &box);
    cxBool Contain(const cxPoint2F &p);
    cxBox4F ToBox4F() const;
    cxJson *ToJson();
    
    cxBool operator==(const cxRect4F &v) const;
    cxBool operator!=(const cxRect4F &v) const;
};

CX_CPP_END

#endif


