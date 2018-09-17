//
//  cxLabel.h
//  cxEngineCore
//
//  Created by xuhua on 6/28/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#ifndef cxEngineCore_cxLabel_h
#define cxEngineCore_cxLabel_h

#include "cxSprite.h"

CX_CPP_BEGIN

enum cxTextAlign
{
    cxTextAlignNone     = 0,
    cxTextAlignLeft     = 1,
    cxTextAlignCenter   = 2,
    cxTextAlignRight    = 3,
};

enum cxTextNumFormat
{
    cxTextNumFormatNone        = 0,
    cxTextNumFormatDecimal     = 1,//123456 -> 123,456
    cxTextNumFormatCurrency    = 2,//123 -> $123
};

struct cxTextAttr
{
    cxFloat size;
    cxTextAlign align;
    cxColor4F color;
    cxFloat strokeWidth;
    cxColor4F strokeColor;
    cxPoint2F strokeOffset;
    cxBool boldFont;
    cxFloat fixWidth;
    cxTextNumFormat format;
    cxBool operator==(const cxTextAttr &v) const;
    cxBool operator!=(const cxTextAttr &v) const;
};

class cxLabel : public cxSprite
{
public:
    CX_DECLARE(cxLabel);
protected:
    explicit cxLabel();
    virtual ~cxLabel();
private:
    cxUInt64 key;
    cxTextAttr attr;
    cxStr *txt;
protected:
    virtual void OnDirty();
public:
    const cxStr *Text();
    cxLabel *SetText(const cxStr *ptxt);
    cxLabel *SetText(cchars fmt,...);
    cxLabel *SetFontSize(cxFloat size);
    cxLabel *SetAlign(const cxTextAlign &align);
    cxLabel *SetStroke(cxFloat width,const cxColor4F &color,const cxPoint2F &off);
    cxLabel *SetBold(cxBool bold);
    cxLabel *SetFixWidth(cxFloat fix);
    cxLabel *SetNumFormat(const cxTextNumFormat &fmt);
    cxLabel *SetLocalized(cchars key,...);
public:
    static cxLabel *FromUTF8(cchars fmt,...);
    static cxLabel *Create(const cxStr *txt);
    static cxLabel *Localized(cchars key,...);
};

CX_CPP_END

#endif


