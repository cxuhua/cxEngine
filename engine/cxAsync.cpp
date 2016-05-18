//
//  cxAsync.cpp
//  cxEngineCore
//
//  Created by xuhua on 7/14/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//


#include "cxAsync.h"

CX_CPP_BEGIN

CX_IMPLEMENT(cxAsync);

cxAsync::cxAsync()
{
    Forever();
    isfinished = false;
    timeout = 0;
    error = 0;
}

cxAsync *cxAsync::SetFinished(cxBool v)
{
    isfinished = v;
    return this;
}

cxAsync *cxAsync::SetTimeout(cxFloat v)
{
    timeout = v;
    return this;
}

cxAsync *cxAsync::SetError(cxInt v)
{
    error = v;
    return this;
}

void cxAsync::OnStep(cxFloat dt)
{
    cxAction::OnStep(dt);
    if(isfinished){
        Exit(true);
    }
    if(timeout > 0 && Elapsed() > timeout){
        onTimeout.Fire(this);
        Exit(true);
    }
}

cxAsync::~cxAsync()
{
    
}

CX_CPP_END

