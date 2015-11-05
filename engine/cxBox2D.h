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

#define CX_BOX2D_BEGIN  CX_CPP_BEGIN namespace cxBox2D {
#define CX_BOX2D_END    } CX_CPP_END
#define PTM_RATIO       100.0f

CX_BOX2D_BEGIN

class cxWorld;

class cxBody : public cxView
{
public:
    CX_DECLARE(cxBody);
    friend cxWorld;
protected:
    explicit cxBody();
    virtual ~cxBody();
protected:
    b2Fixture *fixture;
    b2Body *body;
    b2BodyDef bodyDef;
    b2FixtureDef fixDef;
    void OnUpdate(cxFloat dt);
public:
    virtual cxBool InitFixture(b2FixtureDef *def);
    virtual cxBool CreateFixture(cxWorld *pw);
    virtual void DestroyFixture();
    virtual cxBool InitBody(b2BodyDef *def);
    virtual cxBool CreateBody(cxWorld *pw);
public:
    
    void SetStatic(cxBool v);
    cxBool IsStatic();
    
    cxView *SetPosition(const cxPoint2F &v);
    cxView *SetAngle(const cxFloat &v);
    
    void SetAngularVelocity(const cxFloat &v);
    void SetLinearVelocity(const cxPoint2F &v);
    void SetElasticity(cxFloat v);
    void SetDensity(cxFloat v);
    void SetFriction(cxFloat v);
    void SetLinearDamping(cxFloat linearDamping);
    void SetAngularDamping(cxFloat angularDamping);
    void SetGravityScale(cxFloat scale);
    
    void ApplyForce(const cxPoint2F &force, const cxPoint2F &point, cxBool wake=true);
    void ApplyForceToCenter(const cxPoint2F &force, cxBool wake=true);
    void ApplyLinearImpulse(const cxPoint2F &impulse, const cxPoint2F &point, cxBool wake=true);
    void ApplyAngularImpulse(cxFloat impulse, cxBool wake=true);

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
    cxBool CreateFixture(cxWorld *pw);
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
    
    cxPoint2F GetCenter();
    void SetCenter(const cxPoint2F &v);
    
    cxBool CreateFixture(cxWorld *pw);
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
    cxBool CreateFixture(cxWorld *pw);
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
    cxBool CreateFixture(cxWorld *pw);
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
    cxBool CreateFixture(cxWorld *pw);
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
};

class cxBoxQueryInfo : public b2QueryCallback
{
public:
    cxBoxQueryInfo();
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

class cxRayCastInfo : public b2RayCastCallback
{
public:
    cxRayCastInfo();
    float32 ReportFixture(b2Fixture* fixture, const b2Vec2& point,const b2Vec2& normal, float32 fraction);
    std::function<cxBool(cxRayCastReport *)> *Func;
};

class cxWorld : public cxView,public b2ContactListener,public b2ContactFilter
{
public:
    CX_DECLARE(cxWorld);
protected:
    explicit cxWorld();
    virtual ~cxWorld();
private:
    float32 vIters;
    float32 pIters;
protected:
    void OnRender(cxRender *render, const cxMatrixF &model);
    void OnUpdate(cxFloat dt);
    b2World world;
    void OnRemove(cxView *pview);
public:
    //
    static b2Vec2 ToWorld(const cxPoint2F &v);
    static float32 ToWorld(const cxFloat &v);
    static cxPoint2F FromWorld(const b2Vec2 &v);
    static cxFloat FromWorld(const float32 &v);
    //b2ContactFilter
    bool ShouldCollide(b2Fixture* fixtureA, b2Fixture* fixtureB);
    //b2ContactListener
    void BeginContact(b2Contact* contact);
    void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);
    void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);
    void EndContact(b2Contact* contact);
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
    b2Body *CreateBody(const b2BodyDef* def);
    void DestroyBody(cxBody *body);
    cxBody *AppendBody(cxBody *body);
};

inline b2Vec2 cxWorld::ToWorld(const cxPoint2F &v)
{
    return b2Vec2(ToWorld(v.x), ToWorld(v.y));
}

inline float32 cxWorld::ToWorld(const cxFloat &v)
{
    return v/PTM_RATIO;
}

inline cxPoint2F cxWorld::FromWorld(const b2Vec2 &v)
{
    return cxPoint2F(FromWorld(v.x), FromWorld(v.y));
}

inline cxFloat cxWorld::FromWorld(const float32 &v)
{
    return v*PTM_RATIO;
}

CX_BOX2D_END

#endif


