//
//  cxLineF.h
//  cxEngineCore
//
//  Created by xuhua on 6/27/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#ifndef cxEngineCore_cxLineF_h
#define cxEngineCore_cxLineF_h

#include "cxPoint2F.h"

CX_CPP_BEGIN

struct cxBox4F;
struct cxPoint2F;
struct cxLineF
{
    cxPoint2F a;
    cxPoint2F b;
    cxLineF();
    cxLineF(cxFloat x1,cxFloat y1,cxFloat x2,cxFloat y2);
    cxLineF(const cxPoint2F &aa,const cxPoint2F &ab);
    cxBool Intersect(const cxBox4F &b);
    cxBool Intersect(const cxLineF &l);
    cxPoint2F IntersectPoint(const cxLineF &l);
    void Slope(cxFloat &vk,cxFloat &vb) const;
    cxInt AtLineLR(const cxPoint2F &v) const;
};

CX_CPP_END

#endif


