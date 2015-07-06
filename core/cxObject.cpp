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

cxJson *cxObject::Serialize()
{
    return nullptr;
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

cxObject *cxObject::initFromJson(const cxJson *json)
{
    cxJson::Iter it = json->Begin();
    while(it != json->End()){
        cxJson *value = cxJson::Alloc();
        if(it.Value(value)){
            SetProperty(it.Key(), value);
        }
        value->Release();
        it++;
    }
    return this;
}

void cxObject::SetProperty(cchars key,const cxJson *json)
{
    if(cxStr::IsEqu(key, "tag")){
        SetTag(json->ToInt());
    }
}

const cxJson *cxObject::GetProperty(cchars key)
{
    if(cxStr::IsEqu(key, "tag")){
        return cxJson::Create()->From((cxInt)Tag());
    }
    return nullptr;
}

cxObject *cxObject::alloc(cchars name)
{
    return cxCore::alloc(name);
}

cxObject *cxObject::create(const cxStr *data)
{
    cxJson *json = cxJson::Create()->From(data);
    if(json == nullptr){
        CX_ERROR("json data error");
        return nullptr;
    }
    const cxJson *cxtype = json->At("cxType");
    if(cxtype == nullptr || !cxtype->IsString()){
        CX_ERROR("cxType node miss");
        return nullptr;
    }
    cxObject *pobj = cxObject::create(cxtype->ToString());
    if(pobj == nullptr){
        CX_ERROR("create object error");
        return nullptr;
    }
    return pobj->initFromJson(json);
}

cxObject *cxObject::create(cchars name)
{
    return alloc(name)->AutoRelease();
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











