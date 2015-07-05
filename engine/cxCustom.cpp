//
//  cxCustom.cpp
//  cxEngineCore
//
//  Created by xuhua on 7/5/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//


#include "cxCustom.h"

CX_CPP_BEGIN

CX_IMPLEMENT(cxCustom);

cxCustom::cxCustom()
{
    
}

cxCustom::~cxCustom()
{
    
}

void cxCustom::OnStep(cxFloat dt)
{
    onStep.Fire(this,dt);
}

cxCustom *cxCustom::Create(cxFloat time)
{
    cxCustom *rv = cxCustom::Create();
    rv->SetTime(time);
    return rv;
}

CX_CPP_END

