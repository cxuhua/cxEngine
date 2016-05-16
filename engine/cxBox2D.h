//
//  cxBox2D.h
//  cxEngineCore
//
//  Created by xuhua on 11/2/15.
//  Copyright © 2015 xuhua. All rights reserved.
//

#ifndef cxEngineCore_cxBox2D_h
#define cxEngineCore_cxBox2D_h

#include <Box2D/Box2D.h>
#include <math/cxLineF.h>
#include "cxAtlas.h"

//1 pixel = 0.01m
#define PTM_RATIO       100.0f

CX_CPP_BEGIN

class cxBoxWorld;

class cxBody : public cxView
{
public:
    CX_DECLARE(cxBody);
    friend cxBoxWorld;
protected:
    explicit cxBody();
    virtual ~cxBody();
protected:
    cxBoxWorld *world;
    b2Fixture *fixture;
    b2Body *body;
    b2BodyDef bodyDef;
    b2FixtureDef fixDef;
    void OnUpdate(cxFloat dt);
public:
    virtual cxBool InitFixture(b2FixtureDef *def);
    virtual cxBool CreateFixture(cxBoxWorld *pw);
    virtual void DestroyFixture();
    virtual cxBool InitBody(b2BodyDef *def);
    virtual cxBool CreateBody(cxBoxWorld *pw);
public:
    void SetWorld(cxBoxWorld *pw);
    b2Body *GetBody();
    cxBody *SetFilter(const b2Filter &v);
    //
    cxBody *SetStatic(cxBool v);
    cxBool IsStatic();
    //
    cxView *SetPosition(const cxPoint2F &v);
    cxView *SetAngle(const cxFloat &v);
    //
    cxBody *SetAngularVelocity(const cxFloat &v);
    const cxFloat GetAngularVelocity() const;
    
    cxBody *SetLinearVelocity(const cxPoint2F &v);
    const cxPoint2F GetLinearVelocity() const;
    
    cxBody *SetElasticity(cxFloat v);
    
    cxBody *SetDensity(cxFloat v);
    const cxFloat GetDensity() const;
    
    cxBody *SetFriction(cxFloat v);
    cxBody *SetLinearDamping(cxFloat linearDamping);
    cxBody *SetAngularDamping(cxFloat angularDamping);
    cxBody *SetGravityScale(cxFloat scale);
    //
    cxBody *ApplyForce(const cxPoint2F &force, const cxPoint2F &point, cxBool wake=true);
    cxBody *ApplyForceToCenter(const cxPoint2F &force, cxBool wake=true);
    cxBody *ApplyLinearImpulse(const cxPoint2F &impulse, const cxPoint2F &point, cxBool wake=true);
    cxBody *ApplyAngularImpulse(cxFloat impulse, cxBool wake=true);

    cxFloat GetMass();
};

class cxEdgeBody : public cxBody
{
public:
    CX_DECLARE(cxEdgeBody);
protected:
    explicit cxEdgeBody();
    virtual ~cxEdgeBody();
private:
    b2EdgeShape shape;
public:
    cxLineF GetLinePoint();
    void SetLinePoint(const cxLineF &line);
    cxBool CreateFixture(cxBoxWorld *pw);
};

class cxCircleBody : public cxBody
{
public:
    CX_DECLARE(cxCircleBody);
protected:
    explicit cxCircleBody();
    virtual ~cxCircleBody();
private:
    b2CircleShape shape;
public:
    cxFloat Radius();
    void SetRadius(const cxFloat &v);
    
    cxPoint2F Center();
    void SetCenter(const cxPoint2F &v);
    
    cxBool CreateFixture(cxBoxWorld *pw);
};

class cxBoxBody : public cxBody
{
public:
    CX_DECLARE(cxBoxBody);
protected:
    explicit cxBoxBody();
    virtual ~cxBoxBody();
private:
    b2PolygonShape shape;
public:
    cxBool CreateFixture(cxBoxWorld *pw);
};

class cxPolygonBody : public cxBody
{
public:
    CX_DECLARE(cxPolygonBody);
protected:
    explicit cxPolygonBody();
    virtual ~cxPolygonBody();
private:
    cxPoint2FArray ps;
    b2PolygonShape shape;
public:
    cxPoint2FArray &Points();
    cxBool CreateFixture(cxBoxWorld *pw);
};

