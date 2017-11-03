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
    delay = 0.0f;
    from = -1;
    to = -1;
    keynum = 0;
    speed = 1.0f;
    repeat = -1;
}

// user : set array 1:4:6
void cxActionAttr::SetKey(const cxStr *value)
{
    CX_ASSERT(from >= 0 && to >= 0, "from to not set");
    const cxArray *keys = value->Split(':');
    for(cxArray::CFIter it=keys->FBegin();it != keys->FEnd();it++){
        cxInt v = (*it)->To<cxStr>()->ToInt();
        CX_ASSERT(v >= from && v <= to, "action key range error");
        keyvar[keynum] = v;
        keynum++;
    }
}

void cxActionAttr::SetRepeat(const cxStr *value)
{
    CX_ASSERT(cxStr::IsOK(value), "value nullptr");
    cxInt v = value->ToInt();
    repeat = (v <= 0) ? INT_MAX : v;
}

void cxActionAttr::SetSpeed(const cxStr *value)
{
    CX_ASSERT(cxStr::IsOK(value), "value nullptr");
    cxFloat v = value->ToFloat();
    CX_ASSERT(v >= 0, "value error");
    speed = v;
}

void cxActionAttr::SetFrom(const cxStr *value)
{
    CX_ASSERT(cxStr::IsOK(value), "value nullptr");
    cxInt v = value->ToInt();
    CX_ASSERT(v >= 0, "value error");
    from = v;
}

void cxActionAttr::SetTo(const cxStr *value)
{
    CX_ASSERT(cxStr::IsOK(value), "value nullptr");
    cxInt v = value->ToInt();
    CX_ASSERT(v >= 0, "value error");
    to = v;
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
    keyframe = -1;
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
    SetRepeat((attr.repeat <= 0)?INT_MAX:attr.repeat);
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
    cxSprite *sp = View()->To<cxSprite>();
    sp->SetTexture(frames->Texture());
    cxTimeLine::OnInit();
}

void cxAnimate::OnTime(const cxTimePoint *tp)
{
    // view 应该基于cxAtlas
    View()->To<cxSprite>()->SetCoords(tp->Array(),frames->Map());
    // 计算组中的第几帧
    cxInt idx = Index() - group * frames->Count();
    // 是否是关键帧
    keyframe = -1;
    for(cxInt i=0;i<attr.keynum;i++){
        if(idx != attr.keyvar[i]){
            continue;
        }
        keyframe = i;
        onKey.Fire(this, attr.keyvar[i]);
        break;
    }
    // onevent
    onFrame.Fire(this, idx);
}

const cxInt cxAnimate::KeyValue() const
{
    if(keyframe < 0){
        return -1;
    }
    return attr.keyvar[keyframe];
}

const cxInt cxAnimate::KeyIndex() const
{
    return keyframe;
}

const cxActionAttr &cxAnimate::ActionAttr() const
{
    return attr;
}

cxAnimate *cxAnimate::Create(const cxFrames *frames)
{
    cxAnimate *rv = cxAnimate::Create();
    rv->SetFrames(frames);
    return rv;
}

cxAction *cxAnimate::Reverse()
{
    cxAnimate *rv = cxAnimate::Create();
    cxActionAttr vattr = attr.Reverse();
    rv->SetAction(&vattr, group);
    rv->SetSpeed(Speed());
    rv->SetFrames(frames);
    return rv;
}

cxAction *cxAnimate::Clone()
{
    cxAnimate *rv = cxAnimate::Create();
    rv->SetFrames(frames);
    rv->SetAction(&attr, group);
    rv->SetSpeed(Speed());
    return rv;
}

cxAnimate *cxAnimate::SetFrames(const cxFrames *aframes)
{
    CX_ASSERT(aframes != nullptr, "frames args error");
    cxObject::swap(&frames, aframes);
    SetPoints(frames->Points());
    attr.speed = aframes->Speed();
    attr.delay = aframes->Delay();
    attr.repeat = aframes->Repeat();
    attr.from = 0;
    attr.to = aframes->Points()->Size() - 1;
    SetAction(&attr, 0);
    return this;
}

CX_CPP_END

