//
//  cxDefine.h
//  cxEngineCore
//  -fno-rtti 运行时类型检测
//  -fno-exceptions 异常被禁用
//  Created by xuhua on 5/26/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#ifndef __cxEngineCore__cxDefine__
#define __cxEngineCore__cxDefine__

//#define NDEBUG 1

#define CX_PLATFORM_UNKNOWN            0
#define CX_PLATFORM_IOS                1
#define CX_PLATFORM_ANDROID            2
#define CX_PLATFORM_WIN32              3
#define CX_PLATFORM_MARMALADE          4
#define CX_PLATFORM_LINUX              5
#define CX_PLATFORM_BADA               6
#define CX_PLATFORM_BLACKBERRY         7
#define CX_PLATFORM_MAC                8
#define CX_PLATFORM_NACL               9

// mac
#if defined(CX_TARGET_MAC)
#define CX_TARGET_PLATFORM         CX_PLATFORM_MAC
#endif

// iphone
#if defined(CX_TARGET_IOS)
#define CX_TARGET_PLATFORM         CX_PLATFORM_IOS
#endif

// android
#if defined(ANDROID)
#define CX_TARGET_PLATFORM         CX_PLATFORM_ANDROID
#endif

// win32
#if defined(WIN32) && defined(_WINDOWS)
#define CX_TARGET_PLATFORM         CX_PLATFORM_WIN32
#endif

// linux
#if defined(LINUX)
#define CX_TARGET_PLATFORM         CX_PLATFORM_LINUX
#endif

// marmalade
#if defined(MARMALADE)
#define CX_TARGET_PLATFORM         CX_PLATFORM_MARMALADE
#endif

// bada
#if defined(SHP)
#define CX_TARGET_PLATFORM         CX_PLATFORM_BADA
#endif

// qnx
#if defined(__QNCX__)
#define CX_TARGET_PLATFORM          CX_PLATFORM_BLACKBERRY
#endif

// native client
#if defined(__native_client__)
#define CX_TARGET_PLATFORM          CX_PLATFORM_NACL
#endif

#define CX_UNUSED_PARAM(p) (void)p

#ifdef __cplusplus
#define CX_C_BEGIN       extern "C" {
#define CX_C_END         }
#else
#define CX_C_BEGIN
#define CX_C_END
#endif

#define CX_EXTERN   extern

#ifndef CX_MIN
#define CX_MIN(v1,v2) (((v1) > (v2)) ? (v2) : (v1))
#endif

#ifndef CX_MAX
#define CX_MAX(v1,v2) (((v1) < (v2)) ? (v2) : (v1))
#endif

#ifndef CX_SWAP
#define CX_SWAP(v1, v2) {typeof(v1) _temp_=(v1);v1=v2;v2=_temp_;}
#endif

#ifndef CX_SWAP32
#define CX_SWAP32(i)  ((i & 0x000000ff) << 24 | (i & 0x0000ff00) << 8 | (i & 0x00ff0000) >> 8 | (i & 0xff000000) >> 24)
#endif

#ifndef CX_SWAP16
#define CX_SWAP16(i)  ((i & 0x00ff) << 8 | (i & 0xff00) >> 8)
#endif

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdbool.h>
#include <errno.h>
#include <float.h>
#include <time.h>
#include <limits.h>
#include <sys/types.h>
#include <stddef.h>
#include <ctype.h>
#include <uv/uv.h>

CX_C_BEGIN

#include <ext/xxhash.h>
#include <ext/atomic.h>

CX_C_END

typedef void *          cxAny;
typedef const char *    cchars;
typedef char *          chars;

typedef float           cxFloat;
typedef double          cxDouble;

typedef unsigned char   cxUChar;

typedef int             cxInt;
typedef long            cxLong;
typedef unsigned int    cxUInt;
typedef unsigned long   cxULong;

typedef int8_t          cxInt8;
typedef int16_t         cxInt16;
typedef int32_t         cxInt32;
typedef int64_t         cxInt64;

