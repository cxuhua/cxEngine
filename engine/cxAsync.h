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
    cxBool isfinished;
    cxFloat timeout;
    cxInt error;
public:
    cxAsync *SetTimeout(cxFloat v);
    cxAsync *SetFinished(cxBool v);
    cxAsync *SetError(cxInt v);
public:
    cxEvent<cxAsync> onTimeout;
};

CX_CPP_END

#endif


