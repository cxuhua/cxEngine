//
//  cxRenderF.cpp
//  cxEngineCore
//
//  Created by xuhua on 6/17/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//


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

void cxRenderFArray::Append(cxRenderFArray &v)
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

cxRenderF &cxRenderFArray::At(cxInt idx)
{
    return at(idx);
}

const cxRenderF &cxRenderFArray::At(cxInt idx) const
{
    return at(idx);
}


CX_CPP_END

