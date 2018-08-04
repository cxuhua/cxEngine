//
//  cxBoxRanderF.cpp
//  cxEngineCore
//
//  Created by xuhua on 6/17/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#include "cxBoxColor4F.h"
#include "cxBoxRender.h"
#include "cxBoxPoint3F.h"

CX_CPP_BEGIN

cxBoxRender::cxBoxRender()
{
    SetCoords(cxBoxCoord2F::Default);
}

cxBoxRender::cxBoxRender(const cxBoxRender &v)
{
    lt = v.lt;
    lb = v.lb;
    rt = v.rt;
    rb = v.rb;
}

void cxBoxRender::SetColor(const cxBoxColor4F &v)
{
    lt.colors = v.lt;
    lb.colors = v.lb;
    rt.colors = v.rt;
    rb.colors = v.rb;
}

void cxBoxRender::AddColor(const cxBoxColor4F &v)
{
    
    lt.colors += v.lt;
    lb.colors += v.lb;
    rt.colors += v.rt;
    rb.colors += v.rb;
}

void cxBoxRender::MulColor(const cxBoxColor4F &v)
{
    lt.colors *= v.lt;
    lb.colors *= v.lb;
    rt.colors *= v.rt;
    rb.colors *= v.rb;
}

void cxBoxRender::SetColor(const cxColor4F &v)
{
    lt.colors = v;
    lb.colors = v;
    rt.colors = v;
    rb.colors = v;
}

void cxBoxRender::MulColor(const cxColor4F &v)
{
    lt.colors *= v;
    lb.colors *= v;
    rt.colors *= v;
    rb.colors *= v;
}

void cxBoxRender::SetAlpha(const cxFloat &v)
{
    lt.colors.a = v;
    lb.colors.a = v;
    rt.colors.a = v;
    rb.colors.a = v;
}

void cxBoxRender::SetVertices(const cxBoxPoint3F &v)
{
    lt.vertices = v.lt;
    lb.vertices = v.lb;
    rt.vertices = v.rt;
    rb.vertices = v.rb;
}

void cxBoxRender::SetCoords(const cxBoxCoord2F &v)
{
    lt.coords = v.lt;
    lb.coords = v.lb;
    rt.coords = v.rt;
    rb.coords = v.rb;
}

cxBoxRender cxBoxRender::operator*(const cxMatrixF &v)
{
    cxBoxRender rv = *this;
    rv.lt.vertices = lt.vertices * v;
    rv.lb.vertices = lb.vertices * v;
    rv.rt.vertices = rt.vertices * v;
    rv.rb.vertices = rb.vertices * v;
    return rv;
}

cxBoxPoint3F cxBoxRender::ToBoxPoint()
{
    cxBoxPoint3F v;
    v.lt = lt.vertices;
    v.lb = lb.vertices;
    v.rt = rt.vertices;
    v.rb = rb.vertices;
    return v;
}

cxBoxRender &cxBoxRender::operator*=(const cxMatrixF &v)
{
    lt.vertices *= v;
    lb.vertices *= v;
    rt.vertices *= v;
    rb.vertices *= v;
    return *this;
}

cxBoxRenderArray::cxBoxRenderArray()
{
    number = 0;
}

cxBoxRenderArray::~cxBoxRenderArray()
{
    
}

void cxBoxRenderArray::Clear()
{
    number = 0;
}

void cxBoxRenderArray::Init(cxInt n)
{
    Append(n);
    Clear();
}

void cxBoxRenderArray::Append(cxInt n)
{
    if(size() >= n){
        return;
    }
    if(size() < n){
        n = n - (cxInt)size();
    }
    for(cxInt i=0;i < n;i++){
        Append(cxBoxRender());
    }
}

cxInt cxBoxRenderArray::Inc(cxInt inc)
{
    number += inc;
    return number;
}

cxBoxRender &cxBoxRenderArray::Inc()
{
    return At(number++);
}

void cxBoxRenderArray::Append(const cxBoxRender &v)
{
    if(size() > number){
        At(number) = v;
    }else{
        push_back(v);
    }
    number ++;
}

void cxBoxRenderArray::Append(cxBoxRenderArray &v)
{
    for(cxInt i=0;i<v.Size();i++){
        Append(v.At(i));
    }
}

void cxBoxRenderArray::Remove(cxInt idx)
{
    erase(begin() + idx);
}

void cxBoxRenderArray::Remove(cxInt idx,cxInt n)
{
    erase(begin() + idx, begin() + idx + n);
}

cxBool cxBoxRenderArray::IsEmpty() const
{
    return number == 0;
}

void cxBoxRenderArray::SetAlpha(cxInt idx,cxFloat alpha)
{
    At(idx).SetAlpha(alpha);
}

void cxBoxRenderArray::SetAlpha(cxInt idx,cxInt n,cxFloat alpha)
{
    for(cxInt i=0;i < n;i++){
        At(idx + i).SetAlpha(alpha);
    }
}

void cxBoxRenderArray::SetColor(const cxColor4F &v)
{
    for(cxInt i=0;i < Size();i++){
        At(i).SetColor(v);
    }
}

void cxBoxRenderArray::MulColor(const cxColor4F &v)
{
    for(cxInt i=0;i < Size();i++){
        At(i).MulColor(v);
    }
}

void cxBoxRenderArray::MulColor(const cxBoxColor4F &v)
{
    for(cxInt i=0;i < Size();i++){
        At(i).MulColor(v);
    }
}

const cxInt cxBoxRenderArray::Capacity() const
{
    return (cxInt)size();
}

const cxInt cxBoxRenderArray::Size() const
{
    return (cxInt)number;
}

cxInt cxBoxRenderArray::DataBytes() const
{
    return Size() * sizeof(cxBoxRender);
}

cxInt cxBoxRenderArray::BufferBytes() const
{
    return Capacity() * sizeof(cxBoxRender);
}

const cxBoxRender *cxBoxRenderArray::Buffer() const
{
    return data();
}

cxBoxRender &cxBoxRenderArray::At(cxInt idx)
{
    return at(idx);
}

const cxBoxRender &cxBoxRenderArray::At(cxInt idx) const
{
    return at(idx);
}

const cxBoxRender& cxBoxRenderArray::operator [](cxInt idx) const
{
    return at(idx);
}

cxBoxRender& cxBoxRenderArray::operator [](cxInt idx)
{
    return at(idx);
}


CX_CPP_END

