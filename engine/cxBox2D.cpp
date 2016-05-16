//
//  cxBox2D.cpp
//  cxEngineCore
//
//  Created by xuhua on 11/2/15.
//  Copyright © 2015 xuhua. All rights reserved.
//

#include "cxBox2D.h"

CX_CPP_BEGIN

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

cxBool cxChainBody::CreateFixture(cxBoxWorld *pw)
{
    //use view points
    if(ps.Size() == 0){
        ps.Append(ToBox4F().Points());
        isloop = true;
    }
    if(ps.Size() == 0){
        return false;
    }
    b2Vec2 *points = new b2Vec2[ps.Size()]();
    for(cxInt i=0;i<ps.Size();i++){
        cxPoint2F &p = ps.At(i);
        points[i] = cxBoxWorld::ToWorld(p);
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

cxBool cxPolygonBody::CreateFixture(cxBoxWorld *pw)
{
    CX_ASSERT(ps.Size() >= 3 && ps.Size() <= b2_maxPolygonVertices, "point count error");
    b2Vec2 *points = new b2Vec2[ps.Size()]();
    for(cxInt i=0;i<ps.Size();i++){
        cxPoint2F &p = ps.At(i);
        points[i] = cxBoxWorld::ToWorld(p);
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
    
}

cxEdgeBody::~cxEdgeBody()
{
    
}

void cxEdgeBody::SetLinePoint(const cxLineF &line)
{
    shape.Set(cxBoxWorld::ToWorld(line.a),cxBoxWorld::ToWorld(line.b));
}

cxLineF cxEdgeBody::GetLinePoint()
{
    cxPoint2F p1 = cxBoxWorld::FromWorld(shape.m_vertex1);
    cxPoint2F p2 = cxBoxWorld::FromWorld(shape.m_vertex2);
    return cxLineF(p1, p2);
}

cxBool cxEdgeBody::CreateFixture(cxBoxWorld *pw)
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

cxBool cxBoxBody::CreateFixture(cxBoxWorld *pw)
{
    const cxSize2F &size = Size();
    shape.SetAsBox(cxBoxWorld::ToWorld(size.w/2.0f), cxBoxWorld::ToWorld(size.h/2.0f));
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

cxFloat cxCircleBody::Radius()
{
    return cxBoxWorld::FromWorld(shape.m_radius);
}

void cxCircleBody::SetRadius(const cxFloat &v)
{
    shape.m_radius = cxBoxWorld::ToWorld(v);
}

cxPoint2F cxCircleBody::Center()
{
    return cxBoxWorld::FromWorld(shape.m_p);
}

void cxCircleBody::SetCenter(const cxPoint2F &v)
{
    shape.m_p = cxBoxWorld::ToWorld(v);
}

cxBool cxCircleBody::CreateFixture(cxBoxWorld *pw)
{
    if(cxFloatIsEqual(shape.m_radius, 0)){
        const cxSize2F &size = Size();
        cxFloat max = CX_MAX(size.w, size.h);
        shape.m_radius = cxBoxWorld::ToWorld(max/2.0);
    }
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

cxBody *cxBody::SetLinearDamping(cxFloat linearDamping)
{
    if(body == nullptr){
        bodyDef.linearDamping = linearDamping;
        return this;
    }
    body->SetLinearDamping(linearDamping);
    return this;
}

cxBody *cxBody::SetAngularDamping(cxFloat angularDamping)
{
    if(body == nullptr){
        bodyDef.angularDamping = angularDamping;
        return this;
    }
    body->SetAngularDamping(angularDamping);
    return this;
}

cxBody *cxBody::SetGravityScale(cxFloat scale)
{
    if(body == nullptr){
        bodyDef.gravityScale = scale;
        return this;
    }
    body->SetGravityScale(scale);
    return this;
}

cxBody *cxBody::ApplyLinearImpulse(const cxPoint2F &impulse, const cxPoint2F &point, cxBool wake)
{
    CX_ASSERT(body != nullptr, "body not init");
    body->ApplyLinearImpulse(b2Vec2(impulse.x, impulse.y), b2Vec2(point.x, point.y), wake);
    return this;
}

cxBody *cxBody::ApplyAngularImpulse(cxFloat impulse, cxBool wake)
{
    CX_ASSERT(body != nullptr, "body not init");
    body->ApplyAngularImpulse(impulse, wake);
    return this;
}

cxBody *cxBody::ApplyForce(const cxPoint2F &force, const cxPoint2F &point, cxBool wake)
{
    CX_ASSERT(body != nullptr, "body not init");
    body->ApplyForce(b2Vec2(force.x, force.y), b2Vec2(point.x, point.y), wake);
    return this;
}

cxBody *cxBody::ApplyForceToCenter(const cxPoint2F &force, cxBool wake)
{
    CX_ASSERT(body != nullptr, "body not init");
    body->ApplyForceToCenter(b2Vec2(force.x, force.y), wake);
    return this;
}

cxBody *cxBody::SetLinearVelocity(const cxPoint2F &v)
{
    if(body == nullptr){
        bodyDef.linearVelocity = b2Vec2(v.x, v.y);
        return this;
    }
    body->SetLinearVelocity(b2Vec2(v.x, v.y));
    return this;
}

const cxPoint2F cxBody::GetLinearVelocity() const
{
    b2Vec2 v;
    if(body == nullptr){
        v = bodyDef.linearVelocity;
    }else{
        v = body->GetLinearVelocity();
    }
    return cxPoint2F(v.x, v.y);
}

cxBody *cxBody::SetAngularVelocity(const cxFloat &v)
{
    if(body == nullptr){
        bodyDef.angularVelocity = v;
        return this;
    }
    body->SetAngularVelocity(v);
    return this;
}

const cxFloat cxBody::GetAngularVelocity() const
{
    return body==nullptr?bodyDef.angularVelocity:body->GetAngularVelocity();
}

cxBody *cxBody::SetFilter(const b2Filter &v)
{
    if(fixture == nullptr){
        fixDef.filter = v;
    }else{
        fixture->SetFilterData(v);
    }
    return this;
}

cxBool cxBody::IsStatic()
{
    return bodyDef.type == b2_staticBody;
}

cxBody *cxBody::SetStatic(cxBool v)
{
    bodyDef.type = v?b2_staticBody:b2_dynamicBody;
    return this;
}

cxBody *cxBody::SetElasticity(cxFloat v)
{
    fixDef.restitution = v;
    return this;
}

cxBody *cxBody::SetDensity(cxFloat v)
{
    fixDef.density = v;
    return this;
}

const cxFloat cxBody::GetDensity() const
{
    return fixDef.density;
}

cxBody *cxBody::SetFriction(cxFloat v)
{
    fixDef.friction = v;
    return this;
}

void cxBody::OnUpdate(cxFloat dt)
{
    cxView::OnUpdate(dt);
    const b2Vec2 &pos = body->GetPosition();
    cxView::SetPosition(cxBoxWorld::FromWorld(pos));
    cxView::SetAngle(body->GetAngle());
}

cxView *cxBody::SetPosition(const cxPoint2F &v)
{
    if(body == nullptr){
        bodyDef.position = cxBoxWorld::ToWorld(v);
        return this;
    }
    const b2Vec2 &pos = body->GetPosition();
    cxPoint2F np = cxBoxWorld::FromWorld(pos);
    if(np == v){
        return this;
    }
    body->SetTransform(cxBoxWorld::ToWorld(v), body->GetAngle());
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

void cxBody::SetWorld(cxBoxWorld *pw)
{
    world = pw;
}

b2Body *cxBody::GetBody()
{
    return body;
}

cxBool cxBody::CreateBody(cxBoxWorld *pw)
{
    body = pw->createBody(&bodyDef);
    return body != nullptr;
}

cxBool cxBody::InitFixture(b2FixtureDef *def)
{
    return true;
}

void cxBody::DestroyFixture()
{
    if(fixture == nullptr){
        return;
    }
    body->DestroyFixture(fixture);
    fixture = nullptr;
}

cxBool cxBody::CreateFixture(cxBoxWorld *pw)
{
    fixture = body->CreateFixture(&fixDef);
    return fixture != nullptr;
}

CX_IMPLEMENT(cxBoxWorld);

cxBodyInfo::cxBodyInfo()
{
    Fixture = nullptr;
    Body = nullptr;
}

cxBodyInfo::cxBodyInfo(b2Fixture *fixture)
{
    Fixture = fixture;
    Body = static_cast<cxBody *>(Fixture->GetBody()->GetUserData());
}

cxContactInfo::cxContactInfo(b2Contact *pcontact)
{
    Contact = pcontact;
    A = cxBodyInfo(Contact->GetFixtureA());
    B = cxBodyInfo(Contact->GetFixtureB());
    Manifold = nullptr;
    Impulse = nullptr;
}

void cxContactInfo::SetFriction(float32 friction)
{
    CX_ASSERT(Contact != nullptr, "Contact null");
    Contact->SetFriction(friction);
}

void cxContactInfo::SetRestitution(float32 restitution)
{
    CX_ASSERT(Contact != nullptr, "Contact null");
    Contact->SetRestitution(restitution);
}

cxBoxWorld::cxBoxWorld():world(b2Vec2(0, -10.0f))
{
    vIters = 1;
    pIters = 1;
    world.SetContactFilter(this);
    world.SetContactListener(this);
}

cxBoxWorld::~cxBoxWorld()
{
    
}

cxBool cxBoxWorld::ShouldCollide(cxBodyInfo *a,cxBodyInfo *b)
{
    return true;
}

bool cxBoxWorld::ShouldCollide(b2Fixture* fixtureA, b2Fixture* fixtureB)
{
    cxBodyInfo a(fixtureA);
    cxBodyInfo b(fixtureB);
    return ShouldCollide(&a, &b);;
}

void cxBoxWorld::BeginContact(cxContactInfo *contact)
{
    
}

void cxBoxWorld::BeginContact(b2Contact* contact)
{
    cxContactInfo info(contact);
    BeginContact(&info);
}

cxBool cxBoxWorld::PreSolve(cxContactInfo *contact)
{
    return true;
}

void cxBoxWorld::PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
{
    cxContactInfo info(contact);
    info.Manifold = oldManifold;
    PreSolve(&info);
}

void cxBoxWorld::PostSolve(cxContactInfo *contact)
{
    
}

void cxBoxWorld::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
{
    cxContactInfo info(contact);
    info.Impulse = impulse;
    PostSolve(&info);
}

void cxBoxWorld::EndContact(cxContactInfo *contact)
{
    
}

void cxBoxWorld::EndContact(b2Contact* contact)
{
    cxContactInfo info(contact);
    EndContact(&info);
}

cxRayCastInfo::cxRayCastInfo()
{
    
}

float32 cxRayCastInfo::ReportFixture(b2Fixture* fixture, const b2Vec2& point,const b2Vec2& normal, float32 fraction)
{
    cxRayCastReport info;
    info.Info = cxBodyInfo(fixture);
    info.Point = cxBoxWorld::FromWorld(point);
    info.Normal = cxPoint2F(normal.x, normal.y);
    info.Fraction = fraction;
    return (*Func)(&info);
}

void cxBoxWorld::RayCast(const cxLineF &line,std::function<cxBool(cxRayCastReport *)> func)
{
    cxRayCastInfo info;
    info.Func = &func;
    world.RayCast(&info, ToWorld(line.a), ToWorld(line.b));
}

bool cxBoxQueryInfo::ReportFixture(b2Fixture* fixture)
{
    cxBodyInfo info(fixture);
    return (*Func)(&info);
}

void cxBoxWorld::QueryBox(const cxBox4F &box,std::function<cxBool(cxBodyInfo *)> func)
{
    cxBoxQueryInfo info;
    info.Func = &func;
    b2AABB ab;
    ab.lowerBound = ToWorld(box.LB());
    ab.upperBound = ToWorld(box.RT());
    world.QueryAABB(&info, ab);
}

b2Body *cxBoxWorld::createBody(const b2BodyDef* def)
{
    return world.CreateBody(def);
}

void cxBoxWorld::destroyBody(cxBody *body)
{
    body->DestroyFixture();
    world.DestroyBody(body->body);
    body->body = nullptr;
}

void cxBoxWorld::OnRemove(cxView *pview)
{
    cxView::OnRemove(pview);
    destroyBody(pview->To<cxBody>());
}

void cxBoxWorld::OnAppend(cxView *nview)
{
    cxBody *body = nview->To<cxBody>();
    body->SetWorld(this);
    cxView::OnAppend(body);
    if(!body->InitBody(&body->bodyDef)){
        CX_ASSERT(false, "init body error");
        return;
    }
    if(!body->CreateBody(this)){
        CX_ASSERT(false, "create body error");
        return;
    }
    if(!body->InitFixture(&body->fixDef)){
        CX_ASSERT(false, "init fixture error");
        return;
    }
    if(!body->CreateFixture(this)){
        CX_ASSERT(false, "create fixture error");
        return;
    }
    body->body->SetUserData(body);
}

void cxBoxWorld::OnUpdate(cxFloat dt)
{
    cxView::OnUpdate(dt);
    world.Step(dt, vIters, pIters);
}

void cxBoxWorld::SetGravity(const cxPoint2F &v)
{
    world.SetGravity(b2Vec2(v.x, v.y));
}

CX_CPP_END

