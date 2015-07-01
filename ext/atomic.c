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
//ios for libuv fix
DIR *opendir$INODE64(const char * a)
{
    return opendir(a);
}
struct dirent *readdir$INODE64(DIR *dir)
{
    return readdir(dir);
}
int alphasort$INODE64(const struct dirent **a, const struct dirent **b)
{
    return alphasort(a, b);
}
int scandir$INODE64(const char *a, struct dirent ***b,int (*c)(const struct dirent *), int (*d)(const struct dirent **, const struct dirent **))
{
    return scandir(a, b, c, d);
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
#endif