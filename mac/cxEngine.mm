//
//  cxEngine.m
//  cxEngineCore
//
//  Created by xuhua on 6/28/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#include <engine/cxEngine.h>

CX_CPP_BEGIN

void cxEngine::Exit()
{
    cxEngine::Destroy();
    exit(0);
}

const cxStr *cxEngine::TextImage(const cxStr *txt,const cxTextAttr &attr,cxSize2F &size)
{
    return nullptr;
}

CX_CPP_END
