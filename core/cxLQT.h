//
//  cxLQT.h
//  TextureTool
//
//  Created by xuhua on 7/28/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#ifndef TextureTool_cxLQT_h
#define TextureTool_cxLQT_h

#include "cxUtil.h"

CX_CPP_BEGIN

#pragma pack(1)
struct LQT
{
    static const uint8_t FormatRGBA8888 = 1;
    static const uint8_t FormatRGBA4444 = 2;
    static const uint8_t FormatRGBA5551 = 3;
    static const uint8_t FormatRGB565   = 4;
    static const uint8_t FormatRGB888   = 5;
    uint32_t flag;                              //LQT!
    uint8_t format;                             //LQTFormat
    uint32_t filebytes;                         // = headbytes + atlasbytes + databytes
    uint16_t width;                             //texture width
    uint16_t height;                            //texture height
    uint32_t atlasbytes;                        //json lzma bytes == 0 not atlas file
    uint32_t databytes;                         //data lzma bytes
};

#define CX_LQT_TAG (*(cxUInt32 *)("LQT!"))

#pragma pack()

void pixelRGBA8888ToRGBA4444(cxAny pdata, cxInt dataLen, cxAny outData);
void pixelRGBA8888ToRGBA5551(cxAny pdata, cxInt dataLen, cxAny outData);
void pixelRGBA8888ToRGB565(cxAny pdata, cxInt dataLen, cxAny outData);
void pixelRGBA8888ToRGB888(cxAny pdata, cxInt dataLen, cxAny outData);

CX_CPP_END

#endif


