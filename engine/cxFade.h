//
//  cxFade.h
//  cxEngineCore
//
//  Created by xuhua on 7/4/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#ifndef cxEngineCore_cxFade_h
#define cxEngineCore_cxFade_h

#include "cxAction.h"

CX_CPP_BEGIN

class cxFadeBy : public cxAction
{
public:
    CX_DECLARE(cxFadeBy);
protected:
    explicit cxFadeBy();
    virtual ~cxFadeBy();
protected:
    void OnStep(cxFloat dt);
private:
    cxFloat delta;
public:
    cxAction *Reverse();
    cxAction *Clone();
    static cxFadeBy *Create(cxFloat delta,cxFloat time);
};

class cxFadeTo : public cxAction
{
public:
    CX_DECLARE(cxFadeTo);
protected:
    explicit cxFadeTo();
    virtual ~cxFadeTo();
protected:
    void OnStep(cxFloat dt);
    void OnInit();
private:
    cxFloat from;
    cxFloat to;
    cxFloat delta;
public:
    cxAction *Reverse();
    cxAction *Clone();
    static cxFadeTo *Create(cxFloat to,cxFloat time);
};

CX_CPP_END

#endif


