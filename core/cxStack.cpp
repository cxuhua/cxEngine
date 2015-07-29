//
//  cxStack.cpp
//  cxEngineCore
//
//  Created by xuhua on 6/19/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//


#include "cxStack.h"

CX_CPP_BEGIN

CX_IMPLEMENT(cxStack);

cxStack::cxStack()
{
    
}

cxStack::~cxStack()
{
    Clear();
}

cxStack *cxStack::Clear()
{
    while(!IsEmpty()){
        Pop();
    }
    return this;
}

cxBool cxStack::IsEmpty() const
{
    return ms.empty();
}

cxStack *cxStack::Push(cxObject *obj)
{
    CX_ASSERT(obj != nullptr, "args error");
    ms.push(obj);
    obj->Retain();
    return this;
}

cxStack *cxStack::Pop()
{
    if(IsEmpty()){
        return this;
    }
    ms.top()->Release();
    ms.pop();
    return this;
}

cxObject *cxStack::Top()
{
    if(IsEmpty()){
        return nullptr;
    }
    return ms.top();
}

CX_CPP_END

