//
//  cxPoints.h
//  cxEngineCore
//  points move,从当前位置开始按点移动，所以
//  如果第一个点的位置和物体位置一致，第一个点默认为开始位置，不包含在ps中
//  如要获取数据点请使用 GetIndex
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
    virtual cxBool OnArrive();
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
    void updateAttr(cxInt i);
public:
    cxInt GetIndex();
    void Jump(cxInt v);
    void SetMin(const cxFloat &v);
    const cxInt Index();
    const cxPoint2FArray &Points();
    const cxFloat MoveSpeed();
    void SetPoints(const cxPoint2FArray &v,cxBool combine=true);
    void Append(const cxPoint2F &pos);
    void SetMoveSpeed(const cxFloat &v);
    cxAction *Reverse();
    cxAction *Clone();
};

CX_CPP_END

#endif


