//
//  cxAsync.h
//  cxEngineCore
//
//  Created by xuhua on 7/14/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#ifndef cxEngineCore_cxAsync_h
#define cxEngineCore_cxAsync_h

#include "cxAction.h"

CX_CPP_BEGIN

class cxAsync : public cxAction
{
public:
    CX_DECLARE(cxAsync);
protected:
    explicit cxAsync();
    virtual ~cxAsync();
protected:
    void OnStep(cxFloat dt);
private:
    cxInt64 unix;
    cxBool isfinished;
    cxFloat timeout;
    cxInt error;
public:
    cxInt ErrorCode();
    virtual const cxStr *Error();
    cxAsync *SetTimeout(cxFloat v);
    cxAsync *SetFinished(cxBool v);
    cxAsync *SetError(cxInt v);
protected:
    virtual void OnUnix(cxInt64 unix);
public:
    cxEvent<cxAsync> onTimeout;
    cxEvent<cxAsync,cxInt64> onUnix;
};

CX_CPP_END

#endif


