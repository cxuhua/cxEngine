//
//  IOSEngine.cpp
//  cxEngine2D
//
//  Created by xuhua on 6/17/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#include <engine/cxSprite.h>
#include <engine/cxTexture.h>
#include <engine/cxAtlas.h>
#include <engine/cxLabel.h>
#include <core/cxUtil.h>
#include <engine/cxMove.h>
#include <engine/cxRotate.h>
#include <engine/cxScale.h>
#include <engine/cxButton.h>
#include <engine/cxContainer.h>
#include <engine/cxEmitter.h>
#include <engine/cxTimeLine.h>
#include <engine/cxTimer.h>
#include <engine/cxAnimate.h>
#include <engine/cxTcp.h>
#include <engine/cxHttp.h>
#include <engine/cxSpline.h>
#include <engine/cxFollow.h>
#include <engine/cxBezier.h>
#include <engine/cxFade.h>
#include <engine/cxTint.h>
#include <engine/cxSequence.h>
#include <engine/cxJump.h>
#include <engine/cxResize.h>
#include <engine/cxProgress.h>
#include <engine/cxTable.h>
#include <engine/cxPath.h>
#include <engine/cxTriangles.h>
#include <engine/cxMusic.h>
#include "Game.h"
#include <core/cxKDTree.h>
#include "Sea.h"
#include <engine/cxBox2D.h>
#include <engine/cxSpine.h>
#include <core/cxSqlite.h>
#include <engine/cxScript.h>

#include <raknet/cxClient.h>
#include <raknet/ListServers.h>

CX_CPP_BEGIN

cxClient *client = nullptr;

CX_IMPLEMENT(Game);

void Game::OnUpdate(cxFloat dt)
{
    client->Process();
    cxEngine::OnUpdate(dt);
}

Game::Game()
{
    client = cxClient::Alloc();
    
}

Game::~Game()
{
    
}

cxWorld *w;

void Game::OnDispatch(const cxTouchable *e)
{
    
    float r = CX_RAND_01f();
    float g = CX_RAND_01f();
    float b = CX_RAND_01f();
    const cxTouchPoint *t1 = e->TouchPoint(0);
    if(t1->type == cxTouchPoint::Ended){
        cxCircleBody *body = cxCircleBody::Create();
        body->SetRadius(25);
        body->SetPosition(t1->wp);
        body->SetElasticity(1.0);
        
        body->Invoke([](cxView *pview){
            pview->To<cxCircleBody>()->ApplyForceToCenter(cxPoint2F(800, 0));
        });
        
        w->Append(body);
        
        cxSprite *sp = cxSprite::Create();
        sp->SetColor(cxColor4F(r, g, b, 1.0));
        sp->SetSize(cxSize2F(50, 50));
        sp->SetTexture("grid");
        body->Append(sp);
    }
}

void Game::OnMain()
{
    SetPlanSize(cxSize2F(2048, 1536));
    
    cxTexture::Create()->From("grid.png")->gcSet<cxTexture>("grid");
    
    cxHttp *http = cxHttp::Get("http://192.168.199.244:9001");
    Window()->Append(http);
    http->onError +=[](cxHttp *http){
        CX_LOGGER("error");
    };
    http->onSuccess += [this](cxHttp *http){
        const cxStr *json = http->Body();
        CX_LOGGER("%s",json->ToString());
        ListServers *list = ListServers::Create()->Init(json);
        if(list == nullptr){
            return;
        }
        if(list->Code != 0){
            CX_ERROR("%s",list->Error->ToString());
            return;
        }
        const ServerInfo *info = list->Query();
        if(info == nullptr){
            CX_ERROR("query server info error");
            return;
        }
        client->Connect(info);
    };
//    w = cxWorld::Create();
//    w->SetSize(cxSize2F(2048, 1536));
//    w->SetGravity(cxPoint2F(0, -10));
//    Window()->Append(w);
//    
//    cxChainBody *c = cxChainBody::Create();
//    c->SetSize(cxSize2F(2048, 1536));
//    c->SetStatic(true);
//    w->Append(c);
}

CX_CPP_END

