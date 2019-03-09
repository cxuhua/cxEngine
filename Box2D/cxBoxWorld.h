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
#include "cxBoxBody.h"

CX_CPP_BEGIN

#define PTM_RATIO 32.0f

class cxBoxWorld : public cxView
{
public:
    CX_DECLARE(cxBoxWorld);
protected:
    explicit cxBoxWorld();
    virtual ~cxBoxWorld();
private:
    b2World *world;
protected:
    void OnUpdate(cxFloat dt);
public:
    b2World *GetWorld();
    void Append(cxView *pv,cxViewExt *ext);
    void SetGravity(const cxPoint2F &v);
    cxBoxBody *CreateBox(const cxPoint2F &v,b2BodyType type);
    cxBoxBody *CreateCircle(const cxFloat &r,b2BodyType type);
public:
    cxPoint2F ToPixel(const b2Vec2 &v);
    b2Vec2 ToMeters(const cxPoint2F &v);
    cxFloat ToPixel(const cxFloat &v);
    cxFloat ToMeters(const cxFloat &v);
};

CX_CPP_END

#endif
