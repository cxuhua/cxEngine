//
//  OpenAL.h
//  cxEngineCore
//
//  Created by xuhua on 7/29/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#ifndef cxEngineCore_OpenAL_h
#define cxEngineCore_OpenAL_h

#if (CX_TARGET_PLATFORM == CX_PLATFORM_ANDROID)

#include <OpenAL/al.h>
#include <OpenAL/alc.h>

#elif (CX_TARGET_PLATFORM == CX_PLATFORM_IOS)

#include <OpenAL/al.h>
#include <OpenAL/alc.h>

#elif (CX_TARGET_PLATFORM == CX_PLATFORM_MAC)

#include <OpenAL/al.h>
#include <OpenAL/alc.h>

#else
#error "not define CX_TARGET_PLATFORM"
#endif

#endif
