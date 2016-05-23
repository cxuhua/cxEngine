//
//  cxEngine.h
//  cxEngineCore
//
//  Created by xuhua on 6/11/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#ifndef cxEngineCore_cxEngine_h
#define cxEngineCore_cxEngine_h

#include <queue>
#include <core/cxHash.h>
#include <math/cxMath.h>
#include <math/cxSize2F.h>
#include <math/cxBox4F.h>
#include <math/cxRect4F.h>
#include "cxWindow.h"
#include "cxRender.h"
#include "cxTouch.h"
#include "cxFrames.h"
#include "cxActionGroup.h"

CX_CPP_BEGIN

class cxAsyncEvent
{
private:
    cxLong key;
    std::string data;
public:
    cxAsyncEvent(cxLong akey,cchars adata,cxInt length);
    cxLong Key();
    const cxStr *Data();
};

struct cxTextAttr;
class cxEngine : public cxObject,public cxTouchable
{
    friend cxCore;
private:
    CX_DECLARE(cxEngine);
protected:
    
    static cxEngine *instance;
    
    explicit cxEngine();
    virtual ~cxEngine();
    
    virtual void OnMain();
    
    virtual void OnPause();
    virtual void OnResume();
    virtual void OnMemoryWarning();
    
    virtual void OnUpdate(cxFloat dt);
    
    virtual void OnEvent(cxAsyncEvent *e);
private:
    cxFloat timevar;
    cxRender *render;
    cxBool layout;
    cxRect4F bound;
    cxBox4F box;
    cxSize2F winsize;
    cxSize2F plansize;
    cxPoint2F planscale;
    cxInt nextIter;
    cxInt iter;
    cxFloat dt;
    cxDouble time;
    cxBool init;
    cxBool pause;
    cxWindow *window;
    cxBool istouch;
    cxBool isreset;
    cxHash *configs;
    //
    uv_mutex_t mutex;
    void runEvents();
    std::queue<cxAsyncEvent> events;
    //帧序列和帧动画
    cxHash *frames;
    cxHash *actions;
public:
    void LoadLocalized(cchars file);
    void LoadTexture(cchars file,cchars key=nullptr);
    void LoadFrames(cchars csv);
    const cxFrames *GetFrames(cchars name,cxInt level=1);
    void LoadActions(cchars csv);
    const cxActionGroup *GetActions(cchars fmt,...);
public:
    
    void SetWindow(cxWindow *win);
    
    void PushEvent(cxLong key,const cxStr *data);
    void PushEvent(cxLong key,cchars data,cxInt length);
    
    void SetIsTouch(cxBool v);
    
    void LoadConfig(cchars file);
    const cxStr *Config(cchars key) const;
    
    cxPoint2F FixScaleW();
    cxPoint2F FixScaleH();
    
    cxFloat PlanWidth();
    cxFloat PlanHeight();
    const cxSize2F PlanSize();
    
    const cxFloat Time() const;
    
    
    static void Destroy();
    static cxEngine *Instance();
    static void Startup(cxBool layout);
    
    //need platform imp
    cxStr *TextImage(const cxStr *txt,const cxTextAttr &attr,cxSize2F &size);
    void Exit();
    
    void Reset();

    cxWindow *Window() const;
    
    cxInt FPS() const;
    cxFloat Delta() const;
    
    const cxPoint2F &PlanScale() const;
    void SetPlanSize(const cxSize2F &v);

    void Pause();
    void Resume();
    void MemoryWarning();
    void Layout(cxFloat w,cxFloat h);
    
    void Run();
    
    cxBool IsTouch() const;
    
    const cxSize2F &WinSize() const;
    const cxRect4F &WinBound() const;
    const cxBox4F &WinBox() const;
    cxBool Contain(const cxBoxPoint3F &b);
    
    cxFloat ScaleFactor() const;
    
    cxInt Iter() const;
    void SetIter(cxInt aiter);
};

CX_CPP_END

#endif


