//
//  cxRect4I.cpp
//  cxEngineCore
//
//  Created by xuhua on 8/28/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//


#include "cxRect4I.h"

CX_CPP_BEGIN

cxRect4I::cxRect4I()
{
    x = 0;
    y = 0;
    w = 0;
    h = 0;
}

cxRect4I::cxRect4I(cxInt ax,cxInt ay,cxInt aw,cxInt ah)
{
    x = ax;
    y = ay;
    w = aw;
    h = ah;
}

cxRect4I::cxRect4I(const cxRect4I &v)
{
    x = v.x;
    y = v.y;
    w = v.w;
    h = v.h;
}

cxBool cxRect4I::IsOK()
{
    return w > 0 && h > 0;
}

CX_CPP_END

