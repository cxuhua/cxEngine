//
// cxWorld.h
// cxEngineTest
//
// Created by 徐华 on 2019/2/28.
//Copyright © 2019 xuhua. All rights reserved.
//

#ifndef cxWorld_h
#define cxWorld_h

#include <engine/cxView.h>
#include "Box2D.h"

CX_CPP_BEGIN

class cxBoxWorld : public cxView
{
public:
    CX_DECLARE(cxBoxWorld);
protected:
    explicit cxBoxWorld();
    virtual ~cxBoxWorld();
private:
    b2World world;
protected:
    void OnUpdate(cxFloat dt);
public:
};

CX_CPP_END

#endif
