//
//  atomic.h
//  cxEngineCore
//
//  Created by xuhua on 5/29/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#ifndef cxEngineCore_atomic_h
#define cxEngineCore_atomic_h

#include <stdint.h>
#include <core/cxDefine.h>

CX_C_BEGIN

int32_t cxAtomicAddInt32(int32_t *p, int32_t x);

int32_t cxAtomicSubInt32(int32_t *p, int32_t x);

CX_C_END

#endif


