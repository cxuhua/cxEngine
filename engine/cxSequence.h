//
//  cxSequence.h
//  cxEngineCore
//
//  Created by xuhua on 7/5/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#ifndef cxEngineCore_cxSequence_h
#define cxEngineCore_cxSequence_h

#include <core/cxArray.h>
#include "cxAction.h"

CX_CPP_BEGIN

class cxSequence : public cxAction
{
public:
    CX_DECLARE(cxSequence);
protected:
    explicit cxSequence();
    virtual ~cxSequence();
protected:
    void OnStep(cxFloat dt);
    void OnInit();
private:
    cxInt index;
    cxArray *actions;
public:
    const cxInt Size() const;
    const cxInt Index() const;
    cxSequence *Append(cxAction *pav);
    cxEvent<cxSequence> onAction;
};

CX_CPP_END

#endif


