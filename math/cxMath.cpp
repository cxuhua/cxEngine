//
//  cxMath.cpp
//  cxEngineCore
//
//  Created by xuhua on 6/1/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//


#include "cxMath.h"

CX_CPP_BEGIN

cxFloatRange::cxFloatRange()
{
    v = 0;
    r = 0;
}

cxFloatRange::cxFloatRange(const cxFloatRange &av)
{
    v = av.v;
    r = av.r;
}

cxFloatRange::cxFloatRange(cxFloat av,cxFloat ar)
{
    v = av;
    r = ar;
}

const cxFloat cxFloatRange::ToValue() const
{
    return v + r * CX_RAND_11f();
}

CX_CPP_END

