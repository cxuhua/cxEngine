//
//  cxSequence.h
//  cxEngineCore
//  串行
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
    void OnInit();
    void OnExit();
private:
    cxInt index;
    cxArray *actions;
    void actionExit(cxAction *pav);
public:
    void Exit(cxBool v);
    void Stop();
    const cxInt Size() const;
    const cxInt Index() const;
    cxSequence *Append(cxAction *pav);
    cxEvent<cxSequence,cxAction *> onAction;
};

CX_CPP_END

#endif


