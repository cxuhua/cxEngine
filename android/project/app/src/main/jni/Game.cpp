//
//  IOSEngine.cpp
//  cxEngine2D
//
//  Created by xuhua on 6/17/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#include <engine/cxBox2D.h>
#include <engine/cxContainer.h>
#include <engine/cxSprite.h>
#include <engine/cxMusic.h>
#include <engine/cxButton.h>
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
    
    cxButton *btn = cxButton::Create();
    btn->SetSize(300);
    btn->onRelease+=[this](cxButton *pview){
        cxMusic::Create("o.wav")->AttachTo(Window());
    };
    
    cxSprite *sp = cxSprite::Create();
    sp->SetResizeFlags(cxView::ResizeFill);
    sp->SetTexture(cxTexture::Create()->From("t.png"));
    btn->Append(sp);
    
    Window()->Append(btn);
}

CX_CPP_END

