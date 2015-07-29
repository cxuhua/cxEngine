//
//  cxSound.h
//  cxEngineCore
//
//  Created by xuhua on 7/29/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#ifndef cxEngineCore_cxSound_h
#define cxEngineCore_cxSound_h

#include "cxOpenAL.h"
#include "cxAction.h"

CX_CPP_BEGIN

class cxSound : public cxAction
{
public:
    CX_DECLARE(cxSound);
protected:
    explicit cxSound();
    virtual ~cxSound();
protected:
    void OnInit();
    void OnStep(cxFloat dt);
private:
    cxALSource *source;
public:
    static cxSound *Create(cchars file);
};

CX_CPP_END

#endif


