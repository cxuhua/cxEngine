//  cxGesture.h
//  Created by 徐华 on 2018/8/4.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#ifndef cxEngine____FILEBASENAME____h
#define cxEngine____FILEBASENAME____h

#include "cxView.h"

CX_CPP_BEGIN

class cxGesture : public cxView
{
public:
    CX_DECLARE(cxGesture);
protected:
    explicit cxGesture();
    virtual ~cxGesture();
private:
    cxDouble tapTime[2];
    cxInt tapCount;
private:
    struct swipePoint {
        cxPoint2F pos;
        cxDouble time;
    };
    cxBool swipeischeck;
    std::vector<swipePoint> swipePoints;
    cxBool checkSwipe(const cxTouchPoint *ep);
    cxBool computeSwipe();
    std::vector<cxFloat> speeds;
    std::vector<cxFloat> angles;
public:
    typedef cxUInt SwipeType;
    static const SwipeType SwipeTypeDirectionNone    = 0;
    static const SwipeType SwipeTypeDirectionRight   = 1 << 0;
    static const SwipeType SwipeTypeDirectionUp      = 1 << 1;
    static const SwipeType SwipeTypeDirectionLeft    = 1 << 2;
    static const SwipeType SwipeTypeDirectionDown    = 1 << 3;
private:
    
    cxUInt flags;
    cxBool touchIsPass;
public:
    cxGesture *SetFlags(cxUInt v);
    cxGesture *DelFlags(cxUInt v);
public:
    cxEvent<cxGesture> onDoubleTap;
    cxEvent<cxGesture, SwipeType,cxFloat> onSwipe;
protected:
    cxBool OnDispatch(const cxTouchable *e);
    virtual void OnSwipe(SwipeType type,cxFloat speed);
    virtual void OnDoubleTap();
};

CX_CPP_END

#endif
