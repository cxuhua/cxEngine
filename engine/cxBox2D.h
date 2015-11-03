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
#include "cxAtlas.h"

#define CX_BOX2D_BEGIN  CX_CPP_BEGIN namespace cxBox2D {
#define CX_BOX2D_END    } CX_CPP_END

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
    cxView *SetPosition(const cxPoint2F &v);
    cxView *SetAngle(const cxFloat &v);
    
    virtual cxBool CreateFixture(cxWorld *pw);
    virtual cxBool CreateBody(cxWorld *pw);
    
    void SetStatic(cxBool v);
    void SetElasticity(cxFloat v);
    void SetDensity(cxFloat v);
    void SetFriction(cxFloat v);
};

class cxEdgeBody : public cxBody
{
public:
    CX_DECLARE(cxEdgeBody);
protected:
    explicit cxEdgeBody();
    virtual ~cxEdgeBody();
private:
    cxFloat lineWidth;
    cxPoint2F v1;
    cxPoint2F v2;
    b2EdgeShape *shape;
public:
    void UpdateEdge();
    void SetLineWidth(const cxFloat &v);
    void Set(const cxPoint2F &p1,const cxPoint2F &p2);
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
    b2CircleShape *shape;
public:
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
    b2PolygonShape *shape;
public:
    cxBool CreateFixture(cxWorld *pw);
};

class cxWorld : public cxView,public b2ContactListener,public b2ContactFilter,public b2DestructionListener
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
    void OnUpdate(cxFloat dt);
    b2World world;
    void OnRemove(cxView *pview);
public:
    //b2DestructionListener
    void SayGoodbye(b2Joint* joint);
    void SayGoodbye(b2Fixture* fixture);
    //b2ContactFilter
    bool ShouldCollide(b2Fixture* fixtureA, b2Fixture* fixtureB);
    //b2ContactListener
    void BeginContact(b2Contact* contact);
    void EndContact(b2Contact* contact);
    void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);
    void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);
    
    void SetGravity(const cxPoint2F &v);
    
    b2Body *CreateBody(const b2BodyDef* def);
    
    cxBody *AppendBody(cxBody *body);
};

CX_BOX2D_END

#endif


