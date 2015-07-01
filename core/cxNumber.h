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
        int i32;
        int64_t i64;
        float fv;
        double dv;
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
    int GetType() const;
    //
    template<typename T>
    static cxNumber *Create(T value)
    {
        return cxNumber::Create()->Init(value);
    }
    //
    cxNumber *Init(int value);
    cxNumber *Init(cxBool value);
    cxNumber *Init(int64_t value);
    cxNumber *Init(float value);
    cxNumber *Init(double value);
    //
    int ToInt() const;
    cxBool ToBool() const;
    int64_t ToInt64() const;
    float ToFloat() const;
    double ToDouble() const;
};

CX_CPP_END

#endif


