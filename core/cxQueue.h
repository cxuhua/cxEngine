//
//  cxQueue.h
//  cxEngineCore
//
//  Created by xuhua on 6/21/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#ifndef cxEngineCore_cxQueue_h
#define cxEngineCore_cxQueue_h

#include <queue>
#include "cxObject.h"

CX_CPP_BEGIN

class cxQueue : public cxObject
{
public:
    CX_DECLARE(cxQueue);
protected:
    explicit cxQueue();
    virtual ~cxQueue();
private:
    std::queue<cxObject *> mq;
public:
    cxBool IsEmpty() const;
    cxQueue *Push(cxObject *obj);
    cxQueue *Pop();
    cxObject *Front() const;
    cxObject *Back() const;
    cxInt Size() const;
    cxQueue *Clear();
};

CX_CPP_END

#endif


