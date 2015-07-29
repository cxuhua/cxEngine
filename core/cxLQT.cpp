//
//  cxLQT.cpp
//  TextureTool
//
//  Created by xuhua on 7/28/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//


#include "cxLQT.h"

CX_CPP_BEGIN

//RRRRRRRRGGGGGGGGBBBBBBBB -> RRRRGGGGBBBBAAAAA
void pixelRGBA8888ToRGBA4444(cxAny pdata, cxInt dataLen, cxAny outData)
{
    cxUChar *data = (cxUChar *)pdata;
    cxUInt16 *out16 = (cxUInt16 *)outData;
    for(cxInt i=0,l=dataLen-3; i<l;i+=4){
        *out16++ =
        (data[i]&0xF0)<<8|      //R
        (data[i+1]&0xF0)<<4|    //G
        (data[i+2] & 0xF0)|     //B
        (data[i+3]&0xF0)>>4;    //A
    }
}

//RRRRRRRRGGGGGGGGBBBBBBBB -> RRRRRGGGGGBBBBBA
void pixelRGBA8888ToRGBA5551(cxAny pdata, cxInt dataLen, cxAny outData)
{
    cxUChar *data = (cxUChar *)pdata;
    cxUInt16 *out16 = (cxUInt16 *)outData;
    for(cxInt i = 0, l = dataLen - 2; i < l; i += 4){
        *out16++ =
        (data[i] & 0x00F8) << 8                 //R
        | (data[i + 1] & 0x00F8) << 3           //G
        | (data[i + 2] & 0x00F8) >> 2           //B
        | (data[i + 3] & 0x0080) >> 7;          //A
    }
}

// RRRRRRRRGGGGGGGGBBBBBBBBAAAAAAAA -> RRRRRGGGGGGBBBBB
void pixelRGBA8888ToRGBA565(cxAny pdata, cxInt dataLen, cxAny outData)
{
    cxUChar *data = (cxUChar *)pdata;
    cxUInt16 *out16 = (cxUInt16 *)outData;
    for(cxInt i = 0, l = dataLen - 3; i < l; i += 4){
        *out16++ =
        (data[i] & 0x00F8) << 8                 //R
        | (data[i + 1] & 0x00FC) << 3           //G
        | (data[i + 2] & 0x00F8) >> 3;          //B
    }
}

CX_CPP_END

