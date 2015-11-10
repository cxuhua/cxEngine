//
//  cxNumber.h
//  cxEngineCore
//
//  Created by xuhua on 5/30/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#ifndef cxEngineCore_cxNumber_h
#define cxEngineCore_cxNumber_h

#include "cxObject.h"

CX_CPP_BEGIN

class cxNumber : public cxObject
{
public:
    CX_DECLARE(cxNumber);
protected:
    explicit cxNumber();
    virtual ~cxNumber();
private:
    union{
        cxBool bv;
        cxInt i32;
        cxInt64 i64;
        cxFloat fv;
        cxDouble dv;
    }v;
    unsigned char type;
public:
    enum {
        None,
        Bool,
        Int,
        Int64,
        Float,
        Double
    };
    cxInt GetType() const;
    //
    template<typename T>
    static cxNumber *Create(T value)
    {
        return cxNumber::Create()->Init(value);
    }
    //
    cxNumber *IntInc(cxInt value);
    //
    cxNumber *Init(cxInt value);
    cxNumber *Init(cxBool value);
    cxNumber *Init(cxInt64 value);
    cxNumber *Init(float value);
    cxNumber *Init(double value);
    //
    cxInt ToInt() const;
    cxBool ToBool() const;
    cxInt64 ToInt64() const;
    cxFloat ToFloat() const;
    cxDouble ToDouble() const;
};

CX_CPP_END

#endif


