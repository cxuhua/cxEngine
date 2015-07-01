//
//  cxRenderF.cpp
//  cxEngineCore
//
//  Created by xuhua on 6/17/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//


#include "cxRenderF.h"

CX_CPP_BEGIN

cxRenderF::cxRenderF()
{
    
}

cxRenderF::cxRenderF(const cxRenderF &v)
{
    vertices = v.vertices;
    colors = v.colors;
    coords = v.coords;
}

CX_CPP_END

