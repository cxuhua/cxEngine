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

struct cxTextAttr;
class cxEngine : public cxObject,public cxTouchable
{
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
    cxRender *render;
    cxBool layout;
    cxRect4F bound;
    cxBox4F box;
    cxSize2F size;
    cxSize2F plansize;
    cxPoint2F planscale;
    cxInt nextIter;
    cxInt iter;
    cxFloat dt;
    cxDouble time;
    cxBool init;
    cxBool pause;
    uv_loop_t looper;
    cxWindow *window;
    cxBool istouch;
public:
    
    cxStr *TextImage(const cxStr *txt,const cxTextAttr &attr,cxSize2F &size);
    
    static void Init(cxEngine *engine);
    static void Destroy();
    static cxEngine *Instance();
    
    uv_loop_t *Looper();
    
    cxWindow *Window() const;
    
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


