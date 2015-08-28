//
//  cxRect4I.h
//  cxEngineCore
//
//  Created by xuhua on 8/28/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#ifndef cxEngineCore_cxRect4I_h
#define cxEngineCore_cxRect4I_h

#include "cxMath.h"

CX_CPP_BEGIN

struct cxRect4I
{
    cxInt x;
    cxInt y;
    cxInt w;
    cxInt h;
    cxRect4I();
    cxRect4I(cxInt ax,cxInt ay,cxInt aw,cxInt ah);
    cxRect4I(const cxRect4I &v);
    cxBool IsOK();
};

CX_CPP_END

#endif


