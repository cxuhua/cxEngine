//
//  cxTint.h
//  cxEngineCore
//
//  Created by xuhua on 7/4/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#ifndef cxEngineCore_cxTint_h
#define cxEngineCore_cxTint_h

#include "cxAction.h"

CX_CPP_BEGIN

class cxTintBy : public cxAction
{
public:
    CX_DECLARE(cxTintBy);
protected:
    explicit cxTintBy();
    virtual ~cxTintBy();
protected:
    void OnStep(cxFloat dt);
private:
    cxColor4F delta;
public:
    cxAction *Reverse();
    cxAction *Clone();
    static cxTintBy *Create(cxFloat delta,cxFloat time);
    static cxTintBy *Create(const cxColor4F &delta,cxFloat time);
};

class cxTintTo : public cxAction
{
public:
    CX_DECLARE(cxTintTo);
protected:
    explicit cxTintTo();
    virtual ~cxTintTo();
protected:
    void OnStep(cxFloat dt);
    void OnInit();
private:
    cxColor4F from;
    cxColor4F to;
    cxColor4F delta;
public:
    cxAction *Reverse();
    cxAction *Clone();
    static cxTintTo *Create(const cxColor4F &to,cxFloat time);
    static cxTintTo *Create(cxFloat to,cxFloat time);
};

CX_CPP_END

#endif


