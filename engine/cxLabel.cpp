//
//  cxLabel.cpp
//  cxEngineCore
//
//  Created by xuhua on 6/28/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#include <core/cxLocalized.h>
#include "cxLabel.h"
#include "cxTCaches.h"

CX_CPP_BEGIN

CX_IMPLEMENT(cxLabel);

cxLabel::cxLabel()
{
    key = 0;
    txt = cxStr::Alloc();
    memset(&attr, 0, sizeof(cxTextAttr));
    attr.size = 30;
    attr.cached = false;
    attr.color = cxColor4F::WHITE;
    attr.align = cxTextAlignCenter;
}

cxLabel::~cxLabel()
{
    if(attr.cached && key != 0){
        cxTCaches::Instance()->Remove(key);
    }
    txt->Release();
}

cxLabel *cxLabel::Localized(cchars key,...)
{
    const cxStr *fmt = cxLocalized::Text(key);
    cxLabel *rv = cxLabel::Create();
    va_list ap;
    va_start(ap, key);
    rv->SetText(cxStr::Create()->AppFmt(fmt->ToString(), ap));
    va_end(ap);
    return rv;
}

cxLabel *cxLabel::FromUTF8(cchars fmt,...)
{
    cxLabel *rv = cxLabel::Create();
    va_list ap;
    va_start(ap, fmt);
    rv->SetText(cxStr::Create()->AppFmt(fmt, ap));
    va_end(ap);
    return rv;
}

void cxLabel::OnDirty()
{
    if(!cxStr::IsOK(txt)){
        return;
    }
    if(IsDirtyMode(DirtyModeColor)){
        attr.color = Color();
    }
    if(IsDirtyMode(DirtyModeTexture)){
        cxTexture *ptex = cxTexture::Create()->FromTXT(txt, attr, &key);
        if(key != 0){
            cxTexCoord *coord = cxTCaches::Instance()->Coord(key);
            SetTexture(coord->Texture());
            SetTexCoord(coord);
            SetSize(coord->Size());
        }else if(ptex->IsSuccess()){
            SetTexture(ptex);
            SetSize(ptex->Size());
        }else{
            CX_ASSERT(false, "texture set failed");
        }
    }
    cxSprite::OnDirty();
}

cxLabel *cxLabel::SetFontSize(cxFloat size)
{
    if(!cxFloatIsEqual(attr.size, size)){
        attr.size = size;
        SetDirty(DirtyModeTexture);
    }
    return this;
}

cxLabel *cxLabel::SetAlign(const cxTextAlign &align)
{
    if(attr.align != align){
        attr.align = align;
        SetDirty(DirtyModeTexture);
    }
    return this;
}

cxLabel *cxLabel::SetStroke(cxFloat width,const cxColor4F &color,const cxPoint2F &off)
{
    if(!cxFloatIsEqual(attr.strokeWidth, width)){
        attr.strokeWidth = width;
        SetDirty(DirtyModeTexture);
    }
    if(attr.strokeColor != color){
        attr.strokeColor = color;
        SetDirty(DirtyModeTexture);
    }
    if(attr.strokeOffset != off){
        attr.strokeOffset = off;
        SetDirty(DirtyModeTexture);
    }
    return this;
}

cxLabel *cxLabel::SetBold(cxBool bold)
{
    if(attr.boldFont != bold){
        attr.boldFont = bold;
        SetDirty(DirtyModeTexture);
    }
    return this;
}

cxLabel *cxLabel::SetCached(cxBool cache)
{
    if(attr.cached != cache){
        attr.cached = cache;
        SetDirty(DirtyModeTexture);
    }
    return this;
}

cxLabel *cxLabel::SetFixWidth(cxFloat fix)
{
    if(!cxFloatIsEqual(attr.fixWidth, fix)){
        attr.fixWidth = fix;
        SetDirty(DirtyModeTexture);
    }
    return this;
}

cxLabel *cxLabel::SetNumFormat(const cxTextNumFormat &fmt)
{
    if(attr.format != fmt){
        attr.format = fmt;
        SetDirty(DirtyModeTexture);
    }
    return this;
}

cxLabel *cxLabel::SetLocalized(cchars key,...)
{
    const cxStr *fmt = cxLocalized::Text(key);
    va_list ap;
    va_start(ap, key);
    SetText(cxStr::Create()->AppFmt(fmt->ToString(), ap));
    va_end(ap);
    return this;
}

const cxStr *cxLabel::Text()
{
    return txt;
}

cxLabel *cxLabel::SetText(cchars fmt,...)
{
    CX_ASSERT(cxStr::IsOK(fmt), "fmt error");
    va_list ap;
    va_start(ap, fmt);
    cxStr *ptxt = cxStr::Create()->AppFmt(fmt, ap);
    va_end(ap);
    if(!ptxt->IsEqu(txt)){
        cxObject::swap(&txt, ptxt);
        SetDirty(DirtyModeTexture);
    }
    return this;
}

cxLabel *cxLabel::SetText(const cxStr *ptxt)
{
    CX_ASSERT(cxStr::IsOK(ptxt), "text null");
    if(!ptxt->IsEqu(txt)){
        cxObject::swap(&txt, ptxt);
        SetDirty(DirtyModeTexture);
    }
    return this;
}

CX_CPP_END

