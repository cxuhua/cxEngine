//
//  cxEngine.cpp
//  cxEngineCore
//
//  Created by xuhua on 6/11/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

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

void cxEngine::Destroy()
{
    cxObject::release(&instance);
}

cxEngine *cxEngine::Instance()
{
    CX_ASSERT(instance != nullptr, "instance not init,use cxEngine::Init");
    return instance;
}

void cxEngine::Init(cxEngine *engine)
{
    CX_ASSERT(instance == nullptr, "instande repear set");
    instance = static_cast<cxEngine *>(engine);
}

cxBool cxEngine::IsTouch() const
{
    return istouch;
}

cxEngine::cxEngine()
{
    istouch = true;
    layout = false;
    pause = false;
    iter = 1;
    nextIter = iter;
    time = 0;
    dt = 0;
    init = false;
    uv_loop_init(&looper);
    window = cxWindow::Alloc();
    render = cxRender::Alloc();
}

cxEngine::~cxEngine()
{
    render->Release();
    window->Release();
    uv_loop_close(&looper);
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

cxBool cxEngine::Contain(const cxBoxPoint3F &b)
{
    if(box.Contain(b.lt)){
        return true;
    }
    if(box.Contain(b.lb)){
        return true;
    }
    if(box.Contain(b.rt)){
        return true;
    }
    if(box.Contain(b.rb)){
        return true;
    }
    cxBox4F vb = b.ToMaxBox();
    if(vb.t >= box.t && vb.b <= box.b){
        return true;
    }
    if(vb.l <= box.l && vb.r >= box.r){
        return true;
    }
    return false;
}

cxFloat cxEngine::ScaleFactor() const
{
    return cxUtil::Instance()->ScaleFactor();
}

void cxEngine::SetIter(cxInt aiter)
{
    CX_ASSERT(aiter > 0, "iter must >= 1");
    nextIter = aiter;
}

cxInt cxEngine::Iter() const
{
    return iter;
}

uv_loop_t *cxEngine::Looper()
{
    return &looper;
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
            #ifndef NDEBUG
            Window()->InitDebugLabel();
            #endif
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
            uv_run(Looper(), UV_RUN_NOWAIT);
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

