//
//  cxAnimate.cpp
//  cxEngineCore
//
//  Created by xuhua on 7/3/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#include "cxSprite.h"
#include "cxAnimate.h"
#include "cxFrames.h"

CX_CPP_BEGIN

cxActionAttr::cxActionAttr()
{
    from = 0;
    to = 0;
    key = 0;
    time = 0;
    loop = false;
}

CX_IMPLEMENT(cxAnimate);

cxAnimate::cxAnimate()
{
    frames = nullptr;
    ptex = nullptr;
}

cxAnimate::~cxAnimate()
{
    cxObject::release(&frames);
    cxObject::release(&ptex);
}

cxAnimate *cxAnimate::SetAttr(const cxActionAttr *pattr,cxInt acount,cxInt agroup)
{
    CX_ASSERT(pattr->time > 0 && acount > 0, "time or count not set");
    attr = *pattr;
    count = acount;
    group = agroup;
    cxFloat t = attr.time/1000.0f;
    SetSpeed(1.0f/t);
    SetLoop(attr.loop);
    cxInt from  = agroup * count + attr.from;
    cxInt to    = agroup * count + attr.to;
    SetRange(from, to);
    return this;
}

cxAnimate *cxAnimate::SetAttr(const cxActionAttr *pattr,cxInt agroup)
{
    CX_ASSERT(pattr->time > 0 && frames != nullptr, "time or frames not set");
    SetAttr(pattr, frames->Count(), agroup);
    return this;
}

void cxAnimate::OnInit()
{
    cxSprite *sp = View()->To<cxSprite>();
    if(ptex == nullptr){
        SetTexture(sp->Texture());
    }else{
        View()->To<cxSprite>()->SetTexture(ptex);
    }
    cxTimeLine::OnInit();
}

cxAnimate *cxAnimate::SetLoop(cxBool v)
{
    SetRepeat(v?INT_MAX:1);
    return this;
}

void cxAnimate::OnTime(const cxTimePoint *tp)
{
    cxTexCoord *coord = tp->Object()->To<cxTexCoord>();
    View()->To<cxSprite>()->SetTexCoord(coord);
}

cxAction *cxAnimate::Reverse()
{
    cxAnimate *rv = cxAnimate::Create();
    rv->from = to;
    rv->to = from;
    cxObject::swap(&rv->points, points);
    cxObject::swap(&rv->ptex, ptex);
    return rv;
}

cxAction *cxAnimate::Clone()
{
    cxAnimate *rv = cxAnimate::Create();
    rv->from = from;
    rv->to = to;
    cxObject::swap(&rv->points, points);
    cxObject::swap(&rv->ptex, ptex);
    return rv;
}

cxAnimate *cxAnimate::AppFmt(cxFloat time,cchars fmt,...)
{
    CX_ASSERT(ptex != nullptr && cxStr::IsOK(fmt), "must set texture");
    char key[256]={0};
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(key, 256, fmt, ap);
    va_end(ap);
    return Append(time, key);
}

cxAnimate *cxAnimate::SetFrames(const cxFrames *aframes)
{
    cxObject::swap(&frames, aframes);
    SetPoints(frames->Points());
    return this;
}

cxAnimate *cxAnimate::Append(cxFloat time,cchars key)
{
    CX_ASSERT(ptex != nullptr && cxStr::IsOK(key), "must set texture");
    cxTexCoord *coord = ptex->At(key);
    cxTimePoint *tp = cxTimeLine::Append(time);
    tp->SetObject(coord);
    return this;
}

cxAnimate *cxAnimate::SetTexture(cxTexture *texture)
{
    CX_ASSERT(texture != nullptr, "texture error");
    cxObject::swap(&ptex, texture);
    return this;
}

cxAnimate *cxAnimate::SetTexture(cchars key)
{
    CX_ASSERT(cxStr::IsOK(key), "key error");
    cxTexture *ctex = cxObject::gcpull<cxTexture>(key);
    SetTexture(ctex);
    return this;
}

CX_CPP_END

