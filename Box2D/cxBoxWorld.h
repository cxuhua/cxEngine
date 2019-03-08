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

class cxBoxWorld : public cxView
{
public:
    CX_DECLARE(cxBoxWorld);
protected:
    explicit cxBoxWorld();
    virtual ~cxBoxWorld();
private:
    cxFloat pscale;
    b2World *world;
protected:
    virtual cxFloat PixelScale();
    void OnUpdate(cxFloat dt);
public:
    b2World *GetWorld();
    void AppendViewExt(cxView *pv,cxViewExt *ext);
    void SetGravity(const cxPoint2F &v);
    cxBoxBody *CreateBox(const cxPoint2F &v,b2BodyType type);
public:
    cxPoint2F ToPixel(const b2Vec2 &v);
    b2Vec2 ToMeters(const cxPoint2F &v);
};

CX_CPP_END

#endif
