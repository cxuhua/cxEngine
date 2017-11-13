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
#include <engine/cxPath.h>
#include "Block.h"
#include "Controller.h"

CX_CPP_BEGIN

CX_IMPLEMENT(Block)

Block::Block()
{
    map = nullptr;
    type = 0;
    move = nullptr;
    box = BoxTypeNone;
}

Block::~Block()
{
    
}

cxInt Block::GetLayer()
{
    return LayerActive;
}

void Block::OnTypeChanged(cxUInt typ)
{
    
}

void Block::SetType(cxUInt typ)
{
    if(type == typ){
        return;
    }
    type = typ;
    OnTypeChanged(type);
}

cxBool Block::IsEnableMoving()
{
    //假设红色为不可移动块
    return type != 0;
}

cxBool Block::IsEqu(const Block *item)
{
    return type == item->type;
}

void Block::SetIdx(const cxPoint2I &i)
{
    idx = i;
}

cxPoint2I Block::Index() const
{
    return idx;
}

//强制移除一个块
void Block::Drop()
{
    map->DropView(idx);
    Remove();
}

BoxType Block::GetBoxType()
{
    return box;
}

//当块被移除时
void Block::OnDrop(cxMultiple *m)
{
    cxScaleTo *scale = cxScaleTo::Create(0.0f, 0.15f);
    scale->onExit += cxAction::Remove;
    scale->AttachTo(this);
    m->Append(scale);
}

void Block::OnKeep(BoxType bt)
{
    box = bt;
}

//方块成型，返回true表示保留此方块OnKeepUp将被触发
cxBool Block::OnCompute(BoxType bt,const cxPoint2IArray &ps)
{
    if(bt == BoxType1x4){
        return true;
    }
    if(bt == BoxType3x3){
        return true;
    }
    if(bt == BoxType1x5){
        return true;
    }
    return false;
}

void Block::OnMoveFinished()
{
    
}

void Block::StartMove(cxMultiple *m,const PointArray &ps)
{
    map->DropView(idx);
    map->SetView(ps.First().ToPoint2I(), this);
    PointArray cps = ps.Combine();
    if(cps.Size() < 2){
        return;
    }
    if(move == nullptr){
        move = Move::Create(map);
        move->AttachTo(this);
        m->Append(move);
        move->onExit+=[this](cxAction *pav){
            OnMoveFinished();
            move = nullptr;
        };
    }
    move->AppendPoints(cps);
}

void Block::OnInit(Controller *pmap,const cxPoint2I &idx)
{
    map = pmap;
    SetSize(map->ItemSize());
    SetPosition(map->ToPos(idx));
    SetIdx(idx);
    map->SetView(idx, this);
}

Block *Block::Create(Controller *pmap,const cxPoint2I &idx)
{
    Block *ret = Block::Create();
    ret->OnInit(pmap, idx);
    
    cxSprite *sp = cxSprite::Create();
    sp->SetResizeFlags(ResizeFill);
    sp->SetTexture("grid.png");
    ret->Append(sp);
    
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

cxBool ItemAttr::IsEmpty(Controller *map)
{
    if(Static){
        return false;
    }
    if(Item != nullptr){
        return false;
    }
    return true;
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

CX_CPP_END


