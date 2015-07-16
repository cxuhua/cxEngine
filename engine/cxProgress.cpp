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
    bg = nullptr;
    vv = nullptr;
    value = 0;
    range = cxRange2F(0.0f, 1.0f);
    dir = LeftToRight;
}

cxProgress::~cxProgress()
{
    
}

void cxProgress::OnDirty()
{
    if(!IsDirtyMode(DirtyModeForce)){
        return;
    }
    cxSize2F vsiz = Size();
    if(bg->Size().IsZero()){
        bg->SetSize(vsiz);
    }
    if(vv->Size().IsZero()){
        vv->SetSize(vsiz);
    }
    cxFloat p = value / (range.max - range.min);
    cxSize2F vvsiz = vv->Size();
    cxPoint2F vvpos = 0.0f;
    if(dir & LeftToRight){
        vvsiz.w = p * vsiz.w;
        vvpos.x -= (vsiz.w - vvsiz.w)/2.0f;
    }
    if(dir & RightToLeft){
        vvsiz.w = p * vsiz.w;
        vvpos.x += (vsiz.w - vvsiz.w)/2.0f;
    }
    if(dir & TopToBottom){
        vvsiz.h = p * vsiz.h;
        vvpos.y += (vsiz.h - vvsiz.h)/2.0f;
    }
    if(dir & BottomToTop){
        vvsiz.h = p * vsiz.h;
        vvpos.y -= (vsiz.h - vvsiz.h)/2.0f;
    }
    vv->SetPosition(vvpos);
    vv->SetSize(vvsiz);
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

cxProgress *cxProgress::Create(cxView *pbg,cxView *pvv,cxFloat v)
{
    CX_ASSERT(pbg != nullptr && pvv != nullptr, "args error");
    cxProgress *rv = cxProgress::Create();
    rv->bg = pbg;
    rv->Append(pbg);
    rv->vv = pvv;
    rv->Append(pvv);
    rv->value = v;
    return rv;
}

CX_CPP_END

