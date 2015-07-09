//
//  cxArray.cpp
//  cxEngineCore
//
//  Created by xuhua on 5/23/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#include <vector>
#include <algorithm>
#include "cxArray.h"

CX_CPP_BEGIN

CX_IMPLEMENT(cxArray);

cxArray::cxArray()
{
    
}

cxArray::~cxArray()
{
    Clear();
}

cxBool cxArray::IsEmpty() const
{
    return mv.empty();
}

cxInt cxArray::Size() const
{
    return (cxInt)mv.size();
}

cxObject *cxArray::At(cxInt idx) const
{
    return mv.at(idx);
}

cxArray *cxArray::Clear()
{
    for(FIter it = FBegin(); it != FEnd(); it++){
        ( *it)->Release();
    }
    mv.clear();
    return this;
}

cxArray *cxArray::Append(cxObject *obj)
{
    CX_ASSERT(obj != nullptr && obj != this, "obj error");
    mv.push_back(obj);
    obj->Retain();
    return this;
}

cxObject **cxArray::Buffer()
{
    return (cxObject **)mv.data();
}

cxArray *cxArray::Replace(cxInt idx,cxObject *obj)
{
    CX_ASSERT(idx >= 0 && idx < Size(), "index out");
    const cxObject *op = At(idx);
    if(obj == op){
        return this;
    }
    cxObject **pp = Buffer();
    cxObject::swap(&pp[idx], obj);
    return this;
}

cxArray *cxArray::Remove(cxInt idx)
{
    cxObject *srcObj = At(idx);
    srcObj->Release();
    mv.erase(mv.begin() + idx);
    return this;
}

cxArray *cxArray::Swap(cxInt src,cxInt dst)
{
    cxObject **ptr = (cxObject **)mv.data();
    CX_SWAP(ptr[src], ptr[dst]);
    return this;
}

cxArray *cxArray::Remove(cxObject *obj)
{
    for(FIter it=FBegin();it!=FEnd();){
        if(*it == obj){
            it = Remove(it);
        }else{
            it++;
        }
    }
    return this;
}

cxArray *cxArray::Sort(cxCmpFunc func)
{
    qsort(Buffer(), Size(), sizeof(cxObject *), func);
    return this;
}

cxArray::RIter cxArray::Remove(RIter &iter)
{
    (*iter)->Release();
    return RIter(mv.erase(iter.base()));
}

cxArray::FIter cxArray::Remove(FIter &iter)
{
    (*iter)->Release();
    return mv.erase(iter);
}

cxArray::RIter cxArray::RBegin()
{
    return mv.rbegin();
}

cxArray::RIter cxArray::REnd()
{
    return mv.rend();
}

cxArray::FIter cxArray::FBegin()
{
    return mv.begin();
}

cxArray::FIter cxArray::FEnd()
{
    return mv.end();
}

CX_CPP_END






