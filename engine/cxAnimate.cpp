//
//  cxAnimate.cpp
//  cxEngineCore
//
//  Created by xuhua on 7/3/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#include "cxSprite.h"
#include "cxAtlas.h"
#include "cxAnimate.h"
#include "cxFrames.h"

CX_CPP_BEGIN

cxActionAttr::cxActionAttr()
{
    from = 0;
    to = 0;
    key = 0;
    speed = 1.0f;
    loop = false;
}

CX_IMPLEMENT(cxAnimate);

cxAnimate::cxAnimate()
{
    frames = nullptr;
}

cxAnimate::~cxAnimate()
{
    cxObject::release(&frames);
}

cxAnimate *cxAnimate::SetAttr(const cxActionAttr *pattr,cxInt agroup)
{
    CX_ASSERT(pattr->speed > 0 && frames != nullptr, "time or frames not set");
    group = agroup;
    attr = *pattr;
    SetSpeed(attr.speed);
    SetRepeat(attr.loop ? INT_MAX : 1);
    cxInt from  = agroup * frames->Count() + attr.from;
    CX_ASSERT(from >= 0 && from < Points()->Size(), "from out");
    cxInt to    = agroup * frames->Count() + attr.to;
    CX_ASSERT(to >= 0 && to < Points()->Size(), "to out");
    SetRange(from, to);
    return this;
}

void cxAnimate::OnInit()
{
    CX_ASSERT(frames != nullptr, "frames not set");
    cxAtlas *sp = View()->To<cxAtlas>();
    sp->SetTexture(frames->Texture());
    cxTimeLine::OnInit();
}

void cxAnimate::OnTime(const cxTimePoint *tp)
{
    View()->To<cxAtlas>()->SetCoords(tp->Object()->To<cxArray>());
    onTime.Fire(this, tp);
}

cxAction *cxAnimate::Reverse()
{
    cxAnimate *rv = cxAnimate::Create();
    rv->from = to;
    rv->to = from;
    rv->SetAttr(&attr, group);
    rv->SetFrames(frames);
    return rv;
}

cxAction *cxAnimate::Clone()
{
    cxAnimate *rv = cxAnimate::Create();
    rv->from = from;
    rv->to = to;
    rv->SetAttr(&attr, group);
    rv->SetFrames(frames);
    return rv;
}

cxAnimate *cxAnimate::SetFrames(const cxFrames *aframes)
{
    CX_ASSERT(aframes != nullptr, "set frames error");
    cxObject::swap(&frames, aframes);
    SetPoints(frames->Points());
    return this;
}

CX_CPP_END

