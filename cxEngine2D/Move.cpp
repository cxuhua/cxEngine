//
//  Move.cpp
//  cxEngineTest
//
//  Created by 徐华 on 08/11/2017.
//  Copyright © 2017 xuhua. All rights reserved.
//

#include <engine/cxView.h>
#include "Move.h"
#include "Controller.h"

CX_CPP_BEGIN

CX_IMPLEMENT(Move)

Move::Move()
{
    map =  nullptr;
}

Move::~Move()
{
    
}

const Point Move::GetPoint(cxInt idx)
{
    CX_ASSERT(idx >= 0 && idx < mps.Size(), "v outbound");
    return mps.At(idx);
}

cxBool Move::OnArrive()
{
    cxInt i = GetIndex();
    Point p = GetPoint(i);
    if(p.IsJump()){
        Jump(1);
        return false;
    }
    return false;
}

void Move::AppendPoints(const PointArray &ps)
{
    for(cxInt i = ps.Size()-1;i >= 0;i--){
        AppendPoint(ps.At(i));
    }
}

void Move::AppendPoint(const Point &p)
{
    CX_ASSERT(map != nullptr, "map not set");
    if(mps.HasPoint(p) && !p.IsKeep()){
        return;
    }
    cxPoint2I idx = p.ToPoint2I();
    cxPoint2F pos = ((Controller *)map)->ToPos(idx);
    Append(pos);
    mps.Append(p);
}

Move *Move::Create(cxAny pmap)
{
    Move *rv = Move::Create();
    rv->map = pmap;
    rv->SetMoveSpeed(600);
    return rv;
}

CX_CPP_END
