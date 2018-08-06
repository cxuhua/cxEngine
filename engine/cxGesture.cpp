//
//  cxGesture.cpp
//  cxEngineCore
//
//  Created by 徐华 on 2018/8/4.
//  Copyright © 2018 xuhua. All rights reserved.
//

#include <core/cxUtil.h>
#include "cxGesture.h"

CX_CPP_BEGIN

CX_IMPLEMENT(cxGesture);

cxGesture::cxGesture()
{
    tapTime[0] = 0;
    tapTime[1] = 0;
    tapCount = 0;
    flags = cxTouchPoint::FlagsGestureTypeSwipe|cxTouchPoint::FlagsGestureTypeDoubleTap;
    touchIsPass = false;
}

cxGesture::~cxGesture()
{
    
}

cxGesture *cxGesture::SetFlags(cxUInt v)
{
    flags |= v;
    return this;
}

cxGesture *cxGesture::DelFlags(cxUInt v)
{
    flags &= ~v;
    return this;
}

cxBool cxGesture::checkSwipe(const cxTouchPoint *ep)
{
    if(ep->IsTap()){
        return false;
    }
    cxHitInfo hit = HitTest(ep->wp);
    if(!hit.hited){
        return false;
    }
    if(ep->IsBegan()){
        *ep->Flags() |= cxTouchPoint::FlagsGestureTypeSwipe;
        return true;
    }
    if(!(*ep->Flags() & cxTouchPoint::FlagsGestureTypeSwipe)){
        return false;
    }
    if(ep->IsMoved()){
        return !touchIsPass;
    }
    cxPoint2F sp = ep->Speed();
    if(sp.Length() < 1000){
        return !touchIsPass;
    }
    if(fabs(sp.x) > fabs(sp.y)){
        if(sp.x > 0){
            OnSwipe(SwipeTypeDirectionRight,sp.Length());
        }else if(sp.x < 0){
            OnSwipe(SwipeTypeDirectionLeft,sp.Length());
        }
    }else if(fabs(sp.x) < fabs(sp.y)){
        if(sp.y > 0){
            OnSwipe(SwipeTypeDirectionUp,sp.Length());
        }else if(sp.y < 0){
            OnSwipe(SwipeTypeDirectionDown,sp.Length());
        }
    }
    return !touchIsPass;
}

void cxGesture::OnDoubleTap()
{
    onDoubleTap.Fire(this);
}

cxBool cxGesture::OnDispatch(const cxTouchable *e)
{
    if(e->TouchCount() == 1 && (flags & cxTouchPoint::FlagsGestureTypeDoubleTap)){
        const cxTouchPoint *ep = e->TouchPoint(0);
        cxHitInfo hit = HitTest(ep->wp);
        if(hit.hited && ep->IsEnded() && ep->IsTap()){
            tapTime[tapCount] = cxUtil::Timestamp();
            tapCount++;
            if(tapCount >= 2 && tapTime[1] - tapTime[0] < 0.3){
                OnDoubleTap();
            }
            if(tapCount >= 2){
                tapCount = 0;
            }
        }
    }
    if(e->TouchCount() == 1 && (flags & cxTouchPoint::FlagsGestureTypeSwipe)){
        const cxTouchPoint *ep = e->TouchPoint(0);
        if(checkSwipe(ep)){
            return true;
        }
    }
    return false;
}

void cxGesture::OnSwipe(SwipeType type,cxFloat speed)
{
    onSwipe.Fire(this, type, speed);
}
CX_CPP_END

