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
    actions = cxArray::Alloc();
}

cxAction::~cxAction()
{
    actions->Release();
    onFree.Fire(this);
}

cxBool cxAction::HasAction(cxActionId aid) const
{
    cxBool has = false;
    actions->Elements<cxAction>([&aid,&has](cxAction *pav) -> cxBool{
        if(pav->ID() == aid && !pav->IsExit()){
            has = true;
            return true;
        }
        return false;
    });
    return has;
}

cxInt cxAction::ActionSize() const
{
    return actions->Size();
}

cxAction *cxAction::GetAction(cxActionId aid)
{
    cxAction *ptr = nullptr;
    actions->Elements<cxAction>([&aid,&ptr](cxAction *pav) -> cxBool{
        if(pav->ID() == aid && !pav->IsExit()){
            ptr = pav;
            return true;
        }
        return false;
    });
    return ptr;
}

cxAction *cxAction::StopAction(cxActionId aid)
{
    actions->Elements<cxAction>([&aid](cxAction *pav) -> cxBool{
        if(pav->ID() == aid || aid == 0){
            pav->Stop();
        }
        return false;
    });
    return this;
}

cxAction *cxAction::ExitAction(cxActionId aid)
{
    actions->Elements<cxAction>([&aid](cxAction *pav) -> cxBool{
        if(pav->ID() == aid || aid == 0){
            pav->Exit(true);
        }
        return false;
    });
    return this;
}


cxAction *cxAction::Append(cxAction *action)
{
    CX_ASSERT(action != nullptr, "args error");
    CX_ASSERT(action->View() == nullptr, "action repeat append");
    ExitAction(action->ID());
    action->SetView(View());
    actions->Append(action);
    return this;
}

void cxAction::OnAppend(cxView *pview)
{
    actions->Elements<cxAction>([pview](cxAction *pav) -> cxBool {
        pav->SetView(pview);
        return false;
    });
}

void cxAction::runActions(cxFloat dt)
{
    cxInt mvc = 0;
    cxInt cnt = actions->Size();
    cxObject *mvs[64]={0};
    for(cxInt i = 0; i < cnt; i++){
        cxAction *action = actions->At(i)->To<cxAction>();
        if(!action->Update(dt)){
            continue;
        }
        if(mvc < 64){
            mvs[mvc++] = action;
        }
    }
    for(cxInt i = 0;i < mvc; i++){
        actions->Remove(mvs[i]);
    }
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
    actions->Elements<cxAction>([](cxAction *pav) -> cxBool {
        pav->OnExit();
        return false;
    });
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

void cxAction::Expire()
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
    if(isexit){
        goto exit;
    }
    if(ispause){
        return false;
    }
    if(actions->Size() > 0){
        runActions(dt);
    }
    if(isexit){
        goto exit;
    }
    dt *= Speed();
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

