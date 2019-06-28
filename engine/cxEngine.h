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
#include <core/cxLooper.h>
#include <math/cxMath.h>
#include <math/cxSize2F.h>
#include <math/cxBox4F.h>
#include <math/cxRect4F.h>
#include <core/cxSync.h>
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
    cchars Data();
    cxLong ToLong();
    cxDouble ToDouble();
    const cxJson *Json();
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
    virtual void OnWarning();
    
    virtual void OnUpdate(cxFloat dt);
    
    virtual void OnEvent(cxAsyncEvent *e);
private:
    cxLooper *looper;
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
    cxRWLock mutex;
    void runEvents();
    std::queue<cxAsyncEvent> events;
    //
    void runInputs();
    //帧序列和帧动画
    cxHash *frames;
    cxHash *actions;
public:
    cxLooper *Looper();
    // 加载语言文件
    void LoadLocalized(cchars file);
    // 加载纹理文件
    cxTexture *LoadTexture(cchars file,cchars key=nullptr);
    //加载多个纹理,必须以NULL结尾表示结束，请使用LoadTextures宏
    #define LoadTextures(f,...) __LoadTextures__(f, ##__VA_ARGS__,NULL)
    void __LoadTextures__(cchars file,...);
    // 加载帧序列
    void LoadFrames(cchars csv);
    const cxFrames *GetFrames(cchars name,cxInt level=1);
    // 加载动作序列
    void LoadActions(cchars csv);
    const cxActionGroup *GetActions(cchars fmt,...);
public:
    // 设置底层窗口
    void SetWindow(cxWindow *win);
    // 推送一个事件
    void PushEvent(cxLong key);
    void PushEvent(cxLong key,cxLong data);
    void PushEvent(cxLong key,cxDouble data);
    void PushEvent(cxLong key,const cxStr *data);
    void PushEvent(cxLong key,cchars data,cxInt length);
    // 是否响应屏幕
    void SetIsTouch(cxBool v);
    cxBool IsTouch() const;
    // 加载配置文件
    void LoadConfig(cchars file);
    const cxStr *Config(cchars key) const;
    // 等屏幕比例缩放
    cxPoint2F FixScaleW();
    cxPoint2F FixScaleH();
    // 设计图大小
    cxFloat PlanWidth();
    cxFloat PlanHeight();
    const cxSize2F PlanSize();
    // 从启动开始流逝的时间
    const cxFloat Time() const;
    //
    static void Destroy();
    static cxEngine *Instance();
    static void Startup(cxBool layout);
    //need platform imp
    const cxStr *TextImage(const cxStr *txt,const cxTextAttr &attr,cxSize2F &size);
    void Exit();
    void Reset();
    virtual void OpenURL(cxInt type,const cxStr *txt);
    cxWindow *Window() const;
    // 帧率和每帧时间
    cxInt FPS() const;
    cxFloat Delta() const;
    
    const cxPoint2F &PlanScale() const;
    void SetPlanSize(const cxSize2F &v);

    void Pause();
    void Resume();
    void Warning();
    void Layout(cxFloat w,cxFloat h);
    
    void Run();

    const cxSize2F &WinSize() const;
    const cxRect4F &WinBound() const;
    const cxBox4F &WinBox() const;
    cxBool Contain(const cxBoxPoint3F &b);
    
    cxFloat ScaleFactor() const;
    
    // 每帧迭代次数
    cxInt Iter() const;
    void SetIter(cxInt aiter);
};

CX_CPP_END

#endif


