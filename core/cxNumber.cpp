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

cxNumber *cxNumber::IntInc(cxInt value)
{
    CX_ASSERT(type == Int, "type error");
    v.i32 += value;
    return this;
}

cxNumber *cxNumber::Init(cxInt value)
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

cxNumber *cxNumber::Init(cxInt64 value)
{
    v.i64 = value;
    type = Int64;
    return this;
}

cxNumber *cxNumber::Init(cxFloat value)
{
    v.fv = value;
    type = Float;
    return this;
}

cxNumber *cxNumber::Init(cxDouble value)
{
    v.dv = value;
    type = Double;
    return this;
}

cxInt cxNumber::GetType() const
{
    return type;
}

cxInt cxNumber::ToInt() const
{
    CX_ASSERT(type == Int, "type error");
    return v.i32;
}

cxBool cxNumber::ToBool() const
{
    CX_ASSERT(type == Bool, "type error");
    return v.bv;
}

cxInt64 cxNumber::ToInt64() const
{
    CX_ASSERT(type == Int64, "type error");
    return v.i64;
}

cxFloat cxNumber::ToFloat() const
{
    CX_ASSERT(type == Float, "type error");
    return v.fv;
}

cxDouble cxNumber::ToDouble() const
{
    CX_ASSERT(type == Double, "type error");
    return v.dv;
}

CX_CPP_END

