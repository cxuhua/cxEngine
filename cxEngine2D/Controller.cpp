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
#include <engine/cxPath.h>
#include "Controller.h"

CX_CPP_BEGIN

CX_IMPLEMENT(CardItem)

CardItem::CardItem()
{
    controller = nullptr;
    type = cxUtil::Rand(1, 4);
    SetType(type);
    move = nullptr;
}

CardItem::~CardItem()
{
    
}

void CardItem::SetType(cxUInt typ)
{
    type = typ;
    if(type == 0){
        SetColor(cxColor4F::RED);
    }else if(type == 1){
        SetColor(cxColor4F::GREEN);
    }else if(type == 2){
        SetColor(cxColor4F::WHITE);
    }else if(type == 3){
        SetColor(cxColor4F::BLUE);
    }else{
        SetColor(cxColor4F::YELLOW);
    }
}

cxBool CardItem::IsEnableMoving()
{
    //假设红色为不可移动块
    return type != 0;
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

void CardItem::OnDrop(cxMultiple *m)
{
    cxFadeTo *fade = cxFadeTo::Create(0.3f, 0.15f);
    fade->onExit += cxAction::Remove;
    fade->AttachTo(this);
    m->Append(fade);
}

void CardItem::OnKeepUp(BoxType bt)
{
    
}

cxBool CardItem::OnCompute(BoxType bt,const cxPoint2IArray &ps)
{
    if(bt == BoxType4){
        return true;
    }
    if(bt == BoxType5){
        return true;
    }
    return false;
}

void CardItem::StartMove(cxMultiple *m,const PointArray &ps)
{
    //移动到新位置
    controller->DropView(idx);
    controller->SetView(ps.At(0).ToPoint2I(), this);
    PointArray cps = ps.Combine();
    if(cps.Size() < 2){
        return;
    }
    if(move == nullptr){
        move = Move::Create(controller);
        move->AttachTo(this);
        m->Append(move);
        move->onExit+=[this](cxAction *pav){
            move = nullptr;
        };
    }
    move->AppendPoints(cps);
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


cxBool ItemAttr::IsFactory(Controller *map)
{
    return Item == nullptr && Factory;
}

cxBool ItemAttr::IsActiveItem(Controller *map)
{
    return !Static && Item != nullptr && Item->IsEnableMoving();
}

cxBool ItemAttr::IsSearch(Controller *map)
{
    if(Static){
        return false;
    }
    if(Item != nullptr && !Item->IsEnableMoving()){
        return false;
    }
    return true;
}

cxBool ItemAttr::IsSearchLR(Controller *map)
{
    return Static || (Item != nullptr && !Item->IsEnableMoving());
}

cxBool ItemAttr::IsPipe(Controller *map)
{
    return Item == nullptr && Src.IsPlus() && SrcP.IsPlus();
}

CX_IMPLEMENT(Controller);

Controller::Controller()
{
    Reset();
}

Controller::~Controller()
{
    
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
    CardItem *view = GetView(idx);
    if(view == nullptr){
        return nullptr;
    }
    items[idx.x][idx.y] = nullptr;
    return view;
}

cxMultiple *Controller::SwapView(const cxPoint2I &src,const cxPoint2I &dst)
{
    cxMultiple *m = cxMultiple::Create();
    CardItem *srcView = GetView(src);
    CardItem *dstView = GetView(dst);
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

BoxType Controller::FindHighRanking(const cxPoint2IArray &ps,const cxPoint2I &idx,cxPoint2I &out,cxBox4I &abox)
{
    BoxType tmp = BoxTypeNone;
    for(cxInt i=0;i<ps.Size();i++){
        cxPoint2I v = ps.At(i);
        if(v == idx){
            continue;
        }
        cxBox4I box = Compute(v);
        BoxType bt = ParseBoxType(box);
        if(bt > tmp){
            tmp = bt;
            out = v;
            abox = box;
        }
    }
    return tmp;
}

cxBool Controller::ComputeItem(cxMultiple *m,const cxPoint2I &idx)
{
    CardItem *view = GetView(idx);
    if(view == nullptr) {
        return false;
    }
    cxPoint2I out = idx;
    cxBox4I box = Compute(idx);
    BoxType bt = ParseBoxType(box);
    if(bt == BoxTypeNone){
        return false;
    }
    cxPoint2IArray ps = ToPoints(box, idx);
    //搜索另外一个更高级的方块，如果不是当前位置则使用这个更高级的方块
    BoxType tmp = FindHighRanking(ps, idx, out, box);
    if(tmp != BoxTypeNone){
        bt = tmp;
        ps = ToPoints(box, out);
        view = GetView(out);
    }
    if(view == nullptr){
        return false;
    }
    if(!view->OnCompute(bt, ps)){
        ps.Append(out);
    }else{
        view->OnKeepUp(bt);
    }
    if(ps.Size() > 0){
        MergeTo(m, ps);
    }
    return true;
}

//如果有消除对象启动消除动画，直到没有可消除的卡
cxBool Controller::HasSwap(const PointArray &ps)
{
    cxMultiple *m = cxMultiple::Create();
    for(cxInt i=0;i<ps.Size();i++){
        const Point &idx = ps.At(i);
        ComputeItem(m, idx.ToPoint2I());
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

cxInt Controller::MergeTo(cxMultiple *m,const cxPoint2IArray &ps)
{
    cxInt ret = 0;
    for(cxInt i=0; i<ps.Size();i++){
        cxPoint2I idx = ps.At(i);
        CardItem *sv = DropView(idx);
        if(sv == nullptr){
            continue;
        }
        sv->OnDrop(m);
        ret ++;
    }
    return ret;
}

CardItem *Controller::SearchPointAndView(PointArray &mps,const cxPoint2I &next,const cxPoint2I &prev)
{
    ItemAttr *attr = GetAttr(next);
    //如果是正常方块
    if(attr->IsActiveItem(this)){
        mps.Append(next,0);
        return attr->Item;
    }
    //如果是制造点
    if(attr->IsFactory(this)){
        cxPoint2I nidx = cxPoint2I(next.x, next.y + YTV[prev.x]);
        YTV[prev.x]++;
        attr->Item = CardItem::Create(this, prev);
        attr->Item->SetPosition(ToPos(nidx));
        Append(attr->Item);
        mps.Append(nidx,0);
        return attr->Item;
    }
    //继续向上搜索
    return SearchUpAndView(mps, next);
}

cxBool Controller::HasSearchPath(CardItem **item,const PointArray &mps)
{
    if(item == nullptr){
        return false;
    }
    CardItem *view = *item;
    if(view == nullptr || mps.Size() < 2){
        return false;
    }
    if(!view->IsEnableMoving()){
        return false;
    }
    return true;
}

cxBool Controller::EnableSearch(const cxPoint2I &idx)
{
    if(!IsValidIdx(idx)){
        return false;
    }
    ItemAttr *attr = GetAttr(idx);
    return attr->IsSearch(this);
}

// 未一个空位置搜索方块
CardItem *Controller::SearchUpAndView(PointArray &mps,const cxPoint2I &idx)
{
    ItemAttr *attr = GetAttr(idx);
    //处理隧道
    if(attr->IsPipe(this)){
        ItemAttr *dst = GetAttr(attr->Src);
        if(dst->Dst.IsPlus() && dst->Dst == idx){
            mps.Append(idx,ATTR_IS_KEEP);
            mps.Append(attr->SrcP, ATTR_IS_KEEP);
            mps.Append(dst->DstP,ATTR_IS_KEEP|ATTR_IS_JUMP);
            mps.Append(attr->Src,ATTR_IS_KEEP);
            return SearchPointAndView(mps, attr->Src, idx);
        }
    }
    mps.Append(idx,0);
    //设置下个搜索点
    cxPoint2I next = cxPoint2I(idx.x,idx.y + 1);
    attr = GetAttr(next);
    //是否进行两侧搜索
    if(attr->IsSearchLR(this)){
        YCV[idx.x]++;
        cxPoint2I v1 = cxPoint2I(0);
        cxPoint2I v2 = cxPoint2I(0);
        if((YCV[idx.x] % 2) == 0){
            v1 = cxPoint2I(next.x + 1,next.y);
            v2 = cxPoint2I(next.x - 1,next.y);
        }else{
            v1 = cxPoint2I(next.x - 1,next.y);
            v2 = cxPoint2I(next.x + 1,next.y);
        }
        if(EnableSearch(v1)){
            attr->Item = SearchPointAndView(mps, v1, idx);
            if(HasSearchPath(&attr->Item, mps)){
                return attr->Item;
            }
        }
        if(EnableSearch(v2)){
            attr->Item = SearchPointAndView(mps, v2, idx);
            if(HasSearchPath(&attr->Item, mps)){
                return attr->Item;
            }
        }
        mps.Clear();
        return attr->Item;
    }
    return SearchPointAndView(mps, next, idx);
}

void Controller::MultipleExit(cxMultiple *m)
{
    ScanSwap();
}

cxBool Controller::Search(cxMultiple *m,PointArray &mps,const cxPoint2I &next)
{
    //idx为空位置
    if(!IsValidIdx(next)){
        return false;
    }
    //向上查寻
    mps.Clear();
    CardItem *view = SearchUpAndView(mps,next);
    if(!HasSearchPath(&view, mps)){
        return false;
    }
    view->StartMove(m, mps);
    return true;
}

//从上到下扫描所有格子
cxMultiple *Controller::ScanSwap()
{
    for(cxInt i=0;i<MAX_ITEM;i++){
        YTV[i] = 0;
    }
    cxMultiple *m = cxMultiple::Create();
    //扫描可消除的行并消除方块
    for(cxInt j = 0; j < row; j++)
    for(cxInt i = 0; i < col; i++){
        cxPoint2I idx = cxPoint2I(i, j);
        ComputeItem(m, idx);
    }
    //扫描空位置并补充新卡
    cxBool ret = false;
    do{
        ret = false;
        for(cxInt j = 0; j < row; j++)
        for(cxInt i = 0; i < col; i++){
            cxPoint2I idx = cxPoint2I(i, j);
            ItemAttr *attr = GetAttr(idx);
            if(attr->Static){
                continue;
            }
            if(attr->Item != nullptr){
                continue;
            }
            PointArray mps;
            if(Search(m, mps, idx)){
                ret = true;
            }
        }
    }while(ret);
    //
    m->AttachTo(this);
    //没有数据直接返回(onInit onExit 依然执行)
    if(m->Size() == 0){
        return m;
    }
    m->onInit +=[this](cxAction *pav){
        SetEnableTouch(false);
    };
    m->onExit +=[this](cxAction *pav){
        SetEnableTouch(true);
        MultipleExit(pav->To<cxMultiple>());
    };
    return m;
}

cxMultiple *Controller::CheckSwap(const cxPoint2I &src,const cxPoint2I &dst)
{
    points.Clear();
    points.Append(dst,0);
    points.Append(src,0);
    //检测是否有消除
    if(HasSwap(points)){
        return nullptr;
    }
    //换回位置
    cxMultiple *m = SwapView(src, dst);
    //动画时禁止键盘
    m->onInit +=[this](cxAction *pav){
        SetEnableTouch(false);
    };
    //结束时候开启键盘
    m->onExit +=[this](cxAction *pav){
        SetEnableTouch(true);
    };
    return m;
}

cxBool Controller::OnSwap(const cxPoint2I &src,const cxPoint2I &dst)
{
    CardItem *srcview = GetView(src);
    if(srcview == nullptr || !srcview->IsEnableMoving()){
        return false;
    }
    CardItem *dstview = GetView(dst);
    if(dstview == nullptr || !dstview->IsEnableMoving()){
        return false;
    }
    cxMultiple *m = SwapView(src, dst);
    //动画时禁止键盘
    m->onInit +=[this](cxAction *pav){
        SetEnableTouch(false);
    };
    //动画结束时检测
    m->onExit +=[this,src,dst](cxAction *pav){
        SetEnableTouch(true);
        CheckSwap(src,dst);
    };
    //重置
    Reset();
    return true;
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
    //for test
    if(hit.hited && tp->type == cxTouchPoint::Ended){
        dstIdx = ToIdx(hit.point);
        CardItem *v = GetView(dstIdx);
        if(v != nullptr){
            v->Drop();
        }

//        for(cxInt i = 0;i < col;i++)
//        for(cxInt j = 0;j < row;j++){
//            cxPoint2I idx = cxPoint2I(i, j);
//            CardItem *v = GetView(idx);
//            if(v !=  nullptr && v->IsEnableMoving()){
//                v->Drop();
//            }
//        }
        
        ScanSwap();
        return true;
    }
    return false;

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
    CardItem *item = GetView(idx);
    if(item == nullptr){
        return cxBox4I(0);
    }
    cxInt l = 0;
    for(cxInt i = idx.x-1;i >= 0;i--){
        cxPoint2I v = cxPoint2I(i, idx.y);
        CardItem *pv = GetView(v);
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
        CardItem *pv = GetView(v);
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
        CardItem *pv = GetView(v);
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
        CardItem *pv = GetView(v);
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
    return GetView(idx) != nullptr;
}

void Controller::SetView(const cxPoint2I &idx,CardItem *pview)
{
    CX_ASSERT(IsValidIdx(idx), "idx not valid");
    pview->SetIdx(idx);
    items[idx.x][idx.y] = pview;
}

cxBool Controller::IsValidIdx(const cxPoint2I &idx)
{
    return idx.x >= 0 && idx.x < col && idx.y >= 0 && idx.y < row;
}

CardItem *Controller::GetView(const cxPoint2I &idx)
{
    if(!IsValidIdx(idx)){
        return nullptr;
    }
    return items[idx.x][idx.y];
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

void Controller::OnEnter()
{
    cxSize2F siz = Parent()->Size();
    itemSize.w = siz.w/col;
    itemSize.h = itemSize.w;
    cxSize2F nsiz;
    nsiz.w = siz.w;
    nsiz.h = itemSize.w * row;
    SetSize(nsiz);
    //for test
    for(cxInt i = 0;i < col;i++)
    for(cxInt j = 0;j < row;j++){
        cxPoint2I idx = cxPoint2I(i, j);
        ItemAttr *attr = GetAttr(idx);
        if(attr->Static){
            continue;
        }
        
        if(j == 3 && (i % 2) == 0){
            CardItem *sp = CardItem::Create(this, idx);
            sp->SetType(0);
            Append(sp);
        }
        if(i == 3 && (j == 5 || j== 6)){
            CardItem *sp = CardItem::Create(this, idx);
            sp->SetType(0);
            Append(sp);
        }
    }
    ScanSwap();
    cxView::OnEnter();
}

ItemAttr *Controller::GetAttr(const cxPoint2I &idx)
{
    CX_ASSERT(idx.x >=0 && idx.x < MAX_ITEM && idx.y >=0 && idx.y < MAX_ITEM, "idx error");
    ItemAttr *attr = &attrs[idx.x][idx.y];
    attr->Item = GetView(idx);
    return attr;
}

void Controller::Init()
{
    assert(col < MAX_ITEM && row < MAX_ITEM);
    //创建点
    for(int i=0;i<col;i++){
        ItemAttr attr;
        attr.Factory = true;
        attrs[i][row] = attr;
    }
    //静态位置
    ItemAttr attr;
    attr.Static = true;
    attrs[3][3]=attr;
    //隧道,连通 a1<->a2
    ItemAttr a1;
    a1.Src = cxPoint2I(1, 7);//设置来源坐标
    a1.SrcP = cxPoint2I(1, 2);//设置来源方块出现位置
    attrs[1][1] = a1;
    
    ItemAttr a2;
    a2.Dst = cxPoint2I(1, 1);//设置目标坐标
    a2.DstP = cxPoint2I(1, 6);//设置目标方块消失位置
    attrs[1][7] = a2;
    
//    ItemAttr a3;
//    a3.Dst = cxPoint2I(3,7);
//    a3.Src = cxPoint2I(7,0);
//    attrs[5][7] = a3;
//
//    ItemAttr a4;
//    a3.Dst = cxPoint2I(5,7);
//    attrs[7][0] = a3;
    
    for(int i=0;i<col;i++)
    for(int j=0;j<row;j++){
        items[i][j] = nullptr;
    }
    for(cxInt i=0;i<MAX_ITEM;i++){
        YCV[i]=0;
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

