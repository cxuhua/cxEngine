//
//  cxAsync.cpp
//  cxEngineCore
//
//  Created by xuhua on 7/14/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#include <core/cxUtil.h>
#include "cxAsync.h"

CX_CPP_BEGIN

CX_IMPLEMENT(cxAsync);

cxAsync::cxAsync()
{
    Forever();
    unixTime = 0;
    isfinished = false;
    timeout = 0;
    error = 0;
}

cxInt cxAsync::ErrorCode()
{
    return error;
}

const cxStr *cxAsync::Error()
{
    return nullptr;
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

void cxAsync::OnUnixTime(cxInt64 unixTime)
{
    onUnixTime.Fire(this, unixTime);
}

void cxAsync::OnStep(cxFloat dt)
{
    cxInt64 now = cxUtil::Timestamp();
    if(now != unixTime){
        OnUnixTime(now);
        unixTime = now;
    }
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

