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
#include <engine/cxMultiple.h>
#include "Game.h"
#include <core/cxKDTree.h>
#include "Sea.h"
#include <engine/cxBox2D.h>
#include <engine/cxSpine.h>
#include <core/cxSqlite.h>
#include <engine/cxScript.h>

#include <raknet/cxClient.h>
#include <raknet/ListServers.h>

#include <engine/cxFrames.h>
#include <engine/cxActionGroup.h>
#include <engine/cxLoading.h>
#include <engine/cxRand.h>

#include "Controller.h"

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

cxBoxWorld *w = nullptr;

void Game::OnDispatch(const cxTouchable *e)
{
//    if(w == nullptr){
//        return;
//    }
//    float r = CX_RAND_01f();
//    float g = CX_RAND_01f();
//    float b = CX_RAND_01f();
//    const cxTouchPoint *t1 = e->TouchPoint(0);
//    if(t1->type == cxTouchPoint::Ended){
//        cxCircleBody *body = cxCircleBody::Create();
//        body->SetRadius(25);
//        body->SetPosition(t1->wp);
//        body->SetElasticity(1.0);
//        
//        body->Invoke([](cxView *pview){
//            pview->To<cxCircleBody>()->ApplyForceToCenter(cxPoint2F(800, 0));
//        });
//        
//        w->Append(body);
//        
//        cxSprite *sp = cxSprite::Create();
//        sp->SetColor(cxColor4F(r, g, b, 1.0));
//        sp->SetSize(cxSize2F(50, 50));
//        sp->SetTexture("t.png");
//        body->Append(sp);
//    }
}

void Game::OnMain()
{
    SetPlanSize(cxSize2F(2048, 1536));
    
//    w = cxBoxWorld::Create();
//    w->SetSize(cxSize2F(2048, 1536));
//    w->SetGravity(cxPoint2F(0, -10));
//    Window()->Append(w);
//    
//    cxChainBody *c = cxChainBody::Create();
//    c->SetSize(cxSize2F(2048, 1536));
//    c->SetStatic(true);
//    w->Append(c);
    
    cxLoading *loader = cxLoading::Create();
    loader->Run([this](cxLoading *pview){
        LoadConfig("configs.csv");
    });
    loader->Run([this](cxLoading *pview){
        LoadLocalized("texts.csv");
    });
    loader->Run([this](cxLoading *pview){
        LoadTexture("t.png");
//        LoadTexture("c1.lqt");
    });
//    loader->Run([this](cxLoading *pview){
//        LoadFrames("frames.csv");
//    });
//    loader->Run([this](cxLoading *pview){
//        LoadActions("actions.csv");
//    });
    loader->onProgress+=[](cxLoading *pview,cxInt i,cxInt a){
        CX_LOGGER("loader %d/%d",i,a);
    };
    loader->onCompleted +=[this](cxLoading *pview,cxBool ok){
//        CX_LOGGER("Port:=%d %d",Config("SERVER_PORT")->ToInt(),ok);
//        CX_LOGGER("%s",cxLocalized::Text("TID_LOCALIZED_NAME")->ToString());
//        //获取法师帧序列
//        const cxFrames *fs = GetFrames("Mage");
//        //获取法师的动作列表
//        const cxActionGroup *ag = GetActions("Mage");
//        //获得move动作
//        const cxActionAttr *move = ag->Action("attack1");
//        //创建动画
//        cxAnimate *animate = fs->Animate();
//        animate->onFrame+=[](cxAnimate *pav,cxInt frame){
//            CX_LOGGER("%d %d",frame,pav->IsKeyFrame());
//        };
//        //设置移动组1为当前播放组
//        animate->SetAction(move, 1);
//        animate->SetSpeed(1.5f);
//        //
//        cxAtlas *atlas = cxAtlas::Create();
////        atlas->SetFlipX(true);
//        atlas->SetSize(cxSize2F(600, 600));
//        animate->AttachTo(atlas);
//        
//        Window()->Append(atlas);
        
//        cxSprite *sp = cxSprite::Create();
//        sp->SetTexture("t.png");
//        sp->SetSize(200);
//        Window()->Append(sp);
//        
//        cxSequence *ms = cxSequence::Create();
//        cxMultiple *ms = cxMultiple::Create();
//        
//        cxMoveTo *m1 = cxMoveTo::Create(cxPoint2F(0, 300), 3.0);
//        m1->AttachTo(sp);
//        cxRotateTo *r1 = cxRotateTo::Create(3.0, 5.0);
//        r1->AttachTo(sp);
//        
//        ms->Append(m1);
//        ms->Append(r1);
//        
//        ms->onAction+=[](cxMultiple *p,cxAction *a){
//            CX_LOGGER("%p action exit",a);
//        };
//        Window()->Append(ms);
        
        Controller *c = Controller::Create(5, 3);
        c->SetResizeFlags(cxView::ResizeBottom);
        Window()->Append(c);
    };
    
    Window()->Append(loader);
//    
//    cxHttp *http = cxHttp::Get("http://192.168.199.244:9001");
//    Window()->Append(http);
//    http->onError +=[](cxHttp *http){
//        CX_LOGGER("error");
//    };
//    http->onSuccess += [this](cxHttp *http){
//        const cxStr *json = http->Body();
//        CX_LOGGER("%s",json->ToString());
//        ListServers *list = ListServers::Create()->Init(json);
//        if(list == nullptr){
//            return;
//        }
//        if(list->Code != 0){
//            CX_ERROR("%s",list->Error->ToString());
//            return;
//        }
//        const ServerInfo *info = list->Query();
//        if(info == nullptr){
//            CX_ERROR("query server info error");
//            return;
//        }
//        client->Connect(info);
//    };
}

CX_CPP_END

