//
//  cxUnique.cpp
//  cxEngineCore
//
//  Created by xuhua on 6/22/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//


#include "cxArray.h"
#include "cxUnique.h"

CX_CPP_BEGIN

CX_IMPLEMENT(cxUnique);

cxUnique::cxUnique()
{
    
}

cxUnique::~cxUnique()
{
    
}

cxInt cxUnique::Size() const
{
    return (cxInt)mm.size();
}

cxBool cxUnique::IsEmpty() const
{
    return mm.empty();
}

cxArray *cxUnique::ToArray()
{
    cxArray *ret = cxArray::Create();
    for(Iter it=mm.begin();it!=mm.end();it++){
        ret->Append(it->second);
    }
    return ret;
}

cxUnique *cxUnique::Append(cxObject *obj)
{
    cxULong key = obj->UniqueId();
    cxUnique::Iter it = mm.find(key);
    if(it != End()){
        return this;
    }
    mm.insert(std::pair<cxULong, cxObject *>(key,obj));
    obj->Retain();
    return this;
}

cxUnique *cxUnique::Remove(cxObject *obj)
{
    cxULong key = obj->UniqueId();
    cxUnique::Iter it = mm.find(key);
    if(it == End()){
        return this;
    }
    Remove(it);
    return this;
}

cxUnique *cxUnique::Clear()
{
    for(cxUnique::Iter it=Begin();it!=End();it++){
        it->second->Release();
    }
    mm.clear();
    return this;
}

cxUnique::Iter cxUnique::Remove(const Iter &iter)
{
    iter->second->Release();
    return mm.erase(iter);
}

const cxUnique::Iter cxUnique::Begin() const
{
    return mm.begin();
}

const cxUnique::Iter cxUnique::End() const
{
    return mm.end();
}

CX_CPP_END

