//
//  cxNumber.cpp
//  cxEngineCore
//
//  Created by xuhua on 5/30/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//


#include "cxNumber.h"

CX_CPP_BEGIN

CX_IMPLEMENT(cxNumber);

cxNumber::cxNumber() : type(None)
{
    
}

cxNumber::~cxNumber()
{
    
}

cxNumber *cxNumber::Init(int value)
{
    v.i32 = value;
    type = Int;
    return this;
}
cxNumber *cxNumber::Init(cxBool value)
{
    v.bv = value;
    type = Bool;
    return this;
}
cxNumber *cxNumber::Init(int64_t value)
{
    v.i64 = value;
    type = Int64;
    return this;
}
cxNumber *cxNumber::Init(float value)
{
    v.fv = value;
    type = Float;
    return this;
}
cxNumber *cxNumber::Init(double value)
{
    v.dv = value;
    type = Double;
    return this;
}

int cxNumber::GetType() const
{
    return type;
}

int cxNumber::ToInt() const
{
    CX_ASSERT(type == Int, "type error");
    return v.i32;
}
cxBool cxNumber::ToBool() const
{
    CX_ASSERT(type == Bool, "type error");
    return v.bv;
}
int64_t cxNumber::ToInt64() const
{
    CX_ASSERT(type == Int64, "type error");
    return v.i64;
}
float cxNumber::ToFloat() const
{
    CX_ASSERT(type == Float, "type error");
    return v.fv;
}
double cxNumber::ToDouble() const
{
    CX_ASSERT(type == Double, "type error");
    return v.dv;
}

CX_CPP_END

