//
//  cxProgress.cpp
//  cxEngineCore
//
//  Created by xuhua on 7/13/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#include "cxProgress.h"

CX_CPP_BEGIN

CX_IMPLEMENT(cxProgress);

cxProgress::cxProgress()
{
    timer = nullptr;
    bg = nullptr;
    vv = nullptr;
    value = 0;
    range = cxRange2F(0.0f, 1.0f);
    dir = LeftToRight;
    inner = 0.0f;
}

cxProgress::~cxProgress()
{
    
}

void cxProgress::UpdateValue(cxFloat v)
{
    cxSize2F vsiz = Size();
    if(bg->Size().IsZero()){
        bg->SetSize(vsiz);
    }
    if(vv->Size().IsZero()){
        vv->SetSize(vsiz);
    }
    cxFloat p = v / (range.max - range.min);
    cxSize2F vvsiz = vv->Size();
    vvsiz.w = vsiz.w - (inner.l +inner.r);
    vvsiz.h = vsiz.h - (inner.t +inner.b);
    cxPoint2F vvpos = 0.0f;
    if(dir & LeftToRight){
        vvsiz.w = p * vsiz.w - (inner.r +inner.l);
        vvsiz.w = CX_MAX(vvsiz.w, 0);
        vvpos.x -= ((vsiz.w - vvsiz.w)/2.0f - inner.l);
    }
    if(dir & RightToLeft){
        vvsiz.w = p * vsiz.w - (inner.r +inner.l);
        vvsiz.w = CX_MAX(vvsiz.w, 0);
        vvpos.x += ((vsiz.w - vvsiz.w)/2.0f + inner.r);
    }
    if(dir & TopToBottom){
        vvsiz.h = p * vsiz.h - (inner.t +inner.b);
        vvsiz.h = CX_MAX(vvsiz.h, 0);
        vvpos.y += ((vsiz.h - vvsiz.h)/2.0f - inner.t);
    }
    if(dir & BottomToTop){
        vvsiz.h = p * vsiz.h - (inner.t +inner.b);
        vvsiz.h = CX_MAX(vvsiz.h, 0);
        vvpos.y -= ((vsiz.h - vvsiz.h)/2.0f + inner.b);
    }
    vv->SetPosition(vvpos);
    vv->EnableVisible(!vvsiz.IsZero());
    vv->SetSize(vvsiz);
}

void cxProgress::OnDirty()
{
    if(!IsDirtyMode(DirtyModeForce)){
        return;
    }
    UpdateValue(value);
}

void cxProgress::SetInner(const cxBox4F &v)
{
    inner = v;
}

cxProgress *cxProgress::SetDir(DirType v)
{
    dir = v;
    return this;
}

cxProgress *cxProgress::SetRange(const cxRange2F &v)
{
    if(range != v){
        range = v;
        SetDirty(DirtyModeForce);
    }
    return this;
}

cxProgress *cxProgress::SetValue(cxFloat v)
{
    v = cxFloatClamp(v, range.min, range.max);
    if(!cxFloatIsEqual(value, v)){
        value = v;
        SetDirty(DirtyModeForce);
    }
    return this;
}

cxProgress *cxProgress::Init(cxView *pbg,cxView *pvv,cxFloat v)
{
    bg = pbg;
    Append(pbg);
    vv = pvv;
    Append(pvv);
    value = v;
    return this;
}

cxProgress *cxProgress::Create(cxView *pbg,cxView *pvv,cxFloat v)
{
    CX_ASSERT(pbg != nullptr && pvv != nullptr, "args error");
    cxProgress *rv = cxProgress::Create();
    rv->Init(pbg, pvv, v);
    return rv;
}

CX_CPP_END

