//
//  cxAction.h
//  cxEngineCore
//
//  Created by xuhua on 6/16/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#ifndef cxEngineCore_cxAction_h
#define cxEngineCore_cxAction_h

#include "cxTiming.h"
#include <math/cxPoint2F.h>
#include <math/cxPoint3F.h>
#include <math/cxSize2F.h>
#include <math/cxColor4F.h>

CX_CPP_BEGIN

typedef cxULong cxActionId;

class cxView;
class cxAction : public cxObject
{
    friend cxView;
public:
    CX_DECLARE(cxAction);
public:
    const static cxInt MAX_REPEAT = INT_MAX;
protected:
    explicit cxAction();
    virtual ~cxAction();
    virtual void OnInit();
    virtual void OnExit();
    virtual void OnStop();
    virtual void OnReset();
    virtual void OnStep(cxFloat dt);
private:
    cxFloat delay;
    cxFloat delayvar;
    cxULong actionId;
    cxTimingFunc timing;
    cxView *pview;
    cxFloat speed;
    cxFloat elapsed;
    cxFloat elapsedvar;
    cxFloat time;
    cxFloat delta;
    cxFloat curr;
    cxFloat prev;
    cxBool ispause;
    cxBool isinit;
    cxBool isexit;
    cxFloat deltaTimeFix(cxFloat dt);
    cxInt repeat;
public:
    cxBool IsExit() const;
    
    cxFloat Delay() const;
    void SetDelay(cxFloat v);
    
    const cxInt Repeat() const;
    
    cxAction *SetID(cxULong aid);
    cxULong ID() const;
    
    cxAction *SetRepeat(cxInt v);
    
    cxTimingFunc Timing() const;
    cxAction *SetTiming(cxTimingFunc f);
    
    cxAction *SetTime(cxFloat v);
    const cxFloat Delta() const;
    const cxFloat Time() const;
    const cxFloat Elapsed() const;
    
    cxBool IsPause() const;
    
    cxAction *SetSpeed(cxFloat v);
    cxAction *AddSpeed(cxFloat v);
    cxFloat Speed() const;
    
    cxAction *SetView(cxView *view);
    cxView *View();
public:
    cxEvent<cxAction> onInit;
    cxEvent<cxAction> onStop;
    cxEvent<cxAction> onExit;
    cxEvent<cxAction> onReset;
    cxEvent<cxAction> onFree;
    cxEvent<cxAction,cxFloat> onStep;
public:
    static void Remove(cxAction *pav);
    static cxAction *Create(cxFloat time);
    cxAction *Forever();
    cxFloat Progress() const;
    //v = true,not invoke onStop
    virtual void Exit(cxBool v);
    virtual void Reset();
    virtual void Pause();
    virtual void Resume();
    virtual void Expire();
    //invoke stop
    virtual void Stop();
    cxAction *AttachTo(cxView *pview);
    // append to cxEngine::Window
    cxAction *Run();
    virtual cxAction *Clone();
    virtual cxAction *Reverse();
    //return true -> exit
    virtual cxBool Update(cxFloat dt);
};

CX_CPP_END

#endif


