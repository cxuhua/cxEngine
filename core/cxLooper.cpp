//
//  cxLooper.cpp
//  cxEngineCore
//
//  Created by 徐华 on 2019/6/13.
//  Copyright © 2019 xuhua. All rights reserved.
//

#include "cxCore.h"
#include "cxLooper.h"

CX_CPP_BEGIN

CX_IMPLEMENT(cxLooper)

void cxLooper::uv_async_runcb(uv_async_t* handle)
{
    cxLooper *self = static_cast<cxLooper *>(handle->data);
    uv_mutex_lock(&self->mutex);
    while(!self->cbs.empty()){
        self->cbs.front()();
        self->cbs.pop();
    }
    uv_mutex_unlock(&self->mutex);
}

cxLooper::cxLooper()
{
    uv_mutex_init(&mutex);
    stacks = cxStack::Alloc();
    uv_loop_init(&looper);
    async.data = this;
    uv_async_init(&looper, &async, cxLooper::uv_async_runcb);
}

cxLooper::~cxLooper()
{
    uv_mutex_destroy(&mutex);
    stacks->Release();
    uv_loop_close(&looper);
}

cxLooper *cxLooper::Looper()
{
    cxLooper *l = (cxLooper *)cxCore::Instance()->GetLooper();
    if(l == nullptr){
        l = cxLooper::Alloc();
        l->self = uv_thread_self();
        cxCore::Instance()->SetLooper(l);
    }
    return l;
}

cxBool cxLooper::isCurrThread()
{
    uv_thread_t c = uv_thread_self();
    return uv_thread_equal(&self, &c) != 0;
}

void cxLooper::Run(cxBool wait)
{
    uv_run(&looper, wait?UV_RUN_DEFAULT:UV_RUN_NOWAIT);
}

void cxLooper::Async(CB cb)
{
    if(isCurrThread()){
        cb();
        return;
    }
    uv_mutex_lock(&mutex);
    cbs.push(cb);
    uv_mutex_unlock(&mutex);
    uv_async_send(&async);
}

cxArray *cxLooper::GetTop()
{
    cxArray *top = stacks->Top()->To<cxArray>();
    if(top == nullptr){
        top = cxArray::Alloc();
        stacks->Push(top);
        top->Release();
    }
    return top;
}

cxObject *cxLooper::Append(cxObject *object)
{
    cxArray *top = GetTop();
    top->Append(object);
    CX_ASSERT(object->Refcount() > 1, "append to auto pool object,refcount > 1");
    object->Release();
    return object;
}

void cxLooper::Clear()
{
    stacks->Clear();
}

cxInt cxLooper::Size()
{
    cxArray *top = GetTop();
    return top->Size();
}

void cxLooper::Update()
{
    cxArray *top = GetTop();
    top->Clear();
}

void cxLooper::Push()
{
    cxArray *top = cxArray::Alloc();
    stacks->Push(top);
    top->Release();
}

void cxLooper::Pop()
{
    stacks->Pop();
}

CX_CPP_END
