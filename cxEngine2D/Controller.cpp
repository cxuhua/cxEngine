//
//  Controller.cpp
//  cxEngineTest
//
//  Created by xuhua on 5/16/16.
//  Copyright © 2016 xuhua. All rights reserved.
//

#include <core/cxUtil.h>
#include <engine/cxMove.h>
#include <engine/cxScale.h>
#include <engine/cxEngine.h>
#include "Controller.h"

CX_CPP_BEGIN

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

cxView *Controller::GetLayer(cxInt layer)
{
    CX_ASSERT(layer >= 0 && layer < LayerMax, "layer out bound");
    return layers[layer];
}

Block *Controller::DropView(const cxPoint2I &idx)
{
    Block *view = GetView(idx);
    if(view == nullptr){
        return nullptr;
    }
    items[idx.x][idx.y] = nullptr;
    return view;
}

cxMultiple *Controller::SwapView(const cxPoint2I &src,const cxPoint2I &dst)
{
    cxMultiple *m = cxMultiple::Create();
    Block *srcView = GetView(src);
    Block *dstView = GetView(dst);
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

cxBool Controller::ComputeBox(cxMultiple *m,const cxPoint2I &idx,cxBool advance)
{
    Block *view = GetView(idx);
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
    BoxType tmp = BoxTypeNone;
    //是否搜索另外一个更高级的方块，如果不是当前位置则使用这个更高级的方块
    if(advance){
        tmp = FindHighRanking(ps, idx, out, box);
    }
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
        view->OnKeep(bt);
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
        ComputeBox(m, idx.ToPoint2I(),false);
    }
    if(m->Size() == 0){
        return false;
    }
    combo++;
    OnOneCombo();
    m->onInit +=[this](cxAction *pav){
        SetEnableTouch(false);
    };
    m->onExit +=[this](cxAction *pav){
        SetEnableTouch(true);
        ScanSwapExit(pav->To<cxMultiple>());
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
    if(x >= 5 || y >= 5){
        return BoxType1x5;
    }
    if(x >=3 && y >= 3){
        return BoxType3x3;
    }
    if(x >= 4 || y >= 4){
        return BoxType1x4;
    }
    if(x >=3 || y >= 3){
        return BoxType1x3;
    }
    return BoxTypeNone;
}

cxInt Controller::MergeTo(cxMultiple *m,const cxPoint2IArray &ps)
{
    cxInt ret = 0;
    for(cxInt i=0; i<ps.Size();i++){
        cxPoint2I idx = ps.At(i);
        Block *sv = DropView(idx);
        if(sv == nullptr){
            continue;
        }
        sv->OnDrop(m);
        ret ++;
    }
    return ret;
}

//idx 最终位置
//pos 显示位置
Block *Controller::OnFactory(const cxPoint2I &idx,const cxPoint2I &pos)
{
    cxInt t = cxUtil::Rand(1, 4);
    Block *item = Block::Create(this, idx);
    item->SetType(t);
    item->SetPosition(ToPos(pos));

    cxSprite *sp = cxSprite::Create();
    sp->SetResizeFlags(ResizeFill);
    sp->SetTexture("grid.png");
    item->Append(sp);
    if(t == 1){
        sp->SetColor(cxColor4F::RED);
    }else if(t == 2){
        sp->SetColor(cxColor4F::GREEN);
    }else if(t == 3){
        sp->SetColor(cxColor4F::BLUE);
    }else if(t == 4){
        sp->SetColor(cxColor4F::YELLOW);
    }
    
    return item;
}

Block *Controller::SearchPointAndView(PointArray &mps,const cxPoint2I &next,const cxPoint2I &prev)
{
    ItemAttr *attr = GetAttr(next);
    //如果是正常方块
    if(attr->IsActiveItem(this)){
        mps.Append(next, ATTR_IS_NULL);
        return attr->Item;
    }
    //如果是制造点
    if(attr->IsFactory(this)){
        cxPoint2I nidx = cxPoint2I(next.x, next.y + YTV[prev.x]);
        YTV[prev.x]++;
        attr->Item = OnFactory(prev, nidx);
        CX_ASSERT(attr->Item != nullptr, "OnFactory return null");
        AppendBlock(attr->Item);
        mps.Append(nidx, ATTR_IS_NULL);
        return attr->Item;
    }
    //继续向上搜索
    return SearchUpAndView(mps, next);
}

cxBool Controller::HasSearchPath(Block **item,const PointArray &mps)
{
    if(item == nullptr){
        return false;
    }
    Block *view = *item;
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
Block *Controller::SearchUpAndView(PointArray &mps,const cxPoint2I &idx)
{
    if(idx.x >= MAX_ITEM || idx.y >= MAX_ITEM){
        mps.Clear();
        return nullptr;
    }
    //处理隧道
    ItemAttr *src = GetAttr(idx);
    if(src->IsPipe(this)){
        ItemAttr *dst = GetAttr(src->Src);
        //通道数据是否正常
        if(!dst->Dst.IsPlus() || dst->Dst != idx){
            CX_ERROR("pipe data settting error 2");
            mps.Clear();
            return src->Item;
        }
        //加入面目标位置
        mps.Append(idx, ATTR_IS_KEEP);
        //如果是同一点可以忽略
        if(src->SrcP != dst->DstP){
            mps.Append(src->SrcP, ATTR_IS_KEEP|ATTR_IS_FALL);
            mps.Append(dst->DstP, ATTR_IS_KEEP|ATTR_IS_JUMP);
        }
        //加入起始位置
        mps.Append(src->Src, ATTR_IS_KEEP);
        return SearchPointAndView(mps, src->Src, idx);
    }
    //设置下个搜索点
    mps.Append(idx, ATTR_IS_NULL);
    cxPoint2I next = cxPoint2I(idx.x,idx.y + 1);
    if(next.x >= MAX_ITEM || next.y >= MAX_ITEM){
        mps.Clear();
        return nullptr;
    }
    ItemAttr *attr = GetAttr(next);
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
        if(YCV[idx.x] > 10){
            YCV[idx.x] = 0;
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

//处理可消除块结束后扫描空位路径
void Controller::ScanSwapExit(cxMultiple *m)
{
    ScanEmpty();
}

//扫描路径结束后 如果有新路径则继续扫描是否可消除
void Controller::ScanEmptyExit(cxMultiple *m)
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
    Block *view = SearchUpAndView(mps,next);
    if(!HasSearchPath(&view, mps)){
        return false;
    }
    view->StartMove(m, mps);
    return true;
}

//扫描空位置
cxMultiple *Controller::ScanEmpty()
{
    cxBool ret = false;
    PointArray mps;
    cxMultiple *m = cxMultiple::Create();
    scan:
    ret = false;
    for(cxInt j = 0; j < row; j++)
    for(cxInt i = 0; i < col; i++){
        cxPoint2I idx = cxPoint2I(i, j);
        ItemAttr *attr = GetAttr(idx);
        if(!attr->IsEmpty(this)){
            continue;
        }
        mps.Clear();
        if(Search(m, mps, idx)){
            ret = true;
        }
    }
    if(ret)goto scan;
    if(m->Size() == 0){
        OnOneFinished();
        combo = 0;
        return m;
    }
    m->onInit +=[this](cxAction *pav){
        SetEnableTouch(false);
    };
    m->onExit +=[this](cxAction *pav){
        SetEnableTouch(true);
        ScanEmptyExit(pav->To<cxMultiple>());
    };
    m->AttachTo(this);
    return m;
}

void Controller::OnOneCombo()
{
    CX_LOGGER("OnOneCombo,combo=%d",combo);
}

void Controller::OnOneFinished()
{
    CX_LOGGER("OnOneFinished,combo=%d",combo);
}

//从上到下扫描所有格子
cxMultiple *Controller::ScanSwap()
{
    for(cxInt i=0;i<col;i++){
        YTV[i] = 0;
    }
    cxMultiple *m = cxMultiple::Create();
    //扫描可消除的行并消除方块
    for(cxInt j = 0; j < row; j++)
    for(cxInt i = 0; i < col; i++){
        cxPoint2I idx = cxPoint2I(i, j);
        ComputeBox(m, idx, true);
    }
    //如果没有消除可进行一次空位置扫描
    if(m->Size() == 0){
        ScanEmpty();
        return m;
    }
    combo++;
    OnOneCombo();
    m->onInit +=[this](cxAction *pav){
        SetEnableTouch(false);
    };
    m->onExit +=[this](cxAction *pav){
        SetEnableTouch(true);
        ScanSwapExit(pav->To<cxMultiple>());
    };
    m->AttachTo(this);
    return m;
}

cxBool Controller::HasSpecialSwap(Block *src,Block *dst)
{
    return false;
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
    Block *srcview = GetView(src);
    if(srcview == nullptr || !srcview->IsEnableMoving()){
        return false;
    }
    Block *dstview = GetView(dst);
    if(dstview == nullptr || !dstview->IsEnableMoving()){
        return false;
    }
    cxMultiple *m = SwapView(src, dst);
    //动画时禁止键盘
    m->onInit +=[this](cxAction *pav){
        SetEnableTouch(false);
    };
    //动画结束时检测
    m->onExit +=[this,src,dst,srcview,dstview](cxAction *pav){
        SetEnableTouch(true);
        if(!HasSpecialSwap(srcview,dstview)){
            CheckSwap(src,dst);
        }
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
//    if(hit.hited && tp->type == cxTouchPoint::Ended){
//        dstIdx = ToIdx(hit.point);
//        Block *v = GetView(dstIdx);
//        if(v != nullptr){
//            v->Drop();
//        }
//
////        for(cxInt i = 0;i < col;i++)
////        for(cxInt j = 0;j < row;j++){
////            cxPoint2I idx = cxPoint2I(i, j);
////            Block *v = GetView(idx);
////            if(v !=  nullptr && v->IsEnableMoving()){
////                v->Drop();
////            }
////        }
//
//        ScanSwap();
//        return true;
//    }
//    return false;

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
    Block *item = GetView(idx);
    if(item == nullptr){
        return cxBox4I(0);
    }
    cxInt l = 0;
    for(cxInt i = idx.x-1;i >= 0;i--){
        cxPoint2I v = cxPoint2I(i, idx.y);
        Block *pv = GetView(v);
        if(pv == nullptr || !pv->IsEnableCompute()){
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
        Block *pv = GetView(v);
        if(pv == nullptr || !pv->IsEnableCompute()){
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
        Block *pv = GetView(v);
        if(pv == nullptr || !pv->IsEnableCompute()){
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
        Block *pv = GetView(v);
        if(pv == nullptr || !pv->IsEnableCompute()){
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

void Controller::SetView(const cxPoint2I &idx,Block *pview)
{
    CX_ASSERT(IsValidIdx(idx), "idx not valid");
    pview->SetIdx(idx);
    items[idx.x][idx.y] = pview;
}

cxBool Controller::IsValidIdx(const cxPoint2I &idx)
{
    return idx.x >= 0 && idx.x < col && idx.y >= 0 && idx.y < row;
}

Block *Controller::GetView(const cxPoint2I &idx)
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

void Controller::OnEnter()
{
    //for test
    //创建点
    for(int i=0;i<col;i++){
        ItemAttr attr;
        attr.Factory = true;
        attrs[i][row] = attr;
    }
    ItemAttr a1;
    a1.Src = cxPoint2I(1, 3);//设置来源坐标
    a1.SrcP = cxPoint2I(1, 2);//设置来源方块出现位置
    attrs[1][1] = a1;

    ItemAttr a2;
    a2.Dst = cxPoint2I(1, 1);//设置目标坐标
    a2.DstP = cxPoint2I(1, 2);//设置目标方块消失位置
    attrs[1][3] = a2;
    
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

void Controller::AppendBlock(Block *b)
{
    CX_ASSERT(b != nullptr, "b args error");
    cxInt layer = b->GetLayer();
    CX_ASSERT(layer >= 0 && layer < LayerMax, "layer out bound");
    layers[layer]->Append(b);
}

void Controller::OnInit(cxInt acol,cxInt arow,const cxSize2F &size)
{
    assert(acol > 0 && acol < MAX_ITEM && arow > 0 && arow < MAX_ITEM);
    col = acol;
    row = arow;
    SetSize(size);
    cxSize2F siz = Size();
    itemSize.w = siz.w/col;
    itemSize.h = siz.h/row;
    //初始化动态数据
    for(int i=0;i<col;i++)
    for(int j=0;j<row;j++){
        items[i][j] = nullptr;
    }
    //初始化层
    for(int i=0;i<LayerMax;i++){
        layers[i] = cxView::Create();
        if(i == LayerActive){
            layers[i]->SetClip(true);
        }
        layers[i]->SetEnableTouch(false);
        layers[i]->SetResizeFlags(ResizeFill);
        Append(layers[i]);
    }
    for(cxInt i=0;i<MAX_ITEM;i++){
        YCV[i]=0;
    }
    combo = 0;
}

Controller *Controller::Create(cxInt col,cxInt row,const cxSize2F &size)
{
    Controller *ret = Controller::Create();
    ret->OnInit(col,row,size);
    return ret;
}

CX_CPP_END

