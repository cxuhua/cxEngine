//
//  cxContainer.h
//  cxEngineCore
//
//  Created by xuhua on 6/28/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#ifndef cxEngineCore_cxContainer_h
#define cxEngineCore_cxContainer_h

#include "cxView.h"

CX_CPP_BEGIN

#define CX_CONTAINER_MOVE_ID        100000

class cxContainer : public cxView
{
public:
    CX_DECLARE(cxContainer);
public:
    typedef cxUInt SlidingType;
    static const SlidingType SlidingTypeNone          = 0;
    static const SlidingType SlidingTypeHorizontal    = 1 << 0;
    static const SlidingType SlidingTypeVertical      = 1 << 1;
protected:
    explicit cxContainer();
    virtual ~cxContainer();
protected:
    cxBool OnDispatch(const cxTouchable *e);
    void OnAppend(cxView *nview);
private:
    SlidingType slidingtype;
    cxPoint2F fixPosition(const cxPoint2F &pos);
    cxBool scale(const cxTouchable *e);
    cxFloat scaleinc;
    cxRange2F scalerange;
    cxFloat scaling;
    cxFloat scaledis;
    cxBool isscaling;
    cxBool issliding;
    cxFloat slidingspeed;
    cxFloat slidingtime;
protected:
    cxView *Body();
    cxContainer *FixPosition();
public:
    cxContainer *SetScaleRange(const cxRange2F &v);
    cxContainer *EnableScale(cxBool v);
    cxContainer *EnableSliding(cxBool v);
    cxContainer *SetSlidingType(SlidingType v);
    cxContainer *SetSlidingSpeed(cxFloat v);
    cxContainer *SetSlidingTime(cxFloat v);
};
CX_CPP_END

#endif


