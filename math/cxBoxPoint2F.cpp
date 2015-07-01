//
//  cxBoxPoint2F.cpp
//  cxEngineCore
//
//  Created by xuhua on 6/17/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//


#include "cxBoxPoint2F.h"

CX_CPP_BEGIN

cxBoxPoint2F::cxBoxPoint2F()
{
    
}

cxBoxPoint2F::cxBoxPoint2F(const cxBoxPoint2F &v)
{
    lt = v.lt;
    lb = v.lb;
    rt = v.rt;
    rb = v.rb;
}

CX_CPP_END

