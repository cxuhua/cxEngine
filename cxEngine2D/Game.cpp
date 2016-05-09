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

void Game::OnMain()
{
    SetPlanSize(cxSize2F(2048, 1536));
    
    cxHttp *http = cxHttp::Get("http://192.168.199.244:9001/servers/list");
    Window()->Append(http);
    http->onCompleted += [this](cxHttp *http){
        if(!http->Success()){
            CX_ERROR("Get Server list failed");
            return;
        }
        const cxStr *json = http->Body();
        CX_LOGGER("%s",json->ToString());
        ListServers *list = ListServers::Create();
        list->Init(json);
        if(list->Code != 0){
            CX_ERROR("%s",list->Error->ToString());
            return;
        }
        const ServerInfo *info = list->Query();
        if(info == nullptr){
            CX_ERROR("query server info error");
            return;
        }
        client->SetPublicKey(info->Public);
        client->Connect(info->Host->ToString(), info->Port, info->Pass->ToString());
    };
    
//    const cxStr *key = cxUtil::Instance()->AssetsData("key.pub");
//    client->SetPublicKey(key->Data());
//    client->Connect("192.168.199.244", 10020, "123");
    
    cxSprite *sp = cxSprite::Create();
    sp->SetSize(cxSize2F(250, 250));
    sp->SetTexture(cxTexture::Create()->From("grid.png"));
    Window()->Append(sp);
    
    cxRotateBy *by = cxRotateBy::Create(1.0f, 1000.0f);
    sp->Append(by);
}

CX_CPP_END

