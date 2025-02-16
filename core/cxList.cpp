//
//  cxList.cpp
//  cxEngineCore
//
//  Created by xuhua on 6/21/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//


#include "cxList.h"

CX_CPP_BEGIN

CX_IMPLEMENT(cxList);

cxList::cxList()
{
    
}

cxList::~cxList()
{
    Clear();
}

cxObject *cxList::At(cxInt idx)
{
    CX_ASSERT(idx >= 0 && idx < Size(), "idx range out");
    cxInt i = 0;
    for(FIter it=FBegin();it!=FEnd();it++){
        if(i == idx){
            return *it;
        }
        i++;
    }
    return nullptr;
}

cxBool cxList::IsOK(const cxList *v)
{
    return v != nullptr && !v->IsEmpty();
}

cxBool cxList::IsEmpty() const
{
    return ml.empty();
}

cxInt cxList::Size() const
{
    return (cxInt)ml.size();
}

cxObject *cxList::Front() const
{
    return IsEmpty()?nullptr:ml.front();
}

cxObject *cxList::Back() const
{
    return IsEmpty()?nullptr:ml.back();
}

cxList *cxList::Append(cxObject *obj)
{
    ml.push_back(obj);
    obj->Retain();
    return this;
}

cxList *cxList::Prepend(cxObject *obj)
{
    ml.push_front(obj);
    obj->Retain();
    return this;
}

cxList *cxList::Clear()
{
    for(FIter it=FBegin();it!=FEnd();it++){
        (*it)->Release();
    }
    ml.clear();
    return this;
}

cxList *cxList::PopFront()
{
    if(IsEmpty()){
        return this;
    }
    Front()->Release();
    ml.pop_front();
    return this;
}

cxList *cxList::PopBack()
{
    if(IsEmpty()){
        return this;
    }
    Back()->Release();
    ml.pop_back();
    return this;
}

cxList *cxList::Remove(cxObject *obj)
{
    for(FIter it=FBegin();it!=FEnd();){
        if(obj == *it){
            it = Remove(it);
        }else{
            it++;
        }
    }
    return this;
}

cxList::RIter cxList::RBegin()
{
    return ml.rbegin();
}

cxList::CRIter cxList::RBegin() const
{
    return ml.rbegin();
}

cxList::RIter cxList::REnd()
{
    return ml.rend();
}

cxList::CRIter cxList::REnd() const
{
    return ml.rend();
}

cxList::FIter cxList::FBegin()
{
    return ml.begin();
}

cxList::CFIter cxList::FBegin() const
{
    return ml.begin();
}

cxList::FIter cxList::FEnd()
{
    return ml.end();
}

cxList::CFIter cxList::FEnd() const
{
    return ml.end();
}

cxList::FIter cxList::Remove(FIter &iter)
{
    (*iter)->Release();
    return ml.erase(iter);
}

cxList::CFIter cxList::Remove(CFIter &iter)
{
    (*iter)->Release();
    return ml.erase(iter);
}

cxList::RIter cxList::Remove(RIter &iter)
{
    (*iter)->Release();
    return RIter(ml.erase(iter.base()));
}

cxList::CRIter cxList::Remove(CRIter &iter)
{
    (*iter)->Release();
    return CRIter(ml.erase(iter.base()));
}

CX_CPP_END

