//
//  cxQueue.cpp
//  cxEngineCore
//
//  Created by xuhua on 6/21/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//


#include "cxQueue.h"

CX_CPP_BEGIN

CX_IMPLEMENT(cxQueue);

cxQueue::cxQueue()
{
    
}

cxQueue::~cxQueue()
{
    Clear();
}

cxQueue *cxQueue::Clear()
{
    while (!IsEmpty()) {
        Pop();
    }
    return this;
}

cxInt cxQueue::Size() const
{
    return (cxInt)mq.size();
}

cxQueue *cxQueue::Push(cxObject *obj)
{
    mq.push(obj);
    obj->Retain();
    return this;
}

cxBool cxQueue::IsEmpty() const
{
    return mq.empty();
}

cxQueue *cxQueue::Pop()
{
    if(IsEmpty()){
        return this;
    }
    Front()->Release();
    mq.pop();
    return this;
}

cxObject *cxQueue::Front() const
{
    return IsEmpty()?nullptr:mq.front();
}

cxObject *cxQueue::Back() const
{
    return IsEmpty()?nullptr:mq.back();
}

CX_CPP_END

