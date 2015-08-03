//
//  lzma.c
//  cxEngineStatic
//
//  Created by xuhua on 10/30/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include "lzma.h"

#pragma pack(1)

typedef struct{
    int32_t dataLen;                //not include head  4bytes
    uint8_t props[LZMA_PROPS_SIZE]; //props   5 bytes
}LzmaDataHead;

#pragma pack()

int cxLzmaGetCompressLen(int srcLen)
{
    return srcLen * 1.1f + LZMA_PROPS_SIZE + sizeof(LzmaDataHead);
}

char *cxLzmaCompress(const char *psrc,int asrcLen,char *pdst,int *pdstLen)
{
    assert(psrc != NULL && asrcLen > 0 && pdstLen != NULL && pdst != NULL);
    const unsigned char *src = (const unsigned char *)psrc;
    size_t srcLen = asrcLen;
    unsigned char outProps[LZMA_PROPS_SIZE]={0};
    size_t outPropsLen = LZMA_PROPS_SIZE;
    size_t dstLen = *pdstLen;
    unsigned char *dst = (unsigned char *)(pdst + sizeof(LzmaDataHead));
    int ret = LzmaCompress(dst, &dstLen, src, srcLen, outProps, &outPropsLen, 5, 1 << 16, 3, 0, 2, 32, 1);
    if(ret != SZ_OK){
        free(pdst);
        return NULL;
    }
    LzmaDataHead *head = (LzmaDataHead *)pdst;
    head->dataLen = (int32_t)srcLen;
    memcpy(head->props, outProps, outPropsLen);
    *pdstLen = (int)(dstLen + sizeof(LzmaDataHead));
    return (char *)pdst;
}

int cxLzmaGetUncompressLen(void *src)
{
    assert(src != NULL);
    LzmaDataHead *head = (LzmaDataHead *)src;
    return head->dataLen;
}

char *cxLzmaUncompress(const char *psrc,int asrcLen,char *pdst,int *pdstLen)
{
    assert(psrc != NULL && asrcLen > 0 && pdstLen != NULL && pdst != NULL);
    const unsigned char *src = (const unsigned char *)psrc;
    LzmaDataHead *head = (LzmaDataHead *)src;
    size_t srcLen = asrcLen - sizeof(LzmaDataHead);
    assert(*pdstLen == head->dataLen);
    size_t dstLen = (size_t)head->dataLen;
    //max 128M
    if(dstLen > 1024*1024*128 || dstLen <= 0){
        return NULL;
    }
    int ret = LzmaUncompress((unsigned char *)pdst, &dstLen, src + sizeof(LzmaDataHead), &srcLen, head->props, LZMA_PROPS_SIZE);
    if(ret != SZ_OK){
        return NULL;
    }
    *pdstLen = (int)dstLen;
    return (char *)pdst;
}



