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
    ignore = -1;
    rownum = INT_MAX;
    padding = 0.0f;      //view outter
    margin = 0.0f;       //table inner
}

cxTable::~cxTable()
{
    
}

cxTable *cxTable::SetIgnore(cxLong tag)
{
    if(ignore != tag){
        SetDirty(DirtyModeForce);
        ignore = tag;
    }
    return this;
}

void cxTable::OnDirty()
{
    if(IsDirtyMode(DirtyModeForce)){
        UpdateViews();
    }
    cxSprite::OnDirty();
}

cxTable *cxTable::SetPadding(const cxBox4F &v)
{
    if(padding != v){
        SetDirty(DirtyModeForce);
        padding = v;
    }
    return this;
}

cxTable *cxTable::SetMargin(const cxBox4F &v)
{
    if(margin != v){
        SetDirty(DirtyModeForce);
        margin = v;
    }
    return this;
}

cxTable *cxTable::SetNumPreRow(cxInt v)
{
    if(rownum != v){
        SetDirty(DirtyModeForce);
        rownum = v;
    }
    return this;
}

cxTable *cxTable::UpdateViews()
{
    cxArray *vs = Subviews();
    if(vs->IsEmpty()){
        return this;
    }
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
        if(pv->Tag() == ignore){
            continue;
        }
        cxSize2F siz = pv->Size();
        w += (siz.w + margin.l + margin.r);
        cxFloat th = siz.h + margin.t + margin.b;
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
    maxw += (padding.l + padding.r + w);
    maxh += (padding.t + padding.b + h);
    SetSize(cxSize2F(maxw, maxh));
    
    cxInt i = 0;
    cxFloat offx = padding.l - maxw/2.0f;
    cxFloat offy = padding.b - maxh/2.0f;
    
    maxh = 0;
    for(cxArray::FIter it=vs->FBegin();it!=vs->FEnd();it++){
        cxView *pv = (*it)->To<cxView>();
        if(!pv->EnableVisible()){
            continue;
        }
        if(pv->Tag() == ignore){
            continue;
        }
        cxSize2F siz = pv->Size();
        if(siz.h > maxh){
            maxh = siz.h;
        }
        offx += (siz.w/2.0f + margin.l);
        pv->SetPosition(cxPoint2F(offx, offy + margin.b + siz.h/2.0f));
        offx += (siz.w/2.0f + margin.r);
        if(++i % rownum != 0){
            continue;
        }
        offx = padding.l - maxw/2.0f;
        offy += (maxh + margin.t + margin.b);
    }
    return this;
}

CX_CPP_END









