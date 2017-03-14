//
//  cxSync.h
//  udp
//
//  Created by xuhua on 14/03/2017.
//  Copyright © 2017 xuhua. All rights reserved.
//

#ifndef udp_cxSync_h
#define udp_cxSync_h

#include "cxObject.h"

CX_CPP_BEGIN

class cxBarrier
{
public:
    explicit cxBarrier(int count);
    virtual ~cxBarrier();
private:
    uv_barrier_t m;
public:
    void Wait();
};

class cxCond;
class cxMutex
{
    friend cxCond;
public:
    explicit cxMutex();
    virtual ~cxMutex();
private:
    uv_mutex_t m;
public:
    void Lock();
    void Unlock();
    int TryLock();
};

class cxCond
{
public:
    explicit cxCond();
    virtual ~cxCond();
private:
    uv_cond_t m;
public:
    void Signal();
    void Broadcast();
    void TimedWait(cxMutex &mutex, uint64_t timeout);
    void Wait(cxMutex &mutex);
};

class cxRWLock
{
public:
    explicit cxRWLock();
    virtual ~cxRWLock();
private:
    uv_rwlock_t m;
public:
    void RLock();
    int TryRLock();
    void RUnlock();
    void WLock();
    int TryWLock();
    void WUnlock();
};

class cxSem
{
public:
    explicit cxSem(int count);
    virtual ~cxSem();
private:
    uv_sem_t m;
public:
    void Wait();
    void Post();
    int TryWait();
};

CX_CPP_END

#endif


