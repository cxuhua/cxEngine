//
//  cxRange2F.cpp
//  cxEngineCore
//
//  Created by xuhua on 6/30/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//


#include "cxRange2F.h"

CX_CPP_BEGIN

cxRange2F::cxRange2F()
{
    
}

cxRange2F::cxRange2F(cxFloat amin,cxFloat amax)
{
    min = amin;
    max = amax;
}

cxRange2F::cxRange2F(const cxRange2F &v)
{
    min = v.min;
    max = v.max;
}

cxRange2F cxRange2F::Clamp(const cxRange2F &min,const cxRange2F &max,const cxRange2F &v)
{
    cxRange2F rv;
    rv.min = cxFloatClamp(v.min, min.min, max.min);
    rv.max = cxFloatClamp(v.max, min.max, max.max);
    return rv;
}

CX_CPP_END

