//
//  cxSpline.h
//  cxEngineCore
//
//  Created by xuhua on 7/3/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#ifndef cxEngineCore_cxSpline_h
#define cxEngineCore_cxSpline_h

#include <math/cxPoint2F.h>
#include "cxAction.h"

CX_CPP_BEGIN

class cxSpline : public cxAction
{
public:
    CX_DECLARE(cxSpline);
protected:
    explicit cxSpline();
    virtual ~cxSpline();
    void OnStep(cxFloat dt);
    void OnInit();
    virtual void OnAngle();
private:
    cxPoint2FArray points;
    cxPoint2F prev;
    const cxPoint2F &at(cxInt idx);
    cxFloat delta;
    cxFloat tension;
    cxFloat angle;
    cxFloat speed;
    void computeTime();
public:
    cxSpline *SetSpeed(cxFloat v);
    cxSpline *SetTension(cxFloat v);
    cxSpline *Append(const cxPoint2F &v);
    cxSpline *Clear();
    cxFloat Angle();
    cxInt Angle(cxFloat split,cxFloat *off);
    cxAction *Reverse();
    cxAction *Clone();
public:
    cxEvent<cxSpline> onAngle;
};

CX_CPP_END

#endif


