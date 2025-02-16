//
//  cxAction.cpp
//  cxEngineCore
//
//  Created by xuhua on 6/16/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#include "cxEngine.h"
#include "cxView.h"
#include "cxAction.h"

CX_CPP_BEGIN

CX_IMPLEMENT(cxAction);

cxFloat defaultTimingFunc(cxFloat time)
{
    return time;
}

void cxAction::Remove(cxAction *pav)
{
    pav->View()->Remove();
}

cxAction::cxAction()
{
    delay = 0;
    delayvar = 0;
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
    onFree.Fire(this);
}

const cxInt cxAction::Repeat() const
{
    return repeat;
}

cxAction *cxAction::Create(cxFloat time)
{
    cxAction *rv = cxAction::Create();
    rv->SetTime(time);
    return rv;
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
    delayvar = delay;
    isinit = false;
    isexit = false;
    ispause = false;
    OnReset();
}

cxBool cxAction::IsExit() const
{
    return isexit;
}

cxFloat cxAction::Delay() const
{
    return delay;
}

void cxAction::SetDelay(cxFloat v)
{
    delay = v;
    delayvar = v;
}

void cxAction::Exit(cxBool v)
{
    isexit = v;
    repeat = 0;
}

void cxAction::OnReset()
{
    onReset.Fire(this);
}

void cxAction::OnInit()
{
    onInit.Fire(this);
}

void cxAction::OnStep(cxFloat dt)
{
    onStep.Fire(this, dt);
}

void cxAction::OnExit()
{
    onExit.Fire(this);
}

void cxAction::OnStop()
{
    onStop.Fire(this);
}

void cxAction::Stop()
{
    elapsed = time;
    repeat = 0;
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

cxAction *cxAction::Run()
{
    cxEngine::Instance()->Window()->Append(this);
    return this;
}

cxAction *cxAction::AttachTo(cxView *pview)
{
    CX_ASSERT(pview != nullptr, "args null");
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
    dt *= Speed();
    if(ispause){
        return false;
    }
    if(delayvar > 0){
        delayvar -= dt;
        return false;
    }
    if(!isinit && !isexit){
        elapsed = 0.0f;
        elapsedvar = 0.0f;
        prev = Progress() * time;
        OnInit();
        isinit = true;
    }
    if(time <= 0 || isexit){
        goto exit;
    }
    if(!isexit) {
        elapsed += dt;
        deltaTimeFix(dt);
        curr = Progress() * time;
        delta = curr - prev;
        prev = curr;
        elapsedvar += delta;
        OnStep(delta);
    }
    if(elapsed >= time){
        repeat --;
        isexit = true;
        OnStop();
        if(repeat > 0)Reset();
    }
exit:
    if(isexit || repeat <= 0){
        OnExit();
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
    CX_ASSERT(time > 0, "action time error");
    return timing(elapsed/time);
}

const cxFloat cxAction::Delta() const
{
    return delta;
}

const cxFloat cxAction::Time() const
{
    CX_ASSERT(time != 0, "time zero");
    return time;
}

const cxFloat cxAction::Elapsed() const
{
    return elapsedvar;
}

cxAction *cxAction::SetSpeed(cxFloat v)
{
    speed = v;
    return this;
}

cxAction *cxAction::AddSpeed(cxFloat v)
{
    speed += v;
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

cxAction *cxAction::Forever()
{
    SetTime(INT_MAX);
    return this;
}

cxAction *cxAction::SetTime(cxFloat v)
{
    time = v;
    return this;
}

CX_CPP_END

