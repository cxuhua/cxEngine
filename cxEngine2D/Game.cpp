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

CX_CPP_BEGIN

cxClient *client = nullptr;

CX_IMPLEMENT(Game);

void Game::OnUpdate(cxFloat dt)
{
    client->Process(this);
    cxEngine::OnUpdate(dt);
}

Game::Game()
{
    client = cxClient::Alloc();
    client->Connect("192.168.199.244", 10020, "123");
}

Game::~Game()
{
    
}

void Game::OnMain()
{
    SetPlanSize(cxSize2F(2048, 1536));
    
    cxSprite *sp = cxSprite::Create();
    sp->SetSize(cxSize2F(250, 250));
    sp->SetTexture(cxTexture::Create()->From("grid.png"));
    Window()->Append(sp);
    
    cxRotateBy *by = cxRotateBy::Create(1.0f, 1000.0f);
    sp->Append(by);
}

CX_CPP_END

