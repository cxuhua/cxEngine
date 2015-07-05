//
//  cxCustom.h
//  cxEngineCore
//
//  Created by xuhua on 7/5/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#ifndef cxEngineCore_cxCustom_h
#define cxEngineCore_cxCustom_h

#include "cxAction.h"

CX_CPP_BEGIN

class cxCustom : public cxAction
{
public:
    CX_DECLARE(cxCustom);
protected:
    explicit cxCustom();
    virtual ~cxCustom();
protected:
    void OnStep(cxFloat dt);
private:
public:
    cxEvent<cxCustom,cxArgs> onStep;
    static cxCustom *Create(cxFloat time);
};

CX_CPP_END

#endif


