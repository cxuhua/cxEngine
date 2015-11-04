//
//  cxBox2D.cpp
//  cxEngineCore
//
//  Created by xuhua on 11/2/15.
//  Copyright © 2015 xuhua. All rights reserved.
//

#include "cxBox2D.h"

CX_BOX2D_BEGIN

CX_IMPLEMENT(cxChainBody)

cxChainBody::cxChainBody()
{
    isloop = false;
}

cxChainBody::~cxChainBody()
{
    
}

void cxChainBody::SetLoop(cxBool loop)
{
    isloop = loop;
}

cxPoint2FArray &cxChainBody::Points()
{
    return ps;
}

cxBool cxChainBody::CreateFixture(cxWorld *pw)
{
    b2Vec2 *points = new b2Vec2[ps.Size()]();
    for(cxInt i=0;i<ps.Size();i++){
        cxPoint2F &p = ps.At(i);
        points[i] = ToWorld(p);
    }
    if(isloop){
        shape.CreateLoop(points, ps.Size());
    }else{
        shape.CreateChain(points, ps.Size());
    }
    fixDef.shape = &shape;
    fixture = body->CreateFixture(&fixDef);
    delete []points;
    return fixture != nullptr;
}

CX_IMPLEMENT(cxPolygonBody)

cxPolygonBody::cxPolygonBody()
{
    
}

cxPolygonBody::~cxPolygonBody()
{
    
}

cxPoint2FArray &cxPolygonBody::Points()
{
    return ps;
}

cxBool cxPolygonBody::CreateFixture(cxWorld *pw)
{
    CX_ASSERT(ps.Size() >= 3 && ps.Size() <= b2_maxPolygonVertices, "point count error");
    b2Vec2 *points = new b2Vec2[ps.Size()]();
    for(cxInt i=0;i<ps.Size();i++){
        cxPoint2F &p = ps.At(i);
        points[i] = ToWorld(p);
    }
    shape.Set(points, ps.Size());
    fixDef.shape = &shape;
    fixture = body->CreateFixture(&fixDef);
    delete []points;
    return fixture != nullptr;
}

CX_IMPLEMENT(cxEdgeBody)

cxEdgeBody::cxEdgeBody()
{
    lineWidth = 10;
}

cxEdgeBody::~cxEdgeBody()
{
    
}

void cxEdgeBody::SetLinePoint(const cxLineF &line)
{
    shape.Set(ToWorld(line.a),ToWorld(line.b));
}

cxBool cxEdgeBody::CreateFixture(cxWorld *pw)
{
    fixDef.shape = &shape;
    fixture = body->CreateFixture(&fixDef);
    return fixture != nullptr;
}

CX_IMPLEMENT(cxBoxBody)

cxBoxBody::cxBoxBody()
{
    
}

cxBoxBody::~cxBoxBody()
{
    
}

cxBool cxBoxBody::CreateFixture(cxWorld *pw)
{
    const cxSize2F &size = Size();
    shape.SetAsBox(ToWorld(size.w/2.0f), ToWorld(size.h/2.0f));
    fixDef.shape = &shape;
    fixture = body->CreateFixture(&fixDef);
    return fixture != nullptr;
}

CX_IMPLEMENT(cxCircleBody)

cxCircleBody::cxCircleBody()
{
    
}

cxCircleBody::~cxCircleBody()
{
    
}

cxBool cxCircleBody::CreateFixture(cxWorld *pw)
{
    const cxSize2F &size = Size();
    shape.m_radius = ToWorld(size.w/2.0);
    fixDef.shape = &shape;
    fixture = body->CreateFixture(&fixDef);
    return fixture != nullptr;
}

CX_IMPLEMENT(cxBody)

cxBody::cxBody()
{
    fixture = nullptr;
    body = nullptr;
    bodyDef.type = b2_dynamicBody;
    fixDef.restitution = 0.0f;
    fixDef.density = 1.0f;
    fixDef.friction = 0.0f;
}

cxBody::~cxBody()
{
    
}

cxFloat cxBody::GetMass()
{
    CX_ASSERT(body != nullptr, "body not init");
    return body->GetMass();
}

void cxBody::SetLinearDamping(cxFloat linearDamping)
{
    if(body == nullptr){
        bodyDef.linearDamping = linearDamping;
        return;
    }
    body->SetLinearDamping(linearDamping);
}

void cxBody::SetAngularDamping(cxFloat angularDamping)
{
    if(body == nullptr){
        bodyDef.angularDamping = angularDamping;
        return;
    }
    body->SetAngularDamping(angularDamping);
}

void cxBody::SetGravityScale(cxFloat scale)
{
    if(body == nullptr){
        bodyDef.gravityScale = scale;
        return;
    }
    body->SetGravityScale(scale);
}

void cxBody::ApplyLinearImpulse(const cxPoint2F &impulse, const cxPoint2F &point, cxBool wake)
{
    CX_ASSERT(body != nullptr, "body not init");
    body->ApplyLinearImpulse(b2Vec2(impulse.x, impulse.y), b2Vec2(point.x, point.y), wake);
}

void cxBody::ApplyAngularImpulse(cxFloat impulse, cxBool wake)
{
    CX_ASSERT(body != nullptr, "body not init");
    body->ApplyAngularImpulse(impulse, wake);
}

void cxBody::ApplyForce(const cxPoint2F &force, const cxPoint2F &point, cxBool wake)
{
    CX_ASSERT(body != nullptr, "body not init");
    body->ApplyForce(b2Vec2(force.x, force.y), b2Vec2(point.x, point.y), wake);
}

