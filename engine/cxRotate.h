//
//  cxRotate.h
//  cxEngineCore
//
//  Created by xuhua on 6/29/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#ifndef cxEngineCore_cxRotate_h
#define cxEngineCore_cxRotate_h

#include "cxAction.h"

CX_CPP_BEGIN

class cxRotateBy : public cxAction
{
public:
    CX_DECLARE(cxRotateBy);
protected:
    explicit cxRotateBy();
    virtual ~cxRotateBy();
private:
    cxFloat delta;  //v/s
protected:
    void OnStep(cxFloat dt);
public:
    cxAction *Reverse();
    cxAction *Clone();
    static cxRotateBy *Create(cxFloat delta,cxFloat time);
};

class cxRotateTo : public cxAction
{
public:
    CX_DECLARE(cxRotateTo);
protected:
    explicit cxRotateTo();
    virtual ~cxRotateTo();
private:
    cxFloat from;
    cxFloat to;
    cxFloat delta;
protected:
    void OnStep(cxFloat dt);
    void OnInit();
public:
    cxAction *Reverse();
    cxAction *Clone();
    static cxRotateTo *Create(cxFloat to,cxFloat time);
};

CX_CPP_END

#endif


