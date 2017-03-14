//
//  cxSync.cpp
//  udp
//
//  Created by xuhua on 14/03/2017.
//  Copyright © 2017 xuhua. All rights reserved.
//


#include "cxSync.h"

CX_CPP_BEGIN

////////////////////////////////////////////////////////////////////////

cxBarrier::cxBarrier(int count)
{
    uv_barrier_init(&m, count);
}

cxBarrier::~cxBarrier()
{
    uv_barrier_destroy(&m);
}

void cxBarrier::Wait()
{
    uv_barrier_wait(&m);
}

////////////////////////////////////////////////////////////////////////

cxCond::cxCond()
{
    uv_cond_init(&m);
}

cxCond::~cxCond()
{
    uv_cond_destroy(&m);
}

void cxCond::Signal()
{
    uv_cond_signal(&m);
}

void cxCond::Broadcast()
{
    uv_cond_broadcast(&m);
}

void cxCond::TimedWait(cxMutex &mutex, uint64_t timeout)
{
    uv_cond_timedwait(&m, &mutex.m, timeout);
}

void cxCond::Wait(cxMutex &mutex)
{
    uv_cond_wait(&m, &mutex.m);
}

////////////////////////////////////////////////////////////////////////

cxSem::cxSem(int count)
{
    uv_sem_init(&m, count);
}

cxSem::~cxSem()
{
    uv_sem_destroy(&m);
}

void cxSem::Wait()
{
    uv_sem_wait(&m);
}

void cxSem::Post()
{
    uv_sem_post(&m);
}

int cxSem::TryWait()
{
    return uv_sem_trywait(&m);
}

////////////////////////////////////////////////////////////////////////

cxMutex::cxMutex()
{
    uv_mutex_init(&m);
}

cxMutex::~cxMutex()
{
    uv_mutex_destroy(&m);
}

int cxMutex::TryLock()
{
    return uv_mutex_trylock(&m);
}

void cxMutex::Lock()
{
    uv_mutex_lock(&m);
}

void cxMutex::Unlock()
{
    uv_mutex_unlock(&m);
}

////////////////////////////////////////////////////////////////////////

cxRWLock::cxRWLock()
{
    uv_rwlock_init(&m);
}

cxRWLock::~cxRWLock()
{
    uv_rwlock_destroy(&m);
}

void cxRWLock::RLock()
{
    uv_rwlock_rdlock(&m);
}

int cxRWLock::TryRLock()
{
    return uv_rwlock_tryrdlock(&m);
}
void cxRWLock::RUnlock()
{
    uv_rwlock_rdunlock(&m);
}
void cxRWLock::WLock()
{
    uv_rwlock_wrlock(&m);
}
int cxRWLock::TryWLock()
{
    return uv_rwlock_trywrlock(&m);
}
void cxRWLock::WUnlock()
{
    uv_rwlock_wrunlock(&m);
}

CX_CPP_END

