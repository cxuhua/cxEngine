//
//  cxAutoPool.h
//  cxEngineCore
//  基于引用计数的内存管理实现
//  Created by xuhua on 5/23/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#ifndef __cxEngineCore__cxAutoPool__
#define __cxEngineCore__cxAutoPool__

#include "cxCore.h"
#include "cxStack.h"
#include "cxArray.h"

CX_CPP_BEGIN

class cxAutoPool : public cxObject
{
public:
    CX_DECLARE(cxAutoPool);
protected:
    explicit cxAutoPool();
    virtual ~cxAutoPool();
private:
    static cxStack *getThreadPoolStack();
    static cxAutoPool *getTopAutoPool();
    cxArray *objects;
public:
    static cxAutoPool *Start();
    static void Clear();
    static cxObject *Append(cxObject *object);
    static void Update();
    static cxInt Size();
    static void Push();
    static void Pop();
};

CX_CPP_END

#endif /* defined(__cxEngineCore__cxAutoPool__) */
