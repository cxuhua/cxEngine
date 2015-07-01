//
//  cxHash.cpp
//  cxEngineCore
//
//  Created by xuhua on 5/27/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#include "cxArray.h"
#include "cxHash.h"

CX_CPP_BEGIN

cxHashKey::cxHashKey()
{
    length = 0;
}

cxHashKey::cxHashKey(const cxStr *key)
{
    length = key->Size();
    memcpy(data, key->Data(), length);
}

cxHashKey::cxHashKey(cxInt key)
{
    length = sizeof(cxInt);
    memcpy(data, &key, length);
}

cxHashKey::cxHashKey(cchars key)
{
    length = (cxInt)strlen(key);
    memcpy(data, key, length);
}

cxHashKey::cxHashKey(cxULong key)
{
    length = sizeof(cxULong);
    memcpy(data, &key, length);
}

bool cxHasher::operator()(const cxHashKey& lhs, const cxHashKey& rhs) const
{
    if(lhs.length != rhs.length){
        return false;
    }
    return memcmp(lhs.data, rhs.data, lhs.length) == 0;
}

size_t cxHasher::operator()(const cxHashKey& k) const
{
    return (size_t)XXH32(k.data, k.length, 0);
}

CX_IMPLEMENT(cxHash);

cxHash::cxHash()
{
    
}

cxHash::~cxHash()
{
    Clear();
}

cxInt cxHash::Size() const
{
    return (cxInt)mh.size();
}

cxBool cxHash::IsEmpty() const
{
    return mh.empty();
}

cxHash *cxHash::Clear()
{
    for(Iter it=Begin();it!=End();it++){
        it->second->Release();
    }
    mh.clear();
    return this;
}

cxArray *cxHash::ToArray()
{
    cxArray *rv = cxArray::Create();
    for(Iter it=Begin();it!=End();it++){
        rv->Append(it->second);
    }
    return rv;
}

cxHash::Iter cxHash::Remove(Iter &iter)
{
    return mh.erase(iter);
}

cxHash::Iter cxHash::Begin()
{
    return mh.begin();
}

cxHash::Iter cxHash::End()
{
    return mh.end();
}

void cxHash::Del(const cxHashKey &key)
{
    Iter it = mh.find(key);
    if(it == mh.end()){
        return;
    }
    it->second->Release();
    mh.erase(it);
}

cxBool cxHash::Has(const cxHashKey &key)
{
    return !IsEmpty() && mh.find(key) != mh.end();
}

cxObject *cxHash::Get(const cxHashKey &key)
{
    Iter it = mh.find(key);
    if(it == mh.end()){
        return nullptr;
    }
    return it->second;
}

cxHash *cxHash::Set(const cxHashKey &key,cxObject *obj)
{
    CX_ASSERT(obj != nullptr && obj != this, "obj error");
    std::pair<Iter, bool> ret = mh.emplace(key,obj);
    if(ret.second){
        obj->Retain();
        return this;
    }
    cxObject *pobj = ret.first->second;
    if(pobj  == obj){
        return this;
    }
    pobj->Release();
    ret = mh.emplace(key,obj);
    if(ret.second){
        obj->Retain();
    }
    return this;
}

CX_CPP_END

