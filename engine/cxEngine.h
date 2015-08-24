//
//  cxEngine.h
//  cxEngineCore
//
//  Created by xuhua on 6/11/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#ifndef cxEngineCore_cxEngine_h
#define cxEngineCore_cxEngine_h

#include <core/cxHash.h>
#include <math/cxMath.h>
#include <math/cxSize2F.h>
#include <math/cxBox4F.h>
#include <math/cxRect4F.h>
#include "cxWindow.h"
#include "cxRender.h"
#include "cxTouch.h"

CX_CPP_BEGIN

enum cxEngineId{
    cxEngineIdMusic     = 1,
    
    cxEngineIdMax
};

struct cxTextAttr;
class cxEngine : public cxObject,public cxTouchable
{
    friend cxCore;
private:
    CX_DECLARE(cxEngine);
public:
    static constexpr cxFloat DTM = 1.0f/30.0f;
protected:
    static cxEngine *instance;
    explicit cxEngine();
    virtual ~cxEngine();
    
    virtual void OnMain();
    
    virtual void OnPause();
    virtual void OnResume();
    virtual void OnMemory();
    
    virtual void OnUpdate(cxFloat dt);
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
public:
    
    void LoadConfig(cchars file);
    const cxStr *Config(cchars key) const;
    
    cxPoint2F FixWidth();
    cxPoint2F FixHeight();
    
    cxFloat PlanWidth();
    cxFloat PlanHeight();
    const cxSize2F PlanSize();
    
    const cxFloat Time() const;
    cxStr *TextImage(const cxStr *txt,const cxTextAttr &attr,cxSize2F &size);
    
    static void Destroy();
    static cxEngine *Instance();
    static void Startup(cxBool layout);
    
    void Reset();

    cxWindow *Window() const;
    
    cxInt FPS() const;
    cxFloat Delta() const;
    
    const cxPoint2F &PlanScale() const;
    void SetPlanSize(const cxSize2F &v);

    void Pause();
    void Resume();
    void Memory();
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


