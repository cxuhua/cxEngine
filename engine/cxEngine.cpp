//
//  cxEngine.cpp
//  cxEngineCore
//
//  Created by xuhua on 6/11/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#include <core/cxCSV.h>
#include <core/cxUtil.h>
#include <core/cxAutoPool.h>
#include <math/cxMatrixF.h>
#include <core/cxNotice.h>
#include "cxEngine.h"
#include "cxOpenGL.h"
#include "cxShader.h"
#include "cxView.h"

CX_CPP_BEGIN

cxEngine *cxEngine::instance = nullptr;

CX_IMPLEMENT(cxEngine);

void cxEngine::Reset()
{
    if(!isreset){
        isreset = true;
    }
}

void cxEngine::Destroy()
{
    cxObject::release(&instance);
}

cxEngine *cxEngine::Instance()
{
    CX_ASSERT(instance != nullptr, "instance not init,use cxEngine::Init");
    return instance;
}

void cxEngine::Startup(cxBool layout)
{
    cxSize2F winsiz = 0.0f;
    if(instance != nullptr){
        winsiz = instance->WinSize();
        cxEngine::Destroy();
        cxCore::Instance()->Clear();
    }
    instance = cxObject::alloc("Game")->To<cxEngine>();
    CX_ASSERT(instance != nullptr, "instance error");
    if(layout && !winsiz.IsZero()){
        instance->Layout(winsiz.w, winsiz.h);
    }
}

cxBool cxEngine::IsTouch() const
{
    return istouch;
}

cxEngine::cxEngine()
{
    isreset = false;
    timevar = 0;
    istouch = true;
    layout = false;
    pause = false;
    iter = 1;
    nextIter = iter;
    time = 0;
    dt = 0;
    init = false;
    window = cxWindow::Alloc();
    render = cxRender::Alloc();
    configs = cxHash::Alloc();
}

cxEngine::~cxEngine()
{
    configs->Release();
    render->Release();
    window->Release();
}

const cxStr *cxEngine::Config(cchars key) const
{
    return configs->Get(key)->To<cxStr>();
}

void cxEngine::LoadConfig(cchars file)
{
    const cxStr *data = cxUtil::Assets(file);
    if(!cxStr::IsOK(data)){
        return;
    }
    cxCSV *csv = cxCSV::Create(data);
    for(cxInt i=2; i < csv->Row(); i++){
        const cxStr *ks = csv->At(i, 0);
        if(!cxStr::IsOK(ks)){
            continue;
        }
        const cxStr *vs = csv->At(i, 1);
        if(!cxStr::IsOK(vs)){
            continue;
        }
        configs->Set(ks->Data(), (cxObject *)vs);
    }
}

const cxSize2F &cxEngine::WinSize() const
{
    return winsize;
}

const cxRect4F &cxEngine::WinBound() const
{
    return bound;
}

const cxBox4F &cxEngine::WinBox() const
{
    return box;
}

cxPoint2F cxEngine::FixWidth()
{
    return PlanScale().x;
}

cxPoint2F cxEngine::FixHeight()
{
    return PlanScale().y;
}

cxFloat cxEngine::PlanWidth()
{
    return plansize.w;
}

cxFloat cxEngine::PlanHeight()
{
    return plansize.h;
}

cxBool cxEngine::Contain(const cxBoxPoint3F &b)
{
    return box.Contain(b);
}

cxFloat cxEngine::ScaleFactor() const
{
    return cxUtil::Instance()->ScaleFactor();
}

void cxEngine::SetIter(cxInt aiter)
{
    CX_ASSERT(aiter > 0, "iter must > 0");
    nextIter = aiter;
}

cxInt cxEngine::Iter() const
{
    return iter;
}

void cxEngine::OnUpdate(cxFloat dt)
{
    
}

const cxPoint2F &cxEngine::PlanScale() const
{
    return planscale;
}

void cxEngine::SetPlanSize(const cxSize2F &v)
{
    CX_ASSERT(!winsize.IsZero(), "win size not set");
    plansize = v;
    planscale.x = winsize.w/plansize.w;
    planscale.y = winsize.h/plansize.h;
}

cxInt cxEngine::FPS() const
{
    if(dt == 0)return 0;
    return 1.0f/dt;
}

cxFloat cxEngine::Delta() const
{
    return dt;
}

void cxEngine::Run()
{
    cxAutoPool::Start();
    if(layout){
        CX_LOGGER("opengl screen size:W=%f H=%f",winsize.w,winsize.h);
        cxOpenGL::Instance()->Set3DProject(winsize);
        if(!init){
            cxOpenGL::Instance()->Init();
            OnMain();
            init = true;
        }
        cxOpenGL::Instance()->SetViewport(bound);
        window->SetSize(winsize);
        window->Layout();
        layout = false;
    }
    while(init){
        cxOpenGL *gl = cxOpenGL::Instance();
        cxDouble now = cxUtil::Instance()->Timestamp();
        if(time == 0){
            time = now;
        }
        dt = now - time;
        time = now;
        if(pause || cxFloatIsEqual(dt, 0)){
            break;
        }
        if(dt > DTM){
            dt = DTM;
        }
        if(iter != nextIter){
            iter = nextIter;
        }
        for(cxInt i=0;i<iter;i++){
            timevar += dt;
            OnUpdate(dt);
            window->Update(dt);
        }
        gl->Clear();
        render->Init();
        window->Render(render,gl->ModelView());
        render->Draw();
        break;
    };
    cxAutoPool::Clear();
    if(isreset){
        cxEngine::Startup(true);
    }
}

const cxFloat cxEngine::Time() const
{
    return timevar;
}

void cxEngine::Layout(cxFloat w,cxFloat h)
{
    cxSize2F csize = cxSize2F(w, h);
    if(winsize != csize){
        winsize = csize;
        layout = true;
    }
    bound = cxRect4F(0, 0, w, h);
    box = cxBox4F(-w/2.0f, w/2.0f, h/2.0f, -h/2.0f);
}

cxWindow *cxEngine::Window() const
{
    return window;
}

void cxEngine::Pause()
{
    pause = true;
    istouch = false;
    OnPause();
}

void cxEngine::Resume()
{
    pause = false;
    istouch = true;
    OnResume();
}

void cxEngine::Memory()
{
    OnMemory();
}

void cxEngine::OnMain()
{
    
}

void cxEngine::OnPause()
{
    
}

void cxEngine::OnResume()
{
    
}

void cxEngine::OnMemory()
{
    
}

CX_CPP_END

