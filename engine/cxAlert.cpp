//
//  cxAlert.cpp
//  cxEngineCore
//
//  Created by xuhua on 8/11/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#include "cxEngine.h"
#include "cxFade.h"
#include "cxScale.h"
#include "cxAlert.h"
#include "cxTimer.h"

CX_CPP_BEGIN

CX_IMPLEMENT(cxAlert);

cxAlert::cxAlert()
{
    timeoutclose = false;
    isclose = true;
    SetResizeFlags(ResizeFill);
}

cxAlert::~cxAlert()
{
    
}

void cxAlert::SetTapOutsideClose(cxBool v)
{
    isclose = v;
}

void cxAlert::SetTimeout(cxFloat time)
{
    if(cxFloatIsEqual(time, 0)){
        return;
    }
    cxTimer *timer = cxTimer::Create(1, time);
    timer->onArrive +=[this](cxTimer *pav){
        timeoutclose = true;
        Hide();
    };
    timer->AttachTo(this);
}

cxBool cxAlert::IsTimeoutClose()
{
    return timeoutclose;
}

cxBool cxAlert::OnDispatch(const cxTouchable *e)
{
    if(e->TouchCount() != 1){
        return true;
    }
    cxView *body = Body();
    const cxTouchPoint *tp = e->TouchPoint(0);
    if(tp->IsMoved()){
        return true;
    }
    cxHitInfo info = body->HitTest(tp->wp);
    if(info.hited){
        return true;
    }
    if(!isclose){
        return true;
    }
    if(tp->IsEnded() && tp->IsTap()){
        Hide();
    }
    return true;
}

void cxAlert::Show()
{
    cxEngine::Instance()->Window()->Append(this);
}

void cxAlert::Hide()
{
    cxScaleTo::Create(0.95f, 0.1f)->AttachTo(Body());
    cxAction *fade = cxFadeTo::Create(0.0f, 0.1f)->AttachTo(this);
    fade->onExit = [this](cxAction *pav){
        onHide.Fire(this);
        pav->View()->Remove();
    };
}

void cxAlert::OnEnter()
{
    cxSprite::OnEnter();
    
    Body()->SetScale(0.95f);
    cxScaleTo::Create(1.0f, 0.5f)->AttachTo(Body())->SetTiming(cxTiming::ElasticOut);
    SetAlpha(0.0f);
    cxAction *fade = cxFadeTo::Create(0.6, 0.1f)->AttachTo(this);
    fade->onExit = [this](cxAction *pav){
        onShow.Fire(this);
    };
}

cxView *cxAlert::Body()
{
    CX_ASSERT(!IsEmpty(), "body must append");
    return At(0);
}

CX_CPP_END

