//
//  cxRenderF.cpp
//  cxEngineCore
//
//  Created by xuhua on 6/17/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#include "cxBoxRender.h"
#include "cxRenderF.h"

CX_CPP_BEGIN

cxRenderF::cxRenderF()
{
    colors = cxColor4F::WHITE;
}

cxRenderF::cxRenderF(const cxRenderF &v)
{
    vertices = v.vertices;
    colors = v.colors;
    coords = v.coords;
}

cxRenderF &cxRenderF::operator*=(const cxMatrixF &v)
{
    vertices *= v;
    return *this;
}

cxRenderF cxRenderF::operator*(const cxMatrixF &v)
{
    cxRenderF rv = *this;
    rv.vertices *= v;
    return rv;
}

cxRenderFArray::cxRenderFArray()
{
    number = 0;
}

cxRenderFArray::~cxRenderFArray()
{
    
}

void cxRenderFArray::Clear()
{
    number = 0;
}

void cxRenderFArray::Init(cxInt n)
{
    Append(n);
    Clear();
}

void cxRenderFArray::Append(cxInt n)
{
    if(size() >= n){
        return;
    }
    if(size() < n){
        n = n - (cxInt)size();
    }
    for(cxInt i=0;i < n;i++){
        Append(cxRenderF());
    }
}

cxInt cxRenderFArray::Inc(cxInt inc)
{
    number += inc;
    return number;
}

void cxRenderFArray::MulColor(const cxColor4F &v)
{
    for(cxInt i=0;i<Size();i++){
        At(i).colors *= v;
    }
}

cxRenderF &cxRenderFArray::Inc()
{
    return At(number++);
}

void cxRenderFArray::Append(const cxRenderF &v)
{
    if(size() > number){
        At(number) = v;
    }else{
        push_back(v);
    }
    number ++;
}

void cxRenderFArray::Append(const cxBoxRender &v)
{
    Append(v.lt);
    Append(v.lb);
    Append(v.rt);
    Append(v.rb);
}

void cxRenderFArray::Append(const cxBoxRenderArray &v)
{
    for(cxInt i=0;i<v.Size();i++){
        Append(v.At(i));
    }
}

void cxRenderFArray::Append(const cxRenderFArray &v)
{
    for(cxInt i=0;i<v.Size();i++){
        Append(v.At(i));
    }
}

void cxRenderFArray::Remove(cxInt idx)
{
    erase(begin() + idx);
}

void cxRenderFArray::Remove(cxInt idx,cxInt n)
{
    erase(begin() + idx, begin() + idx + n);
}

cxBool cxRenderFArray::IsEmpty() const
{
    return number == 0;
}

const cxInt cxRenderFArray::Capacity() const
{
    return (cxInt)size();
}

const cxInt cxRenderFArray::Size() const
{
    return (cxInt)number;
}

const cxRenderF *cxRenderFArray::Buffer() const
{
    return data();
}

cxInt cxRenderFArray::DataBytes() const
{
    return Size() * sizeof(cxRenderF);
}

cxInt cxRenderFArray::BufferBytes() const
{
    return Capacity() * sizeof(cxRenderF);
}

cxRenderF &cxRenderFArray::At(cxInt idx)
{
    return at(idx);
}

const cxRenderF &cxRenderFArray::At(cxInt idx) const
{
    return at(idx);
}

const cxRenderF& cxRenderFArray::operator [](cxInt idx) const
{
    return at(idx);
}

cxRenderF& cxRenderFArray::operator [](cxInt idx)
{
    return at(idx);
}

//

cxIndicesArray::cxIndicesArray()
{
    number = 0;
}

cxIndicesArray::~cxIndicesArray()
{
    
}

void cxIndicesArray::Clear()
{
    number = 0;
}

cxInt cxIndicesArray::Inc(cxInt inc)
{
    number += inc;
    return number;
}

cxUInt16 &cxIndicesArray::Inc()
{
    return At(number++);
}

void cxIndicesArray::Append(const cxUInt16 &v)
{
    if(size() > number){
        At(number) = v;
    }else{
        push_back(v);
    }
    number ++;
}

void cxIndicesArray::Append(const cxUInt16 &v,cxInt start)
{
    if(size() > number){
        At(number) = v + start;
    }else{
        push_back(v + start);
    }
    number ++;
}

void cxIndicesArray::Append(const cxIndicesArray &v)
{
    for(cxInt i=0;i<v.Size();i++){
        Append(v.At(i));
    }
}

void cxIndicesArray::Append(const cxIndicesArray &v,cxInt start)
{
    for(cxInt i=0;i<v.Size();i++){
        Append(v.At(i),start);
    }
}

void cxIndicesArray::Init(cxInt n)
{
    for(cxInt i=0; i< n;i++){
        Append(0);
    }
    Clear();
}

void cxIndicesArray::Remove(cxInt idx)
{
    erase(begin() + idx);
}

void cxIndicesArray::Remove(cxInt idx,cxInt n)
{
    erase(begin() + idx, begin() + idx + n);
}

cxBool cxIndicesArray::IsEmpty() const
{
    return number == 0;
}

const cxInt cxIndicesArray::Capacity() const
{
    return (cxInt)size();
}

const cxInt cxIndicesArray::Size() const
{
    return (cxInt)number;
}

cxInt cxIndicesArray::DataBytes() const
{
    return Size() * sizeof(cxUInt16);
}

cxInt cxIndicesArray::BufferBytes() const
{
    return Capacity() * sizeof(cxUInt16);
}

const cxUInt16 *cxIndicesArray::Buffer() const
{
    return data();
}

cxUInt16 &cxIndicesArray::At(cxInt idx)
{
    return at(idx);
}

const cxUInt16 &cxIndicesArray::At(cxInt idx) const
{
    return at(idx);
}


CX_CPP_END

