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
    delay = 0.0f;
    group = 0;
    from = 0;
    to = 0;
    key = 0;
    speed = 1.0f;
    repeat = 0;
}

cxActionAttr cxActionAttr::Reverse()
{
    cxActionAttr attr = *this;
    attr.from = to;
    attr.to = from;
    return attr;
}

CX_IMPLEMENT(cxAnimate);

cxAnimate::cxAnimate()
{
    iskeyframe = false;
    group = 0;
    frames = nullptr;
}

cxAnimate::~cxAnimate()
{
    cxObject::release(&frames);
}

cxAnimate *cxAnimate::SetAction(const cxActionAttr *pattr,cxInt agroup)
{
    CX_ASSERT(pattr->speed > 0 && frames != nullptr, "time or frames not set");
    attr = *pattr;
    SetSpeed(attr.speed);
    SetDelay(attr.delay);
    SetRepeat((attr.repeat == 0)?INT_MAX:attr.repeat);
    SetGroup(agroup);
    return this;
}

cxAnimate *cxAnimate::SetGroup(cxInt agroup)
{
    CX_ASSERT(frames != nullptr, "frames not set");
    CX_ASSERT(agroup < frames->Group(), "group set error");
    group = agroup;
    cxInt from = agroup * frames->Count() + attr.from;
    CX_ASSERT(from >= 0 && from < Points()->Size(), "from out");
    cxInt to = agroup * frames->Count() + attr.to;
    CX_ASSERT(to >= 0 && to < Points()->Size(), "to out");
    cxTimeLine::SetRange(from, to);
    return this;
}

cxAnimate *cxAnimate::SetRange(cxInt afrom,cxInt ato)
{
    CX_ASSERT(frames != nullptr, "frames not set");
    cxInt from = group * frames->Count() + afrom;
    CX_ASSERT(from >= 0 && from < Points()->Size(), "from out");
    cxInt to = group * frames->Count() + ato;
    CX_ASSERT(to >= 0 && to < Points()->Size(), "to out");
    cxTimeLine::SetRange(from, to);
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
    View()->To<cxAtlas>()->SetCoords(tp->Object()->To<cxArray>(),frames);
    cxInt idx = Index() + attr.from;
    iskeyframe = (idx == attr.key);
    onFrame.Fire(this, idx);
}

const cxBool cxAnimate::IsKeyFrame() const
{
    return iskeyframe;
}

const cxActionAttr &cxAnimate::ActionAttr() const
{
    return attr;
}

cxAnimate *cxAnimate::Create(cxFrames *frames)
{
    cxAnimate *rv = cxAnimate::Create();
    rv->SetFrames(frames);
    return rv;
}

cxAction *cxAnimate::Reverse()
{
    cxAnimate *rv = cxAnimate::Create();
    cxActionAttr vattr = attr.Reverse();
    rv->SetFrames(frames);
    rv->SetAction(&vattr, group);
    return rv;
}

cxAction *cxAnimate::Clone()
{
    cxAnimate *rv = cxAnimate::Create();
    rv->SetFrames(frames);
    rv->SetAction(&attr, group);
    return rv;
}

cxAnimate *cxAnimate::SetFrames(const cxFrames *aframes)
{
    CX_ASSERT(aframes != nullptr, "set frames error");
    cxObject::swap(&frames, aframes);
    SetPoints(frames->Points());
    attr.speed = aframes->Speed();
    attr.delay = aframes->Delay();
    attr.repeat = aframes->Repeat();
    attr.group = 0;
    attr.from = 0;
    attr.to = aframes->Points()->Size() - 1;
    SetAction(&attr, 0);
    return this;
}

CX_CPP_END

