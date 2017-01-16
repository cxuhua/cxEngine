//
//  cxButton.cpp
//  cxEngineCore
//
//  Created by xuhua on 6/28/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//


#include "cxButton.h"

CX_CPP_BEGIN

CX_IMPLEMENT(cxButton);

cxButton::cxButton()
{
    longerTime = 0.5f;
    isenable = true;
    ispass = false;
    ispress = false;
    timer = nullptr;
    islong = false;
}

cxButton::~cxButton()
{
    
}

void cxButton::SetIsPass(cxBool v)
{
    ispass = v;
}

void cxButton::SetIsLong(cxBool v)
{
    islong = v;
}

void cxButton::SetIsEnable(cxBool v)
{
    isenable = v;
}

void cxButton::timerArrive(cxAction *pav)
{
    onLonger.Fire(this);
}

void cxButton::SetLongerTime(cxFloat v)
{
    longerTime = v;
}

void cxButton::timerStart()
{
    if(!islong){
        return;
    }
    timerExit();
    timer = cxTimer::Create(1.0, longerTime);
    timer->onArrive = CX_BIND_EVENT_A1(this,cxButton::timerArrive);
    timer->AttachTo(this);
}

void cxButton::timerExit()
{
    if(timer != nullptr){
        timer->Exit(true);
        timer = nullptr;
    }
}

cxPoint2F cxButton::GetPoint() const
{
    return point;
}

cxBool cxButton::OnDispatch(const cxTouchable *e)
{
    if(e->TouchCount() != 1 || !isenable){
        return false;
    }
    const cxTouchPoint *ep = e->TouchPoint(0);
    cxHitInfo hit = HitTest(ep->wp);
    if(!hit.hited && ispress){
        onRelease.Fire(this);
        timerExit();
        ispress = false;
    }
    if(!hit.hited){
        timerExit();
        return false;
    }
    point = hit.point;
    if(ep->IsBegan()){
        onPress.Fire(this);
        timerStart();
        ispress = true;
        return !ispass;
    }
    if(!ispress){
        return false;
    }
    if(ep->IsMoved()){
        onMove.Fire(this);
        if(ep->Length() > 10){
            timerExit();
        }
        return !ispass;
    }
    if(ep->IsEnded() && ep->IsTap()){
        onTap.Fire(this);
    }
    if(ep->IsEnded()){
        onRelease.Fire(this);
        ispress = false;
        timerExit();
        return !ispass;
    }
    return false;
}

CX_CPP_END

