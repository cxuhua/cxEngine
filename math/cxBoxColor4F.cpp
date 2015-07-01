//
//  cxBoxColor4F.cpp
//  cxEngineCore
//
//  Created by xuhua on 6/17/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//


#include "cxBoxColor4F.h"

CX_CPP_BEGIN

cxBoxColor4F::cxBoxColor4F()
{
    lt = cxColor4F::WHITE;
    lb = cxColor4F::WHITE;
    rt = cxColor4F::WHITE;
    rb = cxColor4F::WHITE;
}

cxBoxColor4F::cxBoxColor4F(const cxColor4F &v)
{
    lt = v;
    lb = v;
    rt = v;
    rb = v;
}

cxBoxColor4F::cxBoxColor4F(const cxBoxColor4F &v)
{
    lt = v.lt;
    lb = v.lb;
    rt = v.rt;
    rb = v.rb;
}

const cxBoxColor4F cxBoxColor4F::ToLoop() const
{
    cxBoxColor4F ret;
    ret.lt = lt;
    ret.lb = lb;
    ret.rt = rb;
    ret.rb = rt;
    return ret;
}

void cxBoxColor4F::SetAlpha(cxFloat alpha)
{
    lt.SetAlpha(alpha);
    lb.SetAlpha(alpha);
    rt.SetAlpha(alpha);
    rb.SetAlpha(alpha);
}

void cxBoxColor4F::SetColor(const cxColor4F &color)
{
    lt = color;
    lb = color;
    rt = color;
    rb = color;
}

cxBool cxBoxColor4F::operator==(const cxColor4F &v) const
{
    return lt == v;
}

cxBool cxBoxColor4F::operator!=(const cxColor4F &v) const
{
    return lt != v;
}

const cxColor4F &cxBoxColor4F::Color() const
{
    return lt;
}

cxBoxColor4FArray::cxBoxColor4FArray()
{
    
}

cxBoxColor4FArray::~cxBoxColor4FArray()
{
    
}

void cxBoxColor4FArray::Append(cxInt n)
{
    for(cxInt i=0;i<n;i++){
        push_back(cxBoxColor4F());
    }
}

void cxBoxColor4FArray::Append(cxInt n,const cxColor4F &v)
{
    for(cxInt i=0;i<n;i++){
        push_back(cxBoxColor4F(v));
    }
}

void cxBoxColor4FArray::Append(const cxBoxColor4F &v)
{
    push_back(v);
}

void cxBoxColor4FArray::Remove(cxInt idx)
{
    erase(begin() + idx);
}

void cxBoxColor4FArray::Remove(cxInt idx,cxInt n)
{
    erase(begin() + idx,begin() + idx + n);
}

void cxBoxColor4FArray::SetAlpha(cxInt idx,cxFloat alpha)
{
    At(idx).SetAlpha(alpha);
}

void cxBoxColor4FArray::SetAlpha(cxInt idx,cxInt n,cxFloat alpha)
{
    for(cxInt i=0;i<n;i++){
        SetAlpha(i, alpha);
    }
}

void cxBoxColor4FArray::SetColor(cxInt idx,const cxColor4F &color)
{
    At(idx).SetColor(color);
}

const cxBoxColor4F *cxBoxColor4FArray::Buffer() const
{
    return data();
}

cxBoxColor4F &cxBoxColor4FArray::At(cxInt idx)
{
    return at(idx);
}

const cxInt cxBoxColor4FArray::Size() const
{
    return (cxInt)size();
}

cxBool cxBoxColor4FArray::IsEmpty() const
{
    return empty();
}

CX_CPP_END

