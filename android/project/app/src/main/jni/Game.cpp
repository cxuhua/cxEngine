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
    cxTexture::Create()->From("t.png")->gcSet<cxTexture>("circle");
    
    cxContainer *c = cxContainer::Create();
    c->SetSize(1024);
    c->SetClip(true);
    
    cxSprite *s = cxSprite::Create();
    s->SetSize(2048);
    s->SetTexture("circle");
    c->Append(s);
    
    Window()->Append(c);
    
//    cxWorld *w = cxWorld::Create();
//    w->SetSize(WinSize());
//    
//    for(cxInt i=0;i<10;i++)
//    {
//        cxCircleBody *cb = cxCircleBody::Create();
//        cb->SetElasticity(0.8f);
//        cb->SetFriction(0.0f);
//        cxSprite *sp = cxSprite::Create();
//        sp->SetTexture("circle");
//        sp->SetResizeFlags(cxView::ResizeFill);
//        sp->AttachTo(cb);
//        
//        cb->SetSize(50);
//        cb->SetEnableDir(true);
//        cb->SetPosition(cxPoint2F(CX_RAND_11f()*200, CX_RAND_11f()*200));
//        
//        w->Append(cb);
//        
//    }
//    {
//        cxChainBody *cb = cxChainBody::Create();
//        cb->SetSize(cxSize2F(200, 200));
//        cb->SetLoop(true);
//        cb->SetElasticity(0);
//        cb->SetFriction(0.0f);
//        cb->Points().Append(cxPoint2F(-900, 600));
//        cb->Points().Append(cxPoint2F(-900, -600));
//        cb->Points().Append(cxPoint2F(900, -600));
//        cb->Points().Append(cxPoint2F(900, 600));
//        cb->SetStatic(true);
//        
//        cxPoint2FArray &ps = cb->Points();
//        for(cxInt i=0; i < ps.Size(); i++){
//            cxSprite *sp = cxSprite::Create();
//            sp->SetTexture("circle");
//            sp->SetSize(30.0f);
//            sp->AttachTo(cb);
//            sp->SetPosition(ps.At(i));
//        }
//        w->Append(cb);
//    }
//    Window()->Append(w);
}

CX_CPP_END

