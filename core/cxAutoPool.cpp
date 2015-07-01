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
    cxStack *pool = static_cast<cxStack *>(cxCore::Instance()->GetAutoPool());
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
        cxAutoPool *pool = new cxAutoPool();
        pools->Push(pool);
        pool->Release();
    }
    return static_cast<cxAutoPool *>(pools->Top());
}

cxAutoPool::cxAutoPool()
{
    objects = cxArray::Alloc();
}

cxAutoPool::~cxAutoPool()
{
    objects->Release();
}

cxObject *cxAutoPool::Append(cxObject *item)
{
    CX_ASSERT(item != nullptr, "item null");
    cxAutoPool *pool = getTopAutoPool();
    pool->objects->Append(item);
    CX_ASSERT(item->Refcount() > 1, "append to auto pool object,refcount > 1");
    item->Release();
    return item;
}

void cxAutoPool::Start()
{
    getTopAutoPool();
}

void cxAutoPool::Clear()
{
    cxAutoPool *pool = getTopAutoPool();
    pool->objects->Clear();
}

void cxAutoPool::Push()
{
    cxStack *pools = getThreadPoolStack();
    cxAutoPool *pool = new cxAutoPool();
    pools->Push(pool);
    pool->Release();
}

void cxAutoPool::Pop()
{
    cxStack *pools = getThreadPoolStack();
    pools->Pop();
}

CX_CPP_END