class cxChainBody : public cxBody
{
public:
    CX_DECLARE(cxChainBody);
protected:
    explicit cxChainBody();
    virtual ~cxChainBody();
private:
    cxBool isloop;
    cxPoint2FArray ps;
    b2ChainShape shape;
public:
    void SetLoop(cxBool loop);
    cxPoint2FArray &Points();
    cxBool CreateFixture(cxBoxWorld *pw);
};

struct cxBodyInfo
{
    cxBodyInfo();
    cxBodyInfo(b2Fixture *fixture);
    cxBody      *Body;
    b2Fixture   *Fixture;
};

struct cxContactInfo
{
    cxContactInfo(b2Contact *pcontact);
    b2Contact *Contact;
    cxBodyInfo A;
    cxBodyInfo B;
    const b2Manifold* Manifold;
    const b2ContactImpulse* Impulse;
    //You can call this in PreSolve
    void SetFriction(float32 friction);
    void SetRestitution(float32 restitution);
};

struct cxBoxQueryInfo : public b2QueryCallback
{
    cxBoxQueryInfo(){};
    bool ReportFixture(b2Fixture* fixture);
    std::function<cxBool(cxBodyInfo *)> *Func;
};

struct cxRayCastReport
{
    cxBodyInfo Info;
    cxPoint2F Point;
    cxPoint2F Normal;
    cxFloat Fraction;
};

struct cxRayCastInfo : public b2RayCastCallback
{
    cxRayCastInfo();
    float32 ReportFixture(b2Fixture* fixture, const b2Vec2& point,const b2Vec2& normal, float32 fraction);
    std::function<cxBool(cxRayCastReport *)> *Func;
};

class cxBoxWorld : public cxView,public b2ContactListener,public b2ContactFilter
{
public:
    CX_DECLARE(cxBoxWorld);
    friend cxBody;
protected:
    explicit cxBoxWorld();
    virtual ~cxBoxWorld();
private:
    float32 vIters;
    float32 pIters;
    b2Body *createBody(const b2BodyDef* def);
    void destroyBody(cxBody *body);
protected:
    void OnUpdate(cxFloat dt);
    b2World world;
    void OnRemove(cxView *pview);
    void OnAppend(cxView *nview);
private:
    //b2ContactListener
    void BeginContact(b2Contact* contact);
    void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);
    void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);
    void EndContact(b2Contact* contact);
public:
    //
    static b2Vec2 ToWorld(const cxPoint2F &v);
    static cxFloat ToWorld(const cxFloat &v);
    static cxPoint2F FromWorld(const b2Vec2 &v);
    static cxFloat FromWorld(const cxFloat &v);
    //b2ContactFilter
    bool ShouldCollide(b2Fixture* fixtureA, b2Fixture* fixtureB);
    //
    virtual cxBool ShouldCollide(cxBodyInfo *a,cxBodyInfo *b);
    virtual void BeginContact(cxContactInfo *contact);
    virtual cxBool PreSolve(cxContactInfo *contact);
    virtual void PostSolve(cxContactInfo *contact);
    virtual void EndContact(cxContactInfo *contact);
    //
    //func return true continue,false terminate
    void QueryBox(const cxBox4F &box,std::function<cxBool(cxBodyInfo *)> func);
    //
    //func return -1 to filter, 0 to terminate, fraction to clip the ray for closest hit, 1 to continue
    void RayCast(const cxLineF &line,std::function<cxBool(cxRayCastReport *)> func);
    //
    void SetGravity(const cxPoint2F &v);
};

inline b2Vec2 cxBoxWorld::ToWorld(const cxPoint2F &v)
{
    return b2Vec2(ToWorld(v.x), ToWorld(v.y));
}

inline cxFloat cxBoxWorld::ToWorld(const cxFloat &v)
{
    return v/PTM_RATIO;
}

inline cxPoint2F cxBoxWorld::FromWorld(const b2Vec2 &v)
{
    return cxPoint2F(FromWorld(v.x), FromWorld(v.y));
}

inline cxFloat cxBoxWorld::FromWorld(const cxFloat &v)
{
    return v*PTM_RATIO;
}

CX_CPP_END

#endif


