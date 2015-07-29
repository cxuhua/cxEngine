//
//  cxOpenAL.h
//  cxEngineCore
//
//  Created by xuhua on 7/29/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#ifndef cxEngineCore_cxOpenAL_h
#define cxEngineCore_cxOpenAL_h

#include "cxObject.h"

CX_CPP_BEGIN

class cxOpenAL : public cxObject
{
    friend cxCore;
private:
    CX_DECLARE(cxOpenAL);
protected:
    explicit cxOpenAL();
    virtual ~cxOpenAL();
private:
    static cxOpenAL *instance;
public:
    static cxOpenAL *Instance();
};

CX_CPP_END

#endif


