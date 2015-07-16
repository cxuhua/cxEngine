//
//  cxBoxCoord2F.h
//  cxEngineCore
//
//  Created by xuhua on 6/17/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#ifndef cxEngineCore_cxBoxCoord2F_h
#define cxEngineCore_cxBoxCoord2F_h

#include "cxMath.h"

CX_CPP_BEGIN

struct cxCoord2F
{
    cxFloat u;
    cxFloat v;
    cxCoord2F();
    cxCoord2F(const cxCoord2F &av);
    cxCoord2F(cxFloat au,cxFloat av);
};

struct cxBoxCoord2F
{
    cxCoord2F lt;
    cxCoord2F lb;
    cxCoord2F rt;
    cxCoord2F rb;
    static cxBoxCoord2F Default;
    cxBoxCoord2F();
    cxBoxCoord2F(const cxBoxCoord2F &v);
    cxBoxCoord2F(const cxCoord2F &alt,const cxCoord2F &alb,const cxCoord2F &art,const cxCoord2F &arb);
    const cxFloat X() const;
    const cxFloat Y() const;
    const cxFloat W() const;
    const cxFloat H() const;
};

CX_CPP_END

#endif


