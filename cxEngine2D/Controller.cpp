//
//  Controller.cpp
//  cxEngineTest
//
//  Created by xuhua on 5/16/16.
//  Copyright © 2016 xuhua. All rights reserved.
//

#include <engine/cxMove.h>
#include <engine/cxEngine.h>
#include "Controller.h"

CX_CPP_BEGIN

CX_IMPLEMENT(Controller);

Controller::Controller()
{
    items = cxHash::Alloc();
    Reset();
}

Controller::~Controller()
{
    items->Release();
}

void Controller::Reset()
{
    srcIdx = -1;
    dstIdx = -1;
}

cxMoveTo *Controller::SwapView(const cxPoint2I &src,const cxPoint2I &dst)
{
    cxView *srcView = ToView(src);
    cxView *dstView = ToView(dst);
    CX_ASSERT(srcView != nullptr && dstView != nullptr, "view miss");
    cxMoveTo *m1 = cxMoveTo::Create(ToPos(dst), 0.1f);
    m1->AttachTo(srcView);
    cxMoveTo *m2 = cxMoveTo::Create(ToPos(src), 0.1f);
    m2->AttachTo(dstView);
    SetView(src, dstView);
    SetView(dst, srcView);
    return m1;
}

//如果有消除对象启动消除动画，直到没有可消除的卡
cxBool Controller::IsSwap(const cxPoint2I &idx)
{
    //优先检测idx为中心的卡
    CX_LOGGER("%d %d",idx.x,idx.y);
    return false;
}

cxBool Controller::OnSwap(const cxPoint2I &src,const cxPoint2I &dst)
{
    srcTmp = src;
    dstTmp = dst;
    cxMoveTo *m = SwapView(src, dst);
    //动画时禁止键盘
    m->onInit +=[this](cxAction *pav){
        SetEnableTouch(false);
    };
    //动画结束时检测
    m->onExit +=[this](cxAction *pav){
        //使用dstTmp是因为src点已经移动到dstTmp位置
        if(IsSwap(dstTmp)){
            return;
        }
        cxMoveTo *m = SwapView(srcTmp, dstTmp);
        //结束时候开启键盘
        m->onExit +=[this](cxAction *pav){
            SetEnableTouch(true);
        };
    };
    //重置原和目标数据
    Reset();
    return true;
}

cxBool Controller::OnFight(const cxPoint2I &src)
{
    CX_LOGGER("%d %d go fight",src.x,src.y);
    return false;
}

cxBool Controller::IsMoveTo(const cxPoint2I &src,const cxPoint2I &dst)
{
    if(src == dst){
        return false;
    }
    if(!IsValidIdx(src)){
        return false;
    }
    if(!IsValidIdx(dst)){
        return false;
    }
    return AT_LEFT(src, dst) || AT_RIGHT(src, dst) || AT_TOP(src, dst) || AT_BOTTOM(src, dst);
}

cxBool Controller::OnDispatch(const cxengine::cxTouchable *e)
{
    if(e->TouchCount() != 1){
        return false;
    }
    const cxTouchPoint *tp = e->TouchPoint(0);
    const cxHitInfo hit = e->HitTest(this, tp->wp);
    if(hit.hited && tp->type == cxTouchPoint::Began){
        Reset();
        srcIdx = ToIdx(hit.point);
        return true;
    }
    if(!IsValidIdx(srcIdx)){
        Reset();
        return false;
    }
    if(hit.hited && tp->type == cxTouchPoint::Moved){
        dstIdx = ToIdx(hit.point);
        return IsMoveTo(srcIdx, dstIdx) && OnSwap(srcIdx, dstIdx);
    }
    if(tp->type == cxTouchPoint::Ended){
        Reset();
        return false;
    }
    return false;
}

cxBool Controller::HasView(const cxPoint2I &idx)
{
    return ToView(idx) != nullptr;
}

void Controller::SetView(const cxPoint2I &idx,cxView *pview)
{
    items->Set(ToKey(idx), pview);
}

cxBool Controller::IsValidIdx(const cxPoint2I &idx)
{
    return idx.x >= 0 && idx.x < col && idx.y >= 0 && idx.y < row;
}

cxView *Controller::ToView(const cxPoint2I &idx)
{
    return items->Get(ToKey(idx))->To<cxView>();
}

cxPoint2F Controller::ToPos(const cxPoint2I &idx)
{
    cxSize2F siz = Size();
    cxFloat x = -siz.w/2.0f + itemSize.w/2.0f + idx.x * itemSize.w;
    cxFloat y = -siz.h/2.0f + itemSize.h/2.0f + idx.y * itemSize.h;
    return cxPoint2F(x, y);
}

cxPoint2I Controller::ToIdx(const cxPoint2F &pos)
{
    cxSize2F siz = Size();
    cxFloat x = (pos.x + siz.w/2.0f)/itemSize.w;
    cxFloat y = (pos.y + siz.h/2.0f)/itemSize.h;
    return cxPoint2F(x, y).ToInt();
}

cxPoint2I Controller::ToIdx(cxInt key)
{
    cxInt x = key % col;
    cxInt y = key / col;
    return cxPoint2I(x,y);
}

cxInt Controller::ToKey(const cxPoint2I &idx)
{
    return idx.y * col + idx.x;
}

cxInt Controller::ToKey(const cxPoint2F &pos)
{
    return ToKey(ToIdx(pos));
}

#include <engine/cxLabel.h>

void Controller::Init()
{
    cxSize2F siz = cxEngine::Instance()->WinSize();
    itemSize.w = siz.w/col;
    itemSize.h = itemSize.w;
    cxSize2F nsiz;
    nsiz.w = siz.w;
    nsiz.h = itemSize.w * row;
    SetSize(nsiz);
    //for test
    for(cxInt i = 0; i < col; i++){
        for(cxInt j = 0;j < row;j++){
            cxPoint2I idx = cxPoint2I(i, j);
            cxSprite *sp = cxSprite::Create();
            sp->SetSize(itemSize);
            sp->SetTexture("t.png");
            sp->SetPosition(ToPos(idx));
            Append(sp);
            SetView(idx, sp);
            
            cxLabel *label = cxLabel::FromUTF8("%d %d",i,j);
            label->SetBold(true);
            label->SetFontSize(48);
            sp->Append(label);
        }
    }
}

Controller *Controller::Create(cxInt c,cxInt r)
{
    Controller *ret = Controller::Create();
    ret->col = c;
    ret->row = r;
    ret->Init();
    return ret;
}

CX_CPP_END

