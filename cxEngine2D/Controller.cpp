//
//  Controller.cpp
//  cxEngineTest
//
//  Created by xuhua on 5/16/16.
//  Copyright © 2016 xuhua. All rights reserved.
//

#include <core/cxUtil.h>
#include <engine/cxMove.h>
#include <engine/cxFade.h>
#include <engine/cxEngine.h>
#include "Controller.h"

CX_CPP_BEGIN

CX_IMPLEMENT(CardItem)

CardItem::CardItem()
{
    controller = nullptr;
    type = cxUtil::Rand(0, 3);
    if(type == 0){
        SetColor(cxColor4F::RED);
    }else if(type == 1){
        SetColor(cxColor4F::GREEN);
    }else if(type == 2){
        SetColor(cxColor4F::WHITE);
    }else {
        SetColor(cxColor4F::BLUE);
    }
}

CardItem::~CardItem()
{
    CX_LOGGER("%d %d removed",idx.x,idx.y);
}

cxBool CardItem::IsEqu(const CardItem *item)
{
    return type == item->type;
}

void CardItem::SetIdx(const cxPoint2I &i)
{
    idx = i;
}

cxPoint2I CardItem::Index() const
{
    return idx;
}

void CardItem::Drop()
{
    controller->DropView(idx);
    Remove();
}

cxAction *CardItem::MoveTo(const cxPoint2I &dst)
{
    controller->DropView(idx);
    CX_ASSERT(!controller->HasView(dst), "have view");
    controller->SetView(dst, this);
    cxPoint2F npos = controller->ToPos(dst);
    cxMoveTo *a = cxMoveTo::Create(npos, 0.3f);
    a->SetTiming(cxTiming::CubicOut);
    a->AttachTo(this);
    return a;
}

CardItem *CardItem::Create(Controller *c,const cxPoint2I &idx)
{
    CardItem *ret = CardItem::Create();
    ret->controller = c;
    ret->SetSize(c->ItemSize());
    ret->SetTexture("grid.png");
    ret->SetPosition(c->ToPos(idx));
    ret->SetIdx(idx);
    c->SetView(idx, ret);
    return ret;
}

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

const cxSize2F Controller::ItemSize() const
{
    return itemSize;
}

CardItem *Controller::DropView(const cxPoint2I &idx)
{
    CardItem *view = ToView(idx);
    if(view == nullptr){
        return nullptr;
    }
    items->Del(ToKey(idx));
    return view;
}

cxMultiple *Controller::SwapView(const cxPoint2I &src,const cxPoint2I &dst)
{
    cxMultiple *m = cxMultiple::Create();
    CardItem *srcView = ToView(src);
    CardItem *dstView = ToView(dst);
    CX_ASSERT(srcView != nullptr && dstView != nullptr, "view miss");
    cxMoveTo *m1 = cxMoveTo::Create(ToPos(dst), 0.1f);
    m1->AttachTo(srcView);
    m->Append(m1);
    cxMoveTo *m2 = cxMoveTo::Create(ToPos(src), 0.1f);
    m2->AttachTo(dstView);
    m->Append(m2);
    SetView(src, dstView);
    SetView(dst, srcView);
    m->AttachTo(this);
    return m;
}

//如果有消除对象启动消除动画，直到没有可消除的卡
cxBool Controller::HasSwap(const cxPoint2IArray &ps)
{
    cxMultiple *m = cxMultiple::Create();
    for(cxInt i=0;i<ps.Size();i++){
        const cxPoint2I &idx = ps.At(i);
        CardItem *view = ToView(idx);
        if(view == nullptr) {
            continue;
        }
        cxBox4I box = Compute(idx);
        BoxType bt = ParseBoxType(box);
        if(bt == BoxTypeNone){
            continue;
        }
        cxPoint2IArray ps = ToPoints(box, idx);
        CX_ASSERT(ps.Size() > 0, "points error");
        MergeTo(m, ps, idx);
    }
    if(m->Size() == 0){
        return false;
    }
    m->onInit +=[this](cxAction *pav){
        SetEnableTouch(false);
    };
    m->onExit +=[this](cxAction *pav){
        SetEnableTouch(true);
        MultipleExit(pav->To<cxMultiple>());
    };
    m->AttachTo(this);
    return true;
}

