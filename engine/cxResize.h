//
//  cxResize.h
//  cxEngineCore
//
//  Created by xuhua on 7/7/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#ifndef cxEngineCore_cxResize_h
#define cxEngineCore_cxResize_h

#include "cxAction.h"

CX_CPP_BEGIN

class cxResizeBy : public cxAction
{
public:
    CX_DECLARE(cxResizeBy);
protected:
    explicit cxResizeBy();
    virtual ~cxResizeBy();
protected:
    void OnStep(cxFloat dt);
private:
    cxSize2F delta;
public:
    static cxResizeBy *Create(const cxSize2F &delta,cxFloat time);
    cxAction *Reverse();
    cxAction *Clone();
};

class cxResizeTo : public cxAction
{
public:
    CX_DECLARE(cxResizeTo);
protected:
    explicit cxResizeTo();
    virtual ~cxResizeTo();
protected:
    void OnStep(cxFloat dt);
    void OnInit();
private:
    cxSize2F from;
    cxSize2F to;
    cxSize2F delta;
public:
    static cxResizeTo *Create(const cxSize2F &to,cxFloat time);
    cxAction *Reverse();
    cxAction *Clone();
};

CX_CPP_END

#endif


