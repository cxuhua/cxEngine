//
//  cxStack.h
//  cxEngineCore
//
//  Created by xuhua on 6/19/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#ifndef cxEngineCore_cxStack_h
#define cxEngineCore_cxStack_h

#include <stack>
#include "cxObject.h"

CX_CPP_BEGIN

class cxStack : public cxObject
{
public:
    CX_DECLARE(cxStack);
protected:
    explicit cxStack();
    virtual ~cxStack();
private:
    std::stack<cxObject *> ms;
public:
    cxBool IsEmpty() const;
    cxStack *Push(cxObject *obj);
    cxStack *Pop();
    cxObject *Top();
    cxStack *Clear();
};

CX_CPP_END

#endif


