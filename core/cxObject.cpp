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
    UnBind();
}

cxObject::cxObject():refcount(1),tag(0)
{
    
}


const cxInt cxObject::BindesSize() const
{
    return bindes.empty()?0:(cxInt)bindes.size();
}

const cxInt cxObject::BindedSize() const
{
    return binded.empty()?0:(cxInt)binded.size();
}

void cxObject::Bind(cxObject *pobj,cxLong tag)
{
    CX_ASSERT(pobj != nullptr, "pobj args error");
    CX_ASSERT(pobj != this, "self can't bind self");
    if(bindes.find(pobj) == bindes.end()){
        bindes.emplace(pobj,tag);
    }
    if(pobj->binded.find(this) == pobj->binded.end()){
        pobj->binded.emplace(this,tag);
    }
}

const cxBool cxObject::HasBindes(cxObject *pobj) const
{
    return bindes.find(pobj) != bindes.end();
}

const cxBool cxObject::HasBinded(cxObject *pobj) const
{
    return binded.find(pobj) != binded.end();
}

const cxArray *cxObject::GetBindes()
{
    cxArray *objects = cxArray::Create();
    if(bindes.empty()){
        return objects;
    }
    BindMap::iterator it = bindes.begin();
    while(it != bindes.end()){
        cxObject *pobj = (cxObject *)it->first;
        objects->Append(pobj);
        it++;
    }
    return objects;
}

cxObject *cxObject::GetBindes(cxLong tag)
{
    if(bindes.empty()){
        return nullptr;
    }
    BindMap::iterator it = bindes.begin();
    while(it != bindes.end()){
        if(it->second == tag){
            return (cxObject *)it->first;
        }
        it++;
    }
    return nullptr;
}

const cxArray *cxObject::GetBinded()
{
    cxArray *objects = cxArray::Create();
    if(binded.empty()){
        return objects;
    }
    BindMap::iterator it = binded.begin();
    while(it != binded.end()){
        cxObject *pobj = (cxObject *)it->first;
        objects->Append(pobj);
        it++;
    }
    return objects;
}

cxObject *cxObject::GetBinded(cxLong tag)
{
    if(binded.empty()){
        return nullptr;
    }
    BindMap::iterator it = binded.begin();
    while(it != binded.end()){
        if(it->second == tag){
            return (cxObject *)it->first;
        }
        it++;
    }
    return nullptr;
}

void cxObject::UnBind(cxObject *pobj)
{
    if(pobj == nullptr){
        return;
    }
    bindes.erase(pobj);
    pobj->binded.erase(this);
}

void cxObject::UnBind()
{
    BindMap::iterator it = bindes.begin();
    while(it != bindes.end()){
        cxObject *pobj = (cxObject *)it->first;
        pobj->binded.erase(this);
        it++;
    }
    bindes.clear();
    
    it = binded.begin();
    while(it != binded.end()){
        cxObject *pobj = (cxObject *)it->first;
        pobj->bindes.erase(this);
        it++;
    }
    binded.clear();
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

cxObject *cxObject::create(const cxStr *name)
{
    return create(name->ToString());
}

cxObject *cxObject::fromJson(const cxJson *json)
{
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