typedef uint8_t         cxUInt8;
typedef uint16_t        cxUInt16;
typedef uint32_t        cxUInt32;
typedef uint64_t        cxUInt64;

typedef bool            cxBool;
typedef unsigned char   cxByte;

#define CX_ENGINE_VERSION               301

#define CX_ATTR_UNUSED                  __attribute__((__unused__))

#define CX_ATTR_FORMAT(f,v)             __attribute__((format(printf,f,v)))

#define CX_ATTR_NOT_NULL(n)             __attribute__((__nonnull__ (n)))

#if defined(__GNUC__) && ((__GNUC__ >= 4) || ((__GNUC__ == 3) && (__GNUC_MINOR__ >= 1)))
#define CX_ATTR_DEPRECATED __attribute__((deprecated))
#elif _MSC_VER >= 1400
#define CX_ATTR_DEPRECATED __declspec(deprecated)
#else
#define CX_ATTR_DEPRECATED
#endif

#define CX_HASH_MAX_KEY  64

#define CX_ERROR(format,...)            cxUtilError(__FILE__,__LINE__,format, ##__VA_ARGS__)

#define CX_WARN(format,...)             cxUtilWarn(__FILE__,__LINE__,format, ##__VA_ARGS__)

#define CX_TEST(cond,format,...)        if(!(cond))cxUtilInfo(__FILE__,__LINE__,format, ##__VA_ARGS__)

#if !defined(NDEBUG)
#define CX_LOGGER(format,...)           cxUtilInfo(__FILE__,__LINE__,format, ##__VA_ARGS__)
#define CX_ASSERT(cond,format,...)                              \
do{                                                             \
    cxBool _ret_= (cond);                                       \
    if(!_ret_)                                                  \
    cxUtilAssert(__FILE__,__LINE__,format, ##__VA_ARGS__);      \
    assert(_ret_);                                              \
}while(0)
#else
#define CX_LOGGER(format,...)
#define CX_ASSERT(cond,...)
#endif

#ifndef CX_MIN
#define CX_MIN(v1,v2) (((v1) > (v2)) ? (v2) : (v1))
#endif

#ifndef CX_MAX
#define CX_MAX(v1,v2) (((v1) < (v2)) ? (v2) : (v1))
#endif

#ifndef CX_SWAP
#define CX_SWAP(v1, v2) {typeof(v1) _temp_=(v1);v1=v2;v2=_temp_;}
#endif

#ifndef CX_SWAP32
#define CX_SWAP32(i)  ((i & 0x000000ff) << 24 | (i & 0x0000ff00) << 8 | (i & 0x00ff0000) >> 8 | (i & 0xff000000) >> 24)
#endif

#ifndef CX_SWAP16
#define CX_SWAP16(i)  ((i & 0x00ff) << 8 | (i & 0xff00) >> 8)
#endif

#define CX_INLINE                       inline

#define CX_CPP_BEGIN                    namespace cxengine {
#define CX_CPP_END                      }

CX_C_BEGIN

#include <ext/xxhash.h>
#include <ext/atomic.h>

CX_C_END

#define CX_DECLARE(T)                                               \
static CX_INLINE  T *Alloc()                                        \
{                                                                   \
    T *obj = new T();                                               \
    return obj;                                                     \
}                                                                   \
static CX_INLINE  T *Create()                                       \
{                                                                   \
    return static_cast<T *>(T::Alloc()->AutoRelease());             \
}                                                                   \
private:                                                            \
static cxHelper __helper__;                                         \
public:                                                             \
virtual const cxHelper &GetHelper() const;

#define CX_IMPLEMENT(T)                                             \
cxHelper T::__helper__=cxHelper(#T,(cxCore::AllocFunc)T::Alloc);    \
const cxHelper &T::GetHelper() const                                \
{                                                                   \
    return T::__helper__;                                           \
}

#endif /* defined(__cxEngineCore__cxDefine__) */








