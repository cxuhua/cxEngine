//
//  cxAction.cpp
//  cxEngineCore
//
//  Created by xuhua on 6/16/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#include "cxView.h"
#include "cxAction.h"

CX_CPP_BEGIN

CX_IMPLEMENT(cxAction);

cxFloat defaultTimingFunc(cxFloat time)
{
    return time;
}

cxAction::cxAction()
{
    repeat = 1;
    actionId = 0;
    timing = defaultTimingFunc;
    pview = nullptr;
    ispause = false;
    elapsed = 0;
    speed = 1.0f;
    time = 0;
    isinit = false;
    isexit = false;
}

cxAction::~cxAction()
{
    
}

cxAction *cxAction::SetID(cxULong aid)
{
    actionId = aid;
    return this;
}

cxAction *cxAction::SetRepeat(cxInt v)
{
    repeat = v;
    return this;
}

cxULong cxAction::ID() const
{
    return actionId == 0 ? (cxULong)this : actionId;
}

void cxAction::Reset()
{
    elapsed = 0;
    isinit = false;
    isexit = false;
    ispause = false;
    OnReset();
}

void cxAction::SetExit(cxBool v)
{
    isexit = v;
    repeat = 0;
}

void cxAction::OnReset()
{
    
}

void cxAction::OnInit()
{
    
}

void cxAction::OnStep(cxFloat dt)
{
    
}

void cxAction::OnExit()
{
    
}

void cxAction::OnStop()
{
    
}

void cxAction::Stop()
{
    isexit = true;
}

void cxAction::Pause()
{
    ispause = true;
}

void cxAction::Resume()
{
    ispause = false;
}

cxFloat cxAction::deltaTimeFix(cxFloat dt)
{
    if(elapsed <= time){
        return dt;
    }
    dt -= (elapsed - time);
    dt = CX_MAX(dt, 0);
    elapsed = time;
    return dt;
}

cxAction *cxAction::AttachTo(cxView *pview)
{
    pview->Append(this);
    return this;
}

cxAction *cxAction::Reverse()
{
    CX_ASSERT(false, "this action have not imp reversed method");
    return nullptr;
}

cxAction *cxAction::Clone()
{
    CX_ASSERT(false, "this action have not imp clone method");
    return nullptr;
}

cxBool cxAction::IsPause() const
{
    return ispause;
}

cxBool cxAction::Update(cxFloat dt)
{
    CX_ASSERT(pview != nullptr, "action view not set");
    if(ispause){
        return false;
    }
    if(!isinit){
        prev = timing(0.0f) * time;
        OnInit();
        onInit.Fire(this);
        isinit = true;
    }
    if(time <= 0 || isexit){
        goto exit;
    }
    if(!isexit) {
        dt *= speed;
        elapsed += dt;
        deltaTimeFix(dt);
        cxDouble curr = Progress() * time;
        dt = curr - prev;
        prev = curr;
        OnStep(dt);
    }
    if(elapsed >= time){
        isexit = true;
        OnStop();
        onStop.Fire(this);
        if(--repeat > 0)Reset();
    }
exit:
    if(isexit || repeat <= 0){
        OnExit();
        onExit.Fire(this);
    }
    return isexit;
}

cxTimingFunc cxAction::Timing() const
{
    return timing;
}

cxAction *cxAction::SetTiming(cxTimingFunc f)
{
    timing = f;
    return this;
}

cxFloat cxAction::Progress() const
{
    if(cxFloatIsEqual(time, 0)){
        return 0;
    }
    return timing(elapsed/time);
}

const cxFloat cxAction::Time() const
{
    return time;
}

const cxFloat cxAction::Elapsed() const
{
    return elapsed;
}

cxAction *cxAction::SetSpeed(cxFloat v)
{
    speed = v;
    return this;
}

cxFloat cxAction::Speed() const
{
    return speed;
}

cxAction *cxAction::SetView(cxView *view)
{
    pview = view;
    return this;
}

cxView *cxAction::View()
{
    return pview;
}

cxAction *cxAction::SetTime(cxFloat v)
{
    time = v;
    return this;
}

CX_CPP_END

