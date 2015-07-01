//
//  cxMove.h
//  cxEngineCore
//
//  Created by xuhua on 6/28/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#ifndef cxEngineCore_cxMove_h
#define cxEngineCore_cxMove_h

#include "cxAction.h"

CX_CPP_BEGIN

class cxMoveBy : public cxAction
{
public:
    CX_DECLARE(cxMoveBy);
protected:
    explicit cxMoveBy();
    virtual ~cxMoveBy();
protected:
    void OnStep(cxFloat dt);
private:
    cxPoint2F delta;        //m/s
public:
    cxAction *Reverse();
    cxAction *Clone();
    static cxMoveBy *Create(const cxPoint2F &d,cxFloat time);
};

class cxMoveTo : public cxMoveBy
{
public:
    CX_DECLARE(cxMoveTo);
protected:
    explicit cxMoveTo();
    virtual ~cxMoveTo();
protected:
    void OnStep(cxFloat dt);
    void OnInit();
private:
    cxPoint2F to;
    cxPoint2F from;
    cxPoint2F delta;
public:
    cxAction *Reverse();
    cxAction *Clone();
    static cxMoveTo *Create(const cxPoint2F &to,cxFloat time);
};

CX_CPP_END

#endif


