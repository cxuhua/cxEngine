//
//  cxOpenAL.cpp
//  cxEngineCore
//
//  Created by xuhua on 7/29/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#include "OpenAL.h"
#include "cxOpenAL.h"

CX_CPP_BEGIN

CX_IMPLEMENT(cxOpenAL);

cxOpenAL *cxOpenAL::instance = nullptr;

cxOpenAL::cxOpenAL()
{
    
}

cxOpenAL::~cxOpenAL()
{
    
}

cxOpenAL *cxOpenAL::Instance()
{
    return cxCore::One<cxOpenAL>(&instance);
}

CX_CPP_END

