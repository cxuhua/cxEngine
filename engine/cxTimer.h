//
//  cxTimer.h
//  cxEngineCore
//
//  Created by xuhua on 7/2/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#ifndef cxEngineCore_cxTimer_h
#define cxEngineCore_cxTimer_h

#include "cxAction.h"

CX_CPP_BEGIN

class cxTimer : public cxAction
{
public:
    CX_DECLARE(cxTimer);
protected:
    explicit cxTimer();
    virtual ~cxTimer();
protected:
    void OnStep(cxFloat dt);
    void OnInit();
    void OnStop();
private:
    cxUInt repeat;
public:
    cxEvent<cxTimer> onArrive;
    static cxTimer *Create(cxInt repeat,cxFloat time);
    static cxTimer *Forever(cxFloat time);
};

CX_CPP_END

#endif


