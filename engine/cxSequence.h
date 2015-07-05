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
#include "cxTimeLine.h"

CX_CPP_BEGIN

class cxSequence : public cxTimeLine
{
public:
    CX_DECLARE(cxSequence);
protected:
    explicit cxSequence();
    virtual ~cxSequence();
protected:
    void OnTime(const cxTimePoint *tp);
public:
    cxSequence *Append(cxAction *pav);
};

CX_CPP_END

#endif


