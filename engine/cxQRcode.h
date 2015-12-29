//
//  cxQRcode.h
//  cxEngineCore
//
//  Created by xuhua on 12/28/15.
//  Copyright © 2015 xuhua. All rights reserved.
//

#ifndef cxEngineCore_cxQRcode_h
#define cxEngineCore_cxQRcode_h

#include <qrcode/qrencode.h>
#include "cxTexture.h"

CX_CPP_BEGIN

class cxQRcode : public cxTexture
{
public:
    CX_DECLARE(cxQRcode);
protected:
    explicit cxQRcode();
    virtual ~cxQRcode();
private:
    cxBool hasAlpha;
    QRecLevel level;
    cxInt version;
    cxInt margin;
    cxInt size;
    unsigned char bgColor[4];
    unsigned char fgColor[4];
    void fillRow(unsigned char *row, int num, const unsigned char color[]);
    cxInt copyRow(unsigned char*bmp,unsigned char*row,cxInt rowNum,cxInt width);
public:
    void SetHasAlpha(cxBool v);
    void SetVersion(cxInt v);
    void SetLevel(QRecLevel v);
    void SetBGColor(const cxColor4F &color);
    void SetFGColor(const cxColor4F &color);
    cxQRcode *FromTXT(const cxStr *data);
};

CX_CPP_END

#endif