cxPoint2IArray Controller::ToPoints(const cxBox4I &box,const cxPoint2I &idx)
{
    cxPoint2IArray ps;
    cxInt x = idx.x;
    for(cxInt i=0;i<box.l;i++){
        ps.Append(cxPoint2I(--x, idx.y));
    }
    x = idx.x;
    for(cxInt i=0;i<box.r;i++){
        ps.Append(cxPoint2I(++x, idx.y));
    }
    cxInt y = idx.y;
    for(cxInt i=0;i<box.t;i++){
        ps.Append(cxPoint2I(idx.x, ++y));
    }
    y = idx.y;
    for(cxInt i=0;i<box.b;i++){
        ps.Append(cxPoint2I(idx.x, --y));
    }
    return ps;
}

//解析盒子形状
BoxType Controller::ParseBoxType(const cxBox4I &box)
{
    cxInt x = box.l + box.r + 1;
    cxInt y = box.t + box.b + 1;
    if(x == 5 || y == 5){
        return BoxType5;
    }
    if(x > 3 || y > 3){
        return BoxType4;
    }
    if(x >=3 && y >= 3){
        return BoxType4;
    }
    if(x >=3 || y >= 3){
        return BoxType3;
    }
    return BoxTypeNone;
}

cxInt Controller::MergeTo(cxMultiple *m,const cxPoint2IArray &ps,const cxPoint2I &idx)
{
    cxInt ret = 0;
    CardItem *view = ToView(idx);
    CX_ASSERT(view != nullptr, "view miss");
    for(cxInt i=0; i<ps.Size();i++){
        cxPoint2I sidx = ps.At(i);
        CardItem *sv = DropView(sidx);
        if(sv == nullptr){
            continue;
        }
        cxFadeTo *fade = cxFadeTo::Create(0.0f, 0.1f);
        fade->onExit += cxAction::Remove;
        fade->AttachTo(sv);
        m->Append(fade);
        ret ++;
    }
    return ret;
}

cxAction *Controller::Find(const cxPoint2I &idx)
{
    cxPoint2I nidx = cxPoint2I(idx.x,idx.y - 1);
    CardItem *view = nullptr;
    if(nidx.y < 0){
        view = CardItem::Create(this, idx);
        view->SetPosition(ToPos(nidx));
        Append(view);
    }else{
        view = ToView(nidx);
    }
    if(view == nullptr){
        return nullptr;
    }
    return view->MoveTo(idx);
}

void Controller::MultipleExit(cxMultiple *m)
{
    if(m->Size() == 0){
        return;
    }
    ScanSwap();
}

//从上到下扫描所有格子
cxMultiple *Controller::ScanSwap()
{
    cxMultiple *m = cxMultiple::Create();
    //扫描可消除的行
    for(cxInt i = 0; i < col; i++)
    for(cxInt j = row - 1;j >= 0; j--){
        cxPoint2I idx = cxPoint2I(i, j);
        CardItem *view = ToView(idx);
        if(view == nullptr) {
            continue;
        }
        cxBox4I box = Compute(idx);
        BoxType bt = ParseBoxType(box);
        if(bt == BoxTypeNone){
            continue;
        }
        cxPoint2IArray ps = ToPoints(box, idx);
        CX_ASSERT(ps.Size() > 0, "points error");
        MergeTo(m, ps, idx);
    }
    //扫描空位置并补充新卡
    for(cxInt i = 0; i < col; i++)
    for(cxInt j = row - 1;j >= 0; j--){
        cxPoint2I idx = cxPoint2I(i, j);
        CardItem *view = ToView(idx);
        if(view != nullptr) {
            continue;
        }
        cxAction *mv = Find(idx);
        if(mv == nullptr){
            continue;
        }
        m->Append(mv);
    }
    m->onInit +=[this](cxAction *pav){
        SetEnableTouch(false);
    };
    m->onExit +=[this](cxAction *pav){
        SetEnableTouch(true);
        MultipleExit(pav->To<cxMultiple>());
    };
    m->AttachTo(this);
    return m;
}

