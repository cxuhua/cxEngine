//
//  cxLooper.hpp
//  cxEngineCore
//
//  Created by 徐华 on 2019/6/13.
//  Copyright © 2019 xuhua. All rights reserved.
//

#ifndef cxLooper_hpp
#define cxLooper_hpp

#include <queue>
#include <functional>
#include "cxArray.h"
#include "cxStack.h"

CX_CPP_BEGIN

class cxLooper : public cxObject
{
public:
    CX_DECLARE(cxLooper);
protected:
    explicit cxLooper();
    virtual ~cxLooper();
public:
    static cxLooper *Looper();
    typedef std::function<void()> CB;
private:
    uv_mutex_t mutex;
    uv_async_t async;
    static void uv_async_runcb(uv_async_t* handle);
    std::queue<CB> cbs;
    uv_thread_t self;
    uv_loop_t looper;
    cxStack *stacks;
    cxArray *GetTop();
    cxBool isCurrThread();
public:
    void Run(cxBool wait=true);
    void Clear();
    cxObject *Append(cxObject *object);
    void Update();
    cxInt Size();
    void Push();
    void Pop();
public:
    void Async(CB cb);
};

CX_CPP_END

#endif /* cxLooper_hpp */
