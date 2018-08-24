//
//  cxMultiple.h
//  cxEngineCore
//  并行
//  Created by xuhua on 5/17/16.
//  Copyright © 2016 xuhua. All rights reserved.
//

#ifndef cxEngineCore_cxMultiple_h
#define cxEngineCore_cxMultiple_h

#include <core/cxArray.h>
#include "cxAction.h"

CX_CPP_BEGIN

class cxMultiple : public cxAction
{
public:
    CX_DECLARE(cxMultiple);
protected:
    explicit cxMultiple();
    virtual ~cxMultiple();
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
    cxMultiple *Append(cxAction *pav);
    cxEvent<cxMultiple,cxAction *> onAction;
};

CX_CPP_END

#endif


