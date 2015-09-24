//
//  atomic.cpp
//  cxEngineCore
//
//  Created by xuhua on 5/29/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//


#include "atomic.h"

#if CX_TARGET_PLATFORM == CX_PLATFORM_IOS || CX_TARGET_PLATFORM == CX_PLATFORM_MAC

#include <libkern/OSAtomic.h>
int32_t cxAtomicAddInt32(int32_t *p, int32_t x)
{
    return OSAtomicAdd32(x, p);
}
int32_t cxAtomicSubInt32(int32_t *p, int32_t x)
{
    return OSAtomicAdd32(-x, p);
}

#elif CX_TARGET_PLATFORM == CX_PLATFORM_ANDROID

#include <sys/atomics.h>
int32_t cxAtomicAddInt32(int32_t *p, int32_t x)
{
    return __sync_fetch_and_add(p,x);
}
int32_t cxAtomicSubInt32(int32_t *p, int32_t x)
{
    return __sync_fetch_and_sub(p,x);
}

//fix libuv
int getpwuid_r(uid_t uid, struct passwd *pw, char *buf, size_t size, struct passwd **result)
{
    struct passwd *r = getpwuid(uid);
    if(r == NULL){
        return -ENOENT;
    }
    *pw = *r;
    *result = r;
    return 0;
}

#elif CX_TARGET_PLATFORM == CX_PLATFORM_LINUX

int32_t cxAtomicAddInt32(int32_t *p, int32_t x)
{
    uint32_t t = x;
    asm volatile("lock; xaddl %0, %1;":"+r" (t),"=m"(*p):"m"(*p));
    return (t + x);
}

int32_t cxAtomicSubInt32(int32_t *p, int32_t x)
{
    uint32_t t;
    x = (uint32_t)(-(int32_t)x);
    t = x;
    asm volatile("lock; xaddl %0, %1;":"+r"(t),"=m"(*p):"m"(*p));
    return (t + x);
}

#endif