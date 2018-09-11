//
//  cxArray.cpp
//  cxEngineCore
//
//  Created by xuhua on 5/23/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

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

void cxArray::Move(cxInt idx,cxArray *src,cxArray *dst)
{
    CX_ASSERT(src != nullptr && dst != nullptr, "src or dst args error");
    CX_ASSERT(idx < src->Size(), "idx out bound");
    cxObject *obj = src->At(idx);
    dst->Append(obj);
    src->Remove(idx);
}

cxBool cxArray::IsEmpty() const
{
    return mv.empty();
}

cxInt cxArray::Size() const
{
    return (cxInt)mv.size();
}

cxObject *cxArray::Back() const
{
    return IsEmpty()?nullptr:mv.back();
}

cxObject *cxArray::Front() const
{
    return IsEmpty()?nullptr:mv.front();
}

cxObject *cxArray::At(cxInt idx) const
{
    CX_ASSERT(idx >= 0 && idx < Size(), "idx out bound");
    return mv.at(idx);
}

cxBool cxArray::IsOK(const cxArray *v)
{
    return v != nullptr && !v->IsEmpty();
}

cxArray *cxArray::Clear()
{
    for(FIter it = FBegin(); it != FEnd(); it++){
        (*it)->Release();
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

cxArray *cxArray::AppendArray(const cxArray *vs)
{
    CX_ASSERT(vs != nullptr, "vs args null");
    if(vs->IsEmpty()){
        return this;
    }
    for(cxArray::CFIter it=vs->FBegin();it!=vs->FEnd();it++){
        Append(*it);
    }
    return this;
}

void cxArray::Elements(std::function<void(cxObject *)> func)
{
    for(cxArray::CFIter it=FBegin();it!=FEnd();it++){
        func(*it);
    }
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
    CX_ASSERT(idx >= 0 && idx < Size(), "idx out bound");
    cxObject *srcObj = At(idx);
    srcObj->Release();
    mv.erase(mv.begin() + idx);
    return this;
}

cxArray *cxArray::Swap(cxInt src,cxInt dst)
{
    CX_ASSERT(src >= 0 && src < Size(), "src out bound");
    CX_ASSERT(dst >= 0 && dst < Size(), "dst out bound");
    cxObject **ptr = Buffer();
    CX_SWAP_VAR(ptr[src], ptr[dst]);
    return this;
}

cxArray *cxArray::Remove(cxObject *obj)
{
    if(obj == nullptr){
        return this;
    }
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

cxArray::CRIter cxArray::Remove(CRIter &iter)
{
    (*iter)->Release();
    return CRIter(mv.erase(iter.base()));
}

cxArray::FIter cxArray::Remove(FIter &iter)
{
    (*iter)->Release();
    return mv.erase(iter);
}

cxArray::CFIter cxArray::Remove(CFIter &iter)
{
    (*iter)->Release();
    return mv.erase(iter);
}

cxArray::RIter cxArray::RBegin()
{
    return mv.rbegin();
}

cxArray::CRIter cxArray::RBegin() const
{
    return mv.rbegin();
}

cxArray::RIter cxArray::REnd()
{
    return mv.rend();
}

cxArray::CRIter cxArray::REnd() const
{
    return mv.rend();
}

cxArray::CFIter cxArray::FBegin() const
{
    return mv.begin();
}

cxArray::FIter cxArray::FBegin()
{
    return mv.begin();
}

cxArray::FIter cxArray::FEnd()
{
    return mv.end();
}

cxArray::CFIter cxArray::FEnd() const
{
    return mv.end();
}

CX_CPP_END






