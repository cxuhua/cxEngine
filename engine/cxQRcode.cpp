//
//  cxQRcode.cpp
//  cxEngineCore
//
//  Created by xuhua on 12/28/15.
//  Copyright © 2015 xuhua. All rights reserved.
//

#include "cxEngine.h"
#include "cxQRcode.h"

CX_CPP_BEGIN

CX_IMPLEMENT(cxQRcode);

cxQRcode::cxQRcode()
{
    hasAlpha = false;
    SetBGColor(cxColor4F::WHITE);
    SetFGColor(cxColor4F::BLACK);
    margin = 4;
    version = 0;
    level = QR_ECLEVEL_L;
    size = 4 * cxEngine::Instance()->ScaleFactor();
}

cxQRcode::~cxQRcode()
{
    
}

void cxQRcode::SetVersion(cxInt v)
{
    version = v;
}

void cxQRcode::SetLevel(QRecLevel v)
{
    level = v;
}

void cxQRcode::SetBGColor(const cxColor4F &color)
{
    bgColor[0]=color.r * 255;
    bgColor[1]=color.g * 255;
    bgColor[2]=color.b * 255;
    bgColor[3]=color.a * 255;
}

void cxQRcode::SetFGColor(const cxColor4F &color)
{
    fgColor[0]=color.r * 255;
    fgColor[1]=color.g * 255;
    fgColor[2]=color.b * 255;
    fgColor[3]=color.a * 255;
}

void cxQRcode::SetHasAlpha(cxBool v)
{
    hasAlpha = v;
}

cxQRcode *cxQRcode::FromTXT(const cxStr *data)
{
    CX_ASSERT(cxStr::IsOK(data), "data args error");
    cxInt bw = hasAlpha?4:3;
    QRcode *qrcode = QRcode_encodeData(data->Size(), (const unsigned char *)data->Data(), version, level);
    cxInt realwidth = (qrcode->width + margin * 2) * size;
    cxInt rowBytes = realwidth * bw;
    unsigned char *row = (unsigned char *)malloc(rowBytes);
    unsigned char *bmp = (unsigned char *)malloc(rowBytes * realwidth);
    cxInt rowNum = 0;
    fillRow(row, realwidth, bgColor);
    for(cxInt y = 0; y < margin * size; y++) {
        rowNum = copyRow(bmp, row, rowNum,realwidth);
    }
    unsigned char *p = qrcode->data;
    for(cxInt y = 0; y < qrcode->width; y++) {
        fillRow(row, realwidth, bgColor);
        for(cxInt x = 0; x < qrcode->width; x++) {
            for(cxInt xx = 0; xx < size; xx++) {
                if(*p & 1) {
                    memcpy(&row[((margin + x) * size + xx) * bw], fgColor, bw);
                }
            }
            p++;
        }
        for(cxInt yy = 0; yy < size; yy++) {
            rowNum = copyRow(bmp, row, rowNum,realwidth);
        }
    }
    fillRow(row, realwidth, bgColor);
    for(cxInt y = 0; y < margin * size; y++) {
        rowNum = copyRow(bmp, row, rowNum,realwidth);
    }
    QRcode_free(qrcode);
    FromRGB((cchars)bmp, realwidth, realwidth);
    free(bmp);
    free(row);
    return this;
}

void cxQRcode::fillRow(unsigned char *row, int num, const unsigned char color[])
{
    cxInt bw = hasAlpha?4:3;
    for(cxInt i = 0; i < num; i++) {
        for(cxInt j=0;j<bw;j++){
            row[j + i*bw] = color[j];
        }
    }
}

cxInt cxQRcode::copyRow(unsigned char*bmp,unsigned char*row,cxInt rowNum,cxInt width)
{
    cxInt bw = hasAlpha?4:3;
    memcpy(bmp + rowNum * width * bw, row, width * bw);
    rowNum++;
    return rowNum;
}

CX_CPP_END