cxBool Controller::OnSwap(const cxPoint2I &src,const cxPoint2I &dst)
{
    if(!HasView(src) || !HasView(dst)){
        return false;
    }
    srcTmp = src;
    dstTmp = dst;
    cxMultiple *m = SwapView(src, dst);
    //动画时禁止键盘
    m->onInit +=[this](cxAction *pav){
        SetEnableTouch(false);
    };
    //动画结束时检测
    m->onExit +=[this](cxAction *pav){
        SetEnableTouch(true);
        points.Clear();
        points.Append(dstTmp);
        points.Append(srcTmp);
        //使用dstTmp是因为src点已经移动到dstTmp位置
        if(HasSwap(points)){
            return;
        }
        cxMultiple *m = SwapView(srcTmp, dstTmp);
        //动画时禁止键盘
        m->onInit +=[this](cxAction *pav){
            SetEnableTouch(false);
        };
        //结束时候开启键盘
        m->onExit +=[this](cxAction *pav){
            SetEnableTouch(true);
        };
    };
    //重置
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

cxBox4I Controller::Compute(const cxPoint2I &idx)
{
    CardItem *item = ToView(idx);
    if(item == nullptr){
        return cxBox4I(0);
    }
    cxInt l = 0;
    for(cxInt i = idx.x-1;i >= 0;i--){
        cxPoint2I v = cxPoint2I(i, idx.y);
        CardItem *pv = ToView(v);
        if(pv == nullptr){
            break;
        }
        if(!item->IsEqu(pv)){
            break;
        }
        l++;
    }
    cxInt r = 0;
    for(cxInt i = idx.x+1;i < col;i++){
        cxPoint2I v = cxPoint2I(i, idx.y);
        CardItem *pv = ToView(v);
        if(pv == nullptr){
            break;
        }
        if(!item->IsEqu(pv)){
            break;
        }
        r++;
    }
    cxInt t = 0;
    for(cxInt i = idx.y+1;i < row;i++){
        cxPoint2I v = cxPoint2I(idx.x, i);
        CardItem *pv = ToView(v);
        if(pv == nullptr){
            break;
        }
        if(!item->IsEqu(pv)){
            break;
        }
        t++;
    }
    cxInt b = 0;
    for(cxInt i = idx.y-1;i >= 0;i--){
        cxPoint2I v = cxPoint2I(idx.x, i);
        CardItem *pv = ToView(v);
        if(pv == nullptr){
            break;
        }
        if(!item->IsEqu(pv)){
            break;
        }
        b++;
    }
    return cxBox4I(l, r, t, b);
}

cxBool Controller::HasView(const cxPoint2I &idx)
{
    return ToView(idx) != nullptr;
}

void Controller::SetView(const cxPoint2I &idx,CardItem *pview)
{
    pview->SetIdx(idx);
    items->Set(ToKey(idx), pview);
}

cxBool Controller::IsValidIdx(const cxPoint2I &idx)
{
    return idx.x >= 0 && idx.x < col && idx.y >= 0 && idx.y < row;
}

CardItem *Controller::ToView(const cxPoint2I &idx)
{
    return items->Get(ToKey(idx))->To<CardItem>();
}

cxPoint2F Controller::ToPos(const cxPoint2I &idx)
{
    cxSize2F siz = Size();
    cxFloat x = (itemSize.w-siz.w)/2.0f + idx.x * itemSize.w;
    cxFloat y = (itemSize.h-siz.h)/2.0f + idx.y * itemSize.h;
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
    for(cxInt i = 0; i < col; i++)
    for(cxInt j = 0;j < row;j++){
        cxPoint2I idx = cxPoint2I(i, j);
        CardItem *sp = CardItem::Create(this, idx);
        Append(sp);
    }
    ScanSwap();
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

