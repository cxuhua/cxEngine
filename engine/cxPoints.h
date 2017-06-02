//
//  cxPoints.h
//  cxEngineCore
//  points move
//  Created by xuhua on 26/03/2017.
//  Copyright © 2017 xuhua. All rights reserved.
//

#ifndef cxEngineCore_cxPoints_h
#define cxEngineCore_cxPoints_h

#include <math/cxPoint2I.h>
#include "cxAction.h"

CX_CPP_BEGIN

class cxPoints : public cxAction
{
public:
    CX_DECLARE(cxPoints);
protected:
    explicit cxPoints();
    virtual ~cxPoints();
protected:
    void OnStep(cxFloat dt);
    void OnInit();
    virtual cxBool OnArrive(const cxInt &v);
public:
    virtual cxPoint2F At(cxInt i);
private:
    cxFloat min;
    cxPoint2FArray ps;
    cxFloat speed;
    cxInt idx;
    cxPoint2F np;
    cxPoint2F angle;
    cxBool nextPoint(cxInt i);
public:
    void SetMin(const cxFloat &v);
    const cxInt Index();
    const cxPoint2FArray &Points();
    const cxFloat Speed();
    void SetPoints(const cxPoint2FArray &v,cxBool combine=true);
    void SetSpeed(const cxFloat &v);
    cxAction *Reverse();
    cxAction *Clone();
};

CX_CPP_END

#endif


