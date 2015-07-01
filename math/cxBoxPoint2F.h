//
//  cxBoxPoint2F.h
//  cxEngineCore
//
//  Created by xuhua on 6/17/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#ifndef cxEngineCore_cxBoxPoint2F_h
#define cxEngineCore_cxBoxPoint2F_h

#include "cxPoint2F.h"

CX_CPP_BEGIN

struct cxBoxPoint2F
{
    cxPoint2F lt;
    cxPoint2F lb;
    cxPoint2F rt;
    cxPoint2F rb;
    cxBoxPoint2F();
    cxBoxPoint2F(const cxBoxPoint2F &v);
};

CX_CPP_END

#endif


