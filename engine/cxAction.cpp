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
    isinit = false;
    isexit = false;
    ispause = false;
    OnReset();
    onReset.Fire(this);
}

void cxAction::Exit(cxBool v)
{
    isexit = v;
    repeat = 0;
}

void cxAction::OnReset()
{
    
}

void cxAction::OnInit()
{
    onInit.Fire(this);
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
    elapsed = time;
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
        elapsed = 0.0f;
        elapsedvar = 0.0f;
        prev = Progress() * time;
        OnInit();
        onStart.Fire(this);
        isinit = true;
    }
    if(time <= 0 || isexit){
        goto exit;
    }
    if(!isexit) {
        dt *= speed;
        elapsed += dt;
        deltaTimeFix(dt);
        
        curr = Progress() * time;
        delta = curr - prev;
        prev = curr;
        elapsedvar += delta;
        OnStep(delta);
        onStep.Fire(this, delta);
    }
    if(elapsed >= time){
        repeat --;
        isexit = true;
        OnStop();
        onStop.Fire(this);
        if(repeat > 0)Reset();
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
    time = INT_MAX;
    return this;
}

cxAction *cxAction::SetTime(cxFloat v)
{
    time = v;
    return this;
}

CX_CPP_END

