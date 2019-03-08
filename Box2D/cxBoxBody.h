//
// cxBoxBody.h
// cxEngineTest
//
// Created by 徐华 on 2019/2/28.
//Copyright © 2019 xuhua. All rights reserved.
//

#ifndef cxBoxBody_h
#define cxBoxBody_h

#include <Box2D/Box2D.h>
#include <engine/cxViewExt.h>

CX_CPP_BEGIN

#define cxPoint2FToVec2(v)  b2Vec2((v).x,(v).y)

class cxBoxWorld;
class cxBoxBody : public cxViewExt
{
public:
    CX_DECLARE(cxBoxBody);
public:
    friend cxBoxWorld;
    explicit cxBoxBody();
    virtual ~cxBoxBody();
public:
    void OnAttchView(cxView *pv);
    void OnUpdate(const cxFloat &dt);
    bool SetPosition(const cxPoint2F &v);
    bool SetAngle(const cxFloat &v);
    void OnLeave();
private:
    cxBoxWorld *world;
    b2Body *body;
    b2Fixture *fixture;
public:
    b2Fixture *GetFixture();
    b2Body *GetBody();
};

CX_CPP_END

#endif
