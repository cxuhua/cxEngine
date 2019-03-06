//
//  cxWorld.cpp
//  cxEngineTest
//
//  Created by 徐华 on 2019/2/28.
//  Copyright © 2019 xuhua. All rights reserved.
//

#include "cxBoxWorld.h"

CX_CPP_BEGIN

CX_IMPLEMENT(cxBoxWorld);

cxBoxWorld::cxBoxWorld() : world(b2Vec2(0, -10))
{
    
}

cxBoxWorld::~cxBoxWorld()
{
    
}

void cxBoxWorld::OnUpdate(cxFloat dt)
{
    world.Step(dt, 6, 2);
}

CX_CPP_END

