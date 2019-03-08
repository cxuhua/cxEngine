//
//  cxWorld.cpp
//  cxEngineTest
//
//  Created by 徐华 on 2019/2/28.
//  Copyright © 2019 xuhua. All rights reserved.
//

#include <engine/cxEngine.h>
#include "cxBoxWorld.h"

CX_CPP_BEGIN

CX_IMPLEMENT(cxBoxWorld);

cxBoxWorld::cxBoxWorld()
{
    pscale = PixelScale();
    world = new b2World(b2Vec2(0, 0));
}

cxBoxWorld::~cxBoxWorld()
{
    delete world;
}

void cxBoxWorld::AppendViewExt(cxView *pv,cxViewExt *ext)
{
    pv->SetExt(ext);
    Append(pv);
}

b2World *cxBoxWorld::GetWorld()
{
    return world;
}

cxPoint2F cxBoxWorld::ToPixel(const b2Vec2 &v)
{
    cxPoint2F pos(v.x,v.y);
    return pos * pscale;
}

b2Vec2 cxBoxWorld::ToMeters(const cxPoint2F &v)
{
    cxPoint2F pos = v/pscale;
    return b2Vec2(pos.x,pos.y);
}

cxFloat cxBoxWorld::PixelScale()
{
    return 32;
}

cxBoxBody *cxBoxWorld::CreateBox(const cxPoint2F &v,b2BodyType type)
{
    b2Vec2 mv = ToMeters(v);
    b2BodyDef def;
    def.type = type;
    cxBoxBody *body = cxBoxBody::Create();
    body->world = this;
    body->body = world->CreateBody(&def);
    b2PolygonShape shape;
    shape.SetAsBox(mv.x/2.0f,mv.y/2.0f);
    b2FixtureDef fixdef;
    fixdef.shape = &shape;
    fixdef.density = 1.0f;
    fixdef.friction = 1.0f;
    fixdef.restitution = 0.0f;
    body->fixture = body->body->CreateFixture(&fixdef);
    return body;
}

void cxBoxWorld::OnUpdate(cxFloat dt)
{
    world->Step(dt, 6, 2);
}

void cxBoxWorld::SetGravity(const cxPoint2F &v)
{
    world->SetGravity(cxPoint2FToVec2(v));
}

CX_CPP_END

