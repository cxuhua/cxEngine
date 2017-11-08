//
//  Move.cpp
//  cxEngineTest
//
//  Created by 徐华 on 08/11/2017.
//  Copyright © 2017 xuhua. All rights reserved.
//

#include "Move.h"

CX_CPP_BEGIN

CX_IMPLEMENT(Move)

Move::Move()
{
    
}

Move::~Move()
{
    
}

Move *Move::Make()
{
    Move *m = Move::Create();
    m->SetMoveSpeed(800);
    m->SetTiming(cxTiming::CubicOut);
    return m;
}

CX_CPP_END
