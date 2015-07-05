//
//  cxBezier.h
//  cxEngineCore
//
//  Created by xuhua on 7/4/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#ifndef cxEngineCore_cxBezier_h
#define cxEngineCore_cxBezier_h

#include "cxAction.h"

CX_CPP_BEGIN

class cxBezier : public cxAction
{
public:
    CX_DECLARE(cxBezier);
protected:
    explicit cxBezier();
    virtual ~cxBezier();
protected:
    void OnInit();
    void OnStep(cxFloat dt);
private:
    cxPoint2F prev;
    cxPoint2F a;    //start pos
    cxPoint2F b;    //ctrl pos
    cxPoint2F c;    //end pos
    cxPoint2F d;    //
public:
    cxBezier *SetA(const cxPoint2F &aa);
    cxBezier *SetB(const cxPoint2F &ab);
    cxBezier *SetC(const cxPoint2F &ac);
    cxBezier *SetD(const cxPoint2F &ad);
    static cxBezier *Create(const cxPoint2F &b,const cxPoint2F &c,cxFloat time);
    static cxBezier *Create(const cxPoint2F &b,const cxPoint2F &c,const cxPoint2F &d,cxFloat time);
    cxAction *Reverse();
    cxAction *Clone();
};

CX_CPP_END

#endif


