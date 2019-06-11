//
//  IOSEngine.cpp
//  cxEngine2D
//
//  Created by xuhua on 6/17/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//


#include <engine/cxSprite.h>
#include <engine/cxRotate.h>
#include <engine/cxMusic.h>
#include "Game.h"

CX_CPP_BEGIN

CX_IMPLEMENT(Game);

Game::Game()
{
    skt.InitGmSktClient();
}

Game::~Game()
{
    skt.UnInitGmSktClient();
}

void Game::OnEvent(cxAsyncEvent *e)
{
    if(e->Key() == 1000){
        cxJson *jv = cxJson::CreateObject();
        jv->Set("uuid","abcdxxx");
        jv->Set("cmd","matchteam");
        jv->Set("member_num","1");
        const cxStr *s = jv->Dumps();
        skt.SendStream(s->Buffer(),s->Size());
    }else if(e->Key() == 1001){
        const cxJson *jv = e->Json();
        const cxStr *joinKey = jv->Get("join_key");
        if(joinKey != nullptr){
            skt.JoinAuTeam(joinKey->Data());
        }
    }
}

void Game::OnMain()
{
    LoadTextures("item.png", "grid.png");

    cxSprite *sp = cxSprite::Create();
    sp->SetTexture("item.png");
    sp->SetSize(cxSize2F(200,200));
    Window()->Append(sp);

    auto f = [](cxAction *pav){
        pav->Reverse()->AttachTo(pav->View());
    };

    cxRotateTo *rt = cxRotateTo::Create(M_PI*4,10);
    rt->AttachTo(sp);
    rt->onExit += f;

    skt.Connect("60.205.205.223",9888);

    cxMusic::Create("test.mp3")->AttachTo(Window());
}

CX_CPP_END

