//
//  cxTimer.cpp
//  cxEngineCore
//
//  Created by xuhua on 7/2/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//


#include "cxTimer.h"

CX_CPP_BEGIN

CX_IMPLEMENT(cxTimer);

cxTimer::cxTimer()
{
    
}

cxTimer::~cxTimer()
{
    
}

void cxTimer::OnStep(cxFloat dt)
{
    
}

void cxTimer::OnInit()
{
    
}

void cxTimer::OnStop()
{
    onArrive.Fire(this);
}

cxTimer *cxTimer::Forever(cxFloat time)
{
    cxTimer *rv = cxTimer::Create();
    rv->SetRepeat(INT_MAX);
    rv->SetTime(time);
    return rv;
}

cxTimer *cxTimer::Create(cxInt repeat,cxFloat time)
{
    cxTimer *rv = cxTimer::Create();
    rv->SetRepeat(repeat);
    rv->SetTime(time);
    return rv;
}

CX_CPP_END

