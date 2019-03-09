//
//  cxBoxBody.cpp
//  cxEngineTest
//
//  Created by 徐华 on 2019/2/28.
//  Copyright © 2019 xuhua. All rights reserved.
//

#include <Box2D/Box2D.h>
#include "cxBoxBody.h"
#include "cxBoxWorld.h"


CX_CPP_BEGIN

CX_IMPLEMENT(cxBoxBody);

cxBoxBody::cxBoxBody()
{
    body = nullptr;
    fixture = nullptr;
    world = nullptr;
}

cxBoxBody::~cxBoxBody()
{
    if(body != nullptr){
        world->GetWorld()->DestroyBody(body);
    }
}

b2Fixture *cxBoxBody::GetFixture()
{
    CX_ASSERT(fixture != nullptr, "body not init");
    return fixture;
}

b2Body *cxBoxBody::GetBody()
{
    CX_ASSERT(body != nullptr, "body not init");
    return body;
}

void cxBoxBody::OnLeave()
{
    CX_ASSERT(body != nullptr, "body not init");
    world->GetWorld()->DestroyBody(body);
    body = nullptr;
}

void cxBoxBody::OnAttchView(cxView *pv)
{
    CX_ASSERT(body != nullptr, "body not init");
    cxViewExt::OnAttchView(pv);
    b2Vec2 mv = world->ToMeters(pv->Position());
    const cxFloat angle = pv->Angle();
    body->SetTransform(mv, angle);
}

void cxBoxBody::OnUpdate(const cxFloat &dt)
{
    CX_ASSERT(body != nullptr, "body not init");
    const b2Vec2 &pos = body->GetPosition();
    const float32 angle = body->GetAngle();
    cxPoint2F pv = world->ToPixel(pos);
    pview->SetPosition(pv,false);
    pview->SetAngle(angle,false);
}

bool cxBoxBody::SetPosition(const cxPoint2F &v)
{
    CX_ASSERT(body != nullptr, "body not init");
    const float32 angle = body->GetAngle();
    b2Vec2 mv = world->ToMeters(v);
    body->SetTransform(mv, angle);
    return true;
}

bool cxBoxBody::SetAngle(const cxFloat &v)
{
    CX_ASSERT(body != nullptr, "body not init");
    const b2Vec2 &pos = body->GetPosition();
    body->SetTransform(pos, v);
    return true;
}

CX_CPP_END

