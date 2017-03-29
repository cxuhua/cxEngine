//
//  cxForward.h
//  cxEngineCore
//
//  Created by xuhua on 29/03/2017.
//  Copyright © 2017 xuhua. All rights reserved.
//

#ifndef cxEngineCore_cxForward_h
#define cxEngineCore_cxForward_h

#include "cxAction.h"

CX_CPP_BEGIN

class cxForward : public cxAction
{
public:
    CX_DECLARE(cxForward);
protected:
    explicit cxForward();
    virtual ~cxForward();
protected:
    void OnStep(cxFloat dt);
    void OnInit();
private:
    cxPoint2F angle;
    cxFloat speed;
public:
    void SetSpeed(const cxFloat &v);
    void SetAngle(const cxFloat &v);
    void SetDegrees(const cxFloat &v);
};

CX_CPP_END

#endif