void cxBody::ApplyForceToCenter(const cxPoint2F &force, cxBool wake)
{
    CX_ASSERT(body != nullptr, "body not init");
    body->ApplyForceToCenter(b2Vec2(force.x, force.y), wake);
}

void cxBody::SetLinearVelocity(const cxPoint2F &v)
{
    if(body == nullptr){
        bodyDef.linearVelocity = b2Vec2(v.x, v.y);
        return;
    }
    body->SetLinearVelocity(b2Vec2(v.x, v.y));
}

void cxBody::SetAngularVelocity(const cxFloat &v)
{
    if(body == nullptr){
        bodyDef.angularVelocity = v;
        return;
    }
    body->SetAngularVelocity(v);
}

cxBool cxBody::IsStatic()
{
    return bodyDef.type == b2_staticBody;
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
    cxView::SetPosition(FromWorld(pos));
    cxView::SetAngle(body->GetAngle());
}

cxView *cxBody::SetPosition(const cxPoint2F &v)
{
    if(body == nullptr){
        bodyDef.position = ToWorld(v);
        return this;
    }
    const b2Vec2 &pos = body->GetPosition();
    cxPoint2F np = FromWorld(pos);
    if(np == v){
        return this;
    }
    body->SetTransform(ToWorld(v), body->GetAngle());
    return this;
}

cxView *cxBody::SetAngle(const cxFloat &v)
{
    if(body == nullptr){
        bodyDef.angle = v;
        return this;
    }
    const cxFloat angle = body->GetAngle();
    if(cxFloatIsEqual(angle, v)){
        return this;
    }
    body->SetTransform(body->GetPosition(), v);
    return this;
}

cxBool cxBody::InitBody(b2BodyDef *def)
{
    return true;
}

cxBool cxBody::CreateBody(cxWorld *pw)
{
    body = pw->CreateBody(&bodyDef);
    return body != nullptr;
}

cxBool cxBody::InitFixture(b2FixtureDef *def)
{
    return true;
}

cxBool cxBody::CreateFixture(cxWorld *pw)
{
    fixture = body->CreateFixture(&fixDef);
    return fixture != nullptr;
}

CX_IMPLEMENT(cxWorld);

cxCollideBody::cxCollideBody(b2Contact *pcontact)
{
    Contact = pcontact;
    Fixture = nullptr;
    Body = nullptr;
}

void cxCollideBody::UseA()
{
    CX_ASSERT(Contact != nullptr, "contact not init");
    Fixture = Contact->GetFixtureA();
    Body = static_cast<cxBody *>(Fixture->GetBody()->GetUserData());
}

void cxCollideBody::UseB()
{
    CX_ASSERT(Contact != nullptr, "contact not init");
    Fixture = Contact->GetFixtureB();
    Body = static_cast<cxBody *>(Fixture->GetBody()->GetUserData());
}

cxCollideBody::cxCollideBody(b2Fixture *fixture)
{
    Contact = nullptr;
    Fixture = fixture;
    Body = static_cast<cxBody *>(fixture->GetBody()->GetUserData());
}

cxWorld::cxWorld():world(b2Vec2(0, -10.0f))
{
    vIters = 1;
    pIters = 1;
    world.SetContactFilter(this);
    world.SetContactListener(this);
}

cxWorld::~cxWorld()
{
    
}

cxBool cxWorld::ShouldCollide(cxCollideBody *a,cxCollideBody *b)
{
    b2Vec2 g = world.GetGravity();
    g.y = CX_RAND_11f() * 5.0f;
    g.x = CX_RAND_11f() * 5.0f;
    world.SetGravity(g);
    return true;
}

bool cxWorld::ShouldCollide(b2Fixture* fixtureA, b2Fixture* fixtureB)
{
    cxCollideBody a(fixtureA);
    cxCollideBody b(fixtureB);
    return ShouldCollide(&a, &b);;
}

void cxWorld::BeginContact(cxCollideBody *a,cxCollideBody *b)
{
    
}

void cxWorld::BeginContact(b2Contact* contact)
{
    cxCollideBody a(contact);a.UseA();
    cxCollideBody b(contact);b.UseB();
    BeginContact(&a, &b);
}

cxBool cxWorld::PreSolve(cxCollideBody *a,cxCollideBody *b)
{
    return true;
}

void cxWorld::PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
{
    cxCollideBody a(contact);a.UseA();
    cxCollideBody b(contact);b.UseB();
    contact->SetEnabled(PreSolve(&a, &b));
}

void cxWorld::PostSolve(cxCollideBody *a,cxCollideBody *b)
{
    
}

void cxWorld::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
{
    cxCollideBody a(contact);a.UseA();
    cxCollideBody b(contact);b.UseB();
    PostSolve(&a, &b);
}

void cxWorld::EndContact(cxCollideBody *a,cxCollideBody *b)
{
    
}

void cxWorld::EndContact(b2Contact* contact)
{
    cxCollideBody a(contact);a.UseA();
    cxCollideBody b(contact);b.UseB();
    EndContact(&a, &b);
}

b2Body *cxWorld::CreateBody(const b2BodyDef* def)
{
    return world.CreateBody(def);
}

cxBody *cxWorld::AppendBody(cxBody *body)
{
    if(!body->InitBody(&body->bodyDef)){
        return nullptr;
    }
    if(!body->CreateBody(this)){
        return nullptr;
    }
    if(!body->InitFixture(&body->fixDef)){
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
    if(body->body == nullptr){
        return;
    }
    if(body->fixture != nullptr){
        body->body->DestroyFixture(body->fixture);
        body->fixture = nullptr;
    }
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

