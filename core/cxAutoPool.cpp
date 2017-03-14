//
//  cxAutoPool.cpp
//  cxEngineCore
//
//  Created by xuhua on 5/23/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#include "cxAutoPool.h"

CX_CPP_BEGIN

CX_IMPLEMENT(cxAutoPool);

cxStack *cxAutoPool::getThreadPoolStack()
{
    cxStack *pool = cxCore::Instance()->GetAutoPool();
    if(pool == nullptr){
        pool = cxStack::Alloc();
        cxCore::Instance()->SetAutoPool(pool);
    }
    return pool;
}

cxAutoPool *cxAutoPool::getTopAutoPool()
{
    cxStack *pools = getThreadPoolStack();
    if(pools->IsEmpty()){
        cxAutoPool *pool = cxAutoPool::Alloc();
        pools->Push(pool);
        pool->Release();
    }
    return pools->Top()->To<cxAutoPool>();
}

cxAutoPool::cxAutoPool()
{
    objects = cxArray::Alloc();
}

cxAutoPool::~cxAutoPool()
{
    objects->Release();
}

cxObject *cxAutoPool::Append(cxObject *object)
{
    CX_ASSERT(object != nullptr, "item null");
    cxAutoPool *pool = getTopAutoPool();
    pool->objects->Append(object);
    CX_ASSERT(object->Refcount() > 1, "append to auto pool object,refcount > 1");
    object->Release();
    return object;
}

cxAutoPool *cxAutoPool::Start()
{
    return getTopAutoPool();
}

void cxAutoPool::Clear()
{
    cxStack *pools = getThreadPoolStack();
    CX_ASSERT(pools != nullptr, "pool stack null");
    pools->Clear();
}

cxInt cxAutoPool::Size()
{
    cxAutoPool *pool = getTopAutoPool();
    return pool->objects->Size();
}

void cxAutoPool::Update()
{
    cxAutoPool *pool = getTopAutoPool();
    pool->objects->Clear();
}

void cxAutoPool::Push()
{
    cxStack *pools = getThreadPoolStack();
    cxAutoPool *pool = cxAutoPool::Alloc();
    pools->Push(pool);
    pool->Release();
}

void cxAutoPool::Pop()
{
    cxStack *pools = getThreadPoolStack();
    pools->Pop();
}

CX_CPP_END











