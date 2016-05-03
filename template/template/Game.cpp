//
//  IOSEngine.cpp
//  cxEngine2D
//
//  Created by xuhua on 6/17/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#include <engine/cxSprite.h>
#include <engine/cxRotate.h>
#include "Game.h"

CX_CPP_BEGIN

CX_IMPLEMENT(Game);

Game::Game()
{
    
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

