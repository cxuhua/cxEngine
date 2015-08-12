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

CX_CPP_BEGIN

CX_IMPLEMENT(cxAlert);

cxAlert::cxAlert()
{
    SetResizeFlags(ResizeFill);
    SetAlpha(0.0f);
}

cxAlert::~cxAlert()
{
    
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
    if(tp->IsBegan() || tp->IsEnded()){
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
    cxScaleTo::Create(0.95f, 0.15f)->AttachTo(Body());
    cxAction *fade = cxFadeTo::Create(0.0f, 0.15f)->AttachTo(this);
    fade->onExit = [this](cxAction *pav){
        onHide.Fire(this);
        pav->View()->Remove();
    };
}

void cxAlert::OnEnter()
{
    Body()->SetScale(0.95f);
    cxScaleTo::Create(1.0f, 0.6f)->AttachTo(Body())->SetTiming(cxTiming::ElasticOut);
    SetAlpha(0.0f);
    cxAction *fade = cxFadeTo::Create(0.5f, 0.15f)->AttachTo(this);
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

