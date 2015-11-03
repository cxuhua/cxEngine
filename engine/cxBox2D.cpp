//
//  cxBox2D.cpp
//  cxEngineCore
//
//  Created by xuhua on 11/2/15.
//  Copyright © 2015 xuhua. All rights reserved.
//

#include "cxBox2D.h"

CX_BOX2D_BEGIN

CX_IMPLEMENT(cxEdgeBody)

cxEdgeBody::cxEdgeBody()
{
    lineWidth = 10;
    shape = new b2EdgeShape();
}

cxEdgeBody::~cxEdgeBody()
{
    delete shape;
}

void cxEdgeBody::Set(const cxPoint2F &p1,const cxPoint2F &p2)
{
    v1 = p1;
    v2 = p2;
}

cxBool cxEdgeBody::CreateFixture(cxWorld *pw)
{
    shape->Set(b2Vec2(v1.x, v1.y), b2Vec2(v2.x, v2.y));
    fixDef.shape = shape;
    fixture = body->CreateFixture(&fixDef);
    return fixture != nullptr;
}

CX_IMPLEMENT(cxBoxBody)

cxBoxBody::cxBoxBody()
{
    shape = new b2PolygonShape();
}

cxBoxBody::~cxBoxBody()
{
    delete shape;
}

cxBool cxBoxBody::CreateFixture(cxWorld *pw)
{
    const cxSize2F &size = Size();
    shape->SetAsBox(size.w/2.0f, size.h/2.0f);
    fixDef.shape = shape;
    
    fixture = body->CreateFixture(&fixDef);
    return fixture != nullptr;
}

CX_IMPLEMENT(cxCircleBody)

cxCircleBody::cxCircleBody()
{
    shape = new b2CircleShape();
}

cxCircleBody::~cxCircleBody()
{
    delete shape;
}

cxBool cxCircleBody::CreateFixture(cxWorld *pw)
{
    const cxSize2F &size = Size();
    shape->m_radius = size.w/2.0;
    fixDef.shape = shape;

    fixture = body->CreateFixture(&fixDef);
    return fixture != nullptr;
}

CX_IMPLEMENT(cxBody)

cxBody::cxBody()
{
    fixture = nullptr;
    body = nullptr;
    bodyDef.type = b2_dynamicBody;
    fixDef.restitution = 1.0f;
    fixDef.density = 1.0f;
    fixDef.friction = 0.0f;
}

cxBody::~cxBody()
{
    
}

void cxBody::SetStatic(cxBool v)
{
    bodyDef.type = v?b2_staticBody:b2_dynamicBody;
}

void cxBody::SetElasticity(cxFloat v)
{
    fixDef.restitution = v;
}

void cxBody::SetDensity(cxFloat v)
{
    fixDef.density = v;
}

void cxBody::SetFriction(cxFloat v)
{
    fixDef.friction = v;
}

void cxBody::OnUpdate(cxFloat dt)
{
    cxView::OnUpdate(dt);
    const b2Vec2 &pos = body->GetPosition();
    cxView::SetPosition(cxPoint2F(pos.x, pos.y));
    cxView::SetAngle(body->GetAngle());
}

cxView *cxBody::SetPosition(const cxPoint2F &v)
{
    CX_ASSERT(body != nullptr, "body not init");
    const b2Vec2 &pos = body->GetPosition();
    if(cxFloatIsEqual(pos.x, v.x) && cxFloatIsEqual(pos.y, v.y)){
        return this;
    }
    body->SetTransform(b2Vec2(v.x, v.y), body->GetAngle());
    return this;
}

cxView *cxBody::SetAngle(const cxFloat &v)
{
    CX_ASSERT(body != nullptr, "body not init");
    const cxFloat angle = body->GetAngle();
    if(cxFloatIsEqual(angle, v)){
        return this;
    }
    body->SetTransform(body->GetPosition(), v);
    return this;
}

cxBool cxBody::CreateBody(cxWorld *pw)
{
    body = pw->CreateBody(&bodyDef);
    return body != nullptr;
}

cxBool cxBody::CreateFixture(cxWorld *pw)
{
    fixture = body->CreateFixture(&fixDef);
    return fixture != nullptr;
}

CX_IMPLEMENT(cxWorld);

cxWorld::cxWorld():world(b2Vec2(0, -100.0f))
{
    vIters = 1;
    pIters = 1;
    world.SetDestructionListener(this);
    world.SetContactFilter(this);
    world.SetContactListener(this);
}

cxWorld::~cxWorld()
{
    
}

void cxWorld::SayGoodbye(b2Joint* joint)
{
    CX_LOGGER("joint goodbye");
}

void cxWorld::SayGoodbye(b2Fixture* fixture)
{
    CX_LOGGER("fixture goodbye");
}

bool cxWorld::ShouldCollide(b2Fixture* fixtureA, b2Fixture* fixtureB)
{
    CX_LOGGER("ShouldCollide 0");
    return true;
}

void cxWorld::BeginContact(b2Contact* contact)
{
    CX_LOGGER("BeginContact 1");
}

void cxWorld::PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
{
//    contact->SetRestitution(0);
    CX_LOGGER("PreSolve 2");
}

void cxWorld::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
{
    CX_LOGGER("PostSolve 3");
}

void cxWorld::EndContact(b2Contact* contact)
{
    CX_LOGGER("EndContact 4");
}

b2Body *cxWorld::CreateBody(const b2BodyDef* def)
{
    return world.CreateBody(def);
}

cxBody *cxWorld::AppendBody(cxBody *body)
{
    if(!body->CreateBody(this)){
        return nullptr;
    }
    if(!body->CreateFixture(this)){
        return nullptr;
    }
    body->body->SetUserData(body);
    Append(body);
    return body;
}

void cxWorld::OnRemove(cxView *pview)
{
    cxView::OnRemove(pview);
    cxBody *body = pview->To<cxBody>();
    body->body->DestroyFixture(body->fixture);
    body->fixture = nullptr;
    world.DestroyBody(body->body);
    body->body = nullptr;
}

void cxWorld::OnUpdate(cxFloat dt)
{
    cxView::OnUpdate(dt);
    world.Step(dt, vIters, pIters);
}

void cxWorld::SetGravity(const cxPoint2F &v)
{
    world.SetGravity(b2Vec2(v.x, v.y));
}

CX_BOX2D_END

