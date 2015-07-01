//
//  cxRenderF.h
//  cxEngineCore
//
//  Created by xuhua on 6/17/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#ifndef cxEngineCore_cxRenderF_h
#define cxEngineCore_cxRenderF_h

#include "cxPoint3F.h"
#include "cxColor4F.h"
#include "cxBoxCoord2F.h"
#include "cxMatrixF.h"

CX_CPP_BEGIN

struct cxRenderF
{
    cxPoint3F vertices;
    cxColor4F colors;
    cxCoord2F coords;
    cxRenderF();
    cxRenderF(const cxRenderF &v);
};

CX_CPP_END

#endif


