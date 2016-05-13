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

cchars cxHashKey::Key() const
{
    return (cchars)data;
}

cxHashKey::cxHashKey()
{
    length = 0;
}

cxHashKey::cxHashKey(const cxStr *key)
{
    length = key->Size();
    CX_ASSERT(length < CX_HASH_MAX_KEY, "key too longer");
    memcpy(data, key->Data(), length);
    data[length] = 0;
}

cxHashKey::cxHashKey(cxInt key)
{
    length = sizeof(cxInt);
    memcpy(data, &key, length);
    data[length] = 0;
}

cxHashKey::cxHashKey(cchars key)
{
    length = (cxInt)strlen(key);
    memcpy(data, key, length);
    data[length] = 0;
}

cxHashKey::cxHashKey(cxUInt64 key)
{
    length = sizeof(cxUInt64);
    memcpy(data, &key, length);
    data[length] = 0;
}

cxHashKey::cxHashKey(cxUInt32 key)
{
    length = sizeof(cxUInt32);
    memcpy(data, &key, length);
    data[length] = 0;
}

cxHashKey cxHashKey::Format(cchars fmt,va_list ap)
{
    cxHashKey key;
    key.length = vsnprintf((char *)key.data, CX_HASH_MAX_KEY, fmt, ap);
    key.data[key.length] = 0;
    return key;
}

cxHashKey cxHashKey::Format(cchars fmt,...)
{
    va_list ap;
    va_start(ap, fmt);
    cxHashKey key = Format(fmt, ap);
    va_end(ap);
    return key;
}

bool cxHasher::operator()(const cxHashKey &lhs, const cxHashKey &rhs) const
{
    if(lhs.length != rhs.length){
        return false;
    }
    return memcmp(lhs.data, rhs.data, lhs.length) == 0;
}

size_t cxHasher::operator()(const cxHashKey &k) const
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

cxHash *cxHash::Set(const cxHashKey &key,cxObject *nobj)
{
    CX_ASSERT(nobj != nullptr && nobj != this, "obj error");
    cxObject *oobj = Get(key);
    if(oobj == nobj){
        return this;
    }
    if(oobj != nullptr){
        Del(key);
    }
    mh.emplace(key,nobj);
    nobj->Retain();
    return this;
}

CX_CPP_END

