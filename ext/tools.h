//
//  math.h
//  cxEngineCore
//
//  Created by xuhua on 6/26/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#ifndef cxEngineCore_tools_h
#define cxEngineCore_tools_h

#include <core/cxDefine.h>

CX_C_BEGIN

#if (CX_TARGET_PLATFORM == CX_PLATFORM_ANDROID)
#include <cpu-features.h>
#endif

#if (CX_TARGET_PLATFORM == CX_PLATFORM_IOS)
    #if defined (__arm64__)
        #define USE_NEON64
        #define INCLUDE_NEON64
    #elif defined (__ARM_NEON__)
        #define USE_NEON32
        #define INCLUDE_NEON32
    #endif
#elif (CX_TARGET_PLATFORM == CX_PLATFORM_ANDROID)
    #if defined (__arm64__) || defined (__aarch64__)
        #define USE_NEON64
        #define INCLUDE_NEON64
    #elif defined (__ARM_NEON__)
        #define INCLUDE_NEON32
    #endif
#endif

#if defined (__SSE__)
    #define USE_SSE
    #define INCLUDE_SSE
#endif

void Mat4_X_Mat4(const float *m1, const float *m2, float *dst);

void Vec4_X_Mat4(const float *m, float x, float y, float z, float w, float *dst);

CX_C_END

#endif
