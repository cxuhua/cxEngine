//
//  cxScale.h
//  cxEngineCore
//
//  Created by xuhua on 6/29/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#ifndef cxEngineCore_cxScale_h
#define cxEngineCore_cxScale_h

#include "cxAction.h"

CX_CPP_BEGIN

class cxScaleBy : public cxAction
{
public:
    CX_DECLARE(cxScaleBy);
protected:
    explicit cxScaleBy();
    virtual ~cxScaleBy();
private:
    cxPoint2F delta;        //v/s
protected:
    void OnStep(cxFloat dt);
public:
    cxAction *Reverse();
    cxAction *Clone();
    static cxScaleBy *Create(const cxPoint2F &d,cxFloat time);
};

class cxScaleTo : public cxAction
{
public:
    CX_DECLARE(cxScaleTo);
protected:
    explicit cxScaleTo();
    virtual ~cxScaleTo();
private:
    cxPoint2F from;
    cxPoint2F to;
    cxPoint2F delta;
protected:
    void OnStep(cxFloat dt);
    void OnInit();
public:
    cxAction *Reverse();
    cxAction *Clone();
    static cxScaleTo *Create(const cxPoint2F &to,cxFloat time);
};

CX_CPP_END

#endif


