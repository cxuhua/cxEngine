//
//  cxRange2F.h
//  cxEngineCore
//
//  Created by xuhua on 6/30/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#ifndef cxEngineCore_cxRange2F_h
#define cxEngineCore_cxRange2F_h

#include "cxMath.h"

CX_CPP_BEGIN

struct cxRange2F
{
    cxFloat min;
    cxFloat max;
    
    cxRange2F();
    cxRange2F(cxFloat amin,cxFloat amax);
    cxRange2F(const cxRange2F &v);
    
    static cxRange2F Clamp(const cxRange2F &min,const cxRange2F &max,const cxRange2F &v);
};

CX_CPP_END

#endif


