//
//  lzma.h
//  cxEngine
//
//  Created by xuhua on 10/30/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#ifndef cxEngine_lzma_h
#define cxEngine_lzma_h

#ifdef __cplusplus
extern "C" {
#endif
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include "LzmaLib.h"

void cxLzmaSetFlags(char *flags);
    
int cxLzmaGetCompressLen(int srcLen);
    
int cxLzmaGetUncompressLen(void *src);

char *cxLzmaCompress(const char *psrc,int asrcLen,char *dst,int *pdesLen);

char *cxLzmaUncompress(const char *psrc,int asrcLen,char *dst,int *pdesLen);

#ifdef __cplusplus
}
#endif

#endif
