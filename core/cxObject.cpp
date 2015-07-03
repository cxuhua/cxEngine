//
//  cxObject.cpp
//  cxEngineCore
//
//  Created by xuhua on 5/24/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#include "cxStr.h"
#include "cxJson.h"
#include "cxObject.h"
#include "cxAutoPool.h"
#include "cxNotice.h"

CX_CPP_BEGIN

CX_IMPLEMENT(cxObject);

cxObject::~cxObject()
{
    
}

cxObject::cxObject():refcount(1),tag(0)
{
    
}

cxLong cxObject::Tag() const
{
    return tag;
}

cxULong cxObject::Hash() const
{
    return (cxULong)this;
}

void cxObject::SetTag(cxLong value)
{
    tag = value;
}

void cxObject::Retain()
{
    CX_ASSERT(refcount > 0, "error refcount retain > 0");
    cxAtomicAddInt32(&refcount, 1);
}

cxInt cxObject::Refcount() const
{
    return refcount;
}

cxObject *cxObject::AutoRelease()
{
    return cxAutoPool::Append(this);
}

void cxObject::Release()
{
    CX_ASSERT(refcount > 0, "error,retain count must > 0");
    cxAtomicSubInt32(&refcount, 1);
    if(refcount == 0){
        delete this;
    }
}

CX_CPP_END











