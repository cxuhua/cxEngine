//
//  cxTable.cpp
//  cxEngineCore
//
//  Created by xuhua on 7/15/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#include <core/cxArray.h>
#include "cxTable.h"

CX_CPP_BEGIN

CX_IMPLEMENT(cxTable);

cxTable::cxTable()
{
    rownum = INT_MAX;
    outter = 1.0f;//view outter
    inner = 1.0f;//table inner
}

cxTable::~cxTable()
{
    
}

void cxTable::OnDirty()
{
    if(IsDirtyMode(DirtyModeForce)){
        UpdateViews();
    }
    cxSprite::OnDirty();
}

cxTable *cxTable::SetOutter(const cxBox4F &v)
{
    outter = v;
    return this;
}

cxTable *cxTable::SetInner(const cxBox4F &v)
{
    inner = v;
    return this;
}

cxTable *cxTable::SetRowNum(cxInt v)
{
    rownum = v;
    return this;
}

cxTable *cxTable::UpdateViews()
{
    cxArray *vs = Subviews();
    cxInt num = 0;
    cxFloat maxw = 0;
    cxFloat maxh = 0;
    cxFloat w = 0;
    cxFloat h = 0;
    for(cxArray::FIter it=vs->FBegin();it!=vs->FEnd();it++){
        cxView *pv = (*it)->To<cxView>();
        if(!pv->EnableVisible()){
            continue;
        }
        cxSize2F siz = pv->Size();
        w += (siz.w + outter.l + outter.r);
        cxFloat th = siz.h + outter.t + outter.b;
        if(th > h){
            h = th;
        }
        if(++num % rownum != 0){
            continue;
        }
        maxh += h;
        if(w >= maxw){
            maxw = w;
        }
        w = 0;
        h = 0;
    }
    maxw += (inner.l + inner.r + w);
    maxh += (inner.r + inner.b + h);
    SetSize(cxSize2F(maxw, maxh));
    
    cxInt i = 0;
    cxFloat offx = inner.l - maxw/2.0f;
    cxFloat offy = inner.b - maxh/2.0f;
    
    maxh = 0;
    for(cxArray::FIter it=vs->FBegin();it!=vs->FEnd();it++){
        cxView *pv = (*it)->To<cxView>();
        if(!pv->EnableVisible()){
            continue;
        }
        cxSize2F siz = pv->Size();
        if(siz.h > maxh){
            maxh = siz.h;
        }
        offx += (siz.w/2.0f + outter.l);
        pv->SetPosition(cxPoint2F(offx, offy + outter.b + siz.h/2.0f));
        offx += (siz.w/2.0f + outter.r);
        if(++i % rownum != 0){
            continue;
        }
        offx = inner.l - maxw/2.0f;
        offy += (maxh + outter.t + outter.b);
    }
    return this;
}

CX_CPP_END









