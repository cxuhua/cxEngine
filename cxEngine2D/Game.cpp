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

    cxTexture::Create()->From("circle.png")->gcSet<cxTexture>("circle");
    cxTexture::Create()->From("t.png")->gcSet<cxTexture>("point");
    
//    {
//    cxTriangles *ts = cxTriangles::Create();
//    ts->SetSize(400);
//    ts->SetTexture("point");
//    cxRenderFArray &rs = ts->Renders();
//    rs.Init(4);
//    
//    int a = 100;
//    int c = 200;
//    int &b = a;
//    b = c;
//    {
//    cxRenderF &r = rs.Inc();
//    r.vertices = cxPoint3F(-400, 400, 0);
//    r.colors = cxColor4F::WHITE;
//    r.coords = cxCoord2F(0, 0);
//    }
//    {
//    cxRenderF &r = rs.Inc();
//    r.vertices = cxPoint3F(400, 400, 0);
//    r.colors = cxColor4F::WHITE;
//    r.coords = cxCoord2F(1, 0);
//    }
//    {
//    cxRenderF &r = rs.Inc();
//    r.vertices = cxPoint3F(400, -400, 0);
//    r.colors = cxColor4F::WHITE;
//    r.coords = cxCoord2F(1, 1);
//    }
//    {
//    cxRenderF &r = rs.Inc();
//    r.vertices = cxPoint3F(-400, -400, 0);
//    r.colors = cxColor4F::WHITE;
//    r.coords = cxCoord2F(0, 1);
//    }
//    Window()->Append(ts);
//    }
//    
//    {
//        cxTriangles *ts = cxTriangles::Create();
//        ts->SetSize(400);
//        ts->SetTexture("point");
//        cxRenderFArray &rs = ts->Renders();
//        rs.Init(4);
//        
//        int a = 100;
//        int c = 200;
//        int &b = a;
//        b = c;
//        {
//            cxRenderF &r = rs.Inc();
//            r.vertices = cxPoint3F(-200, 200, 0);
//            r.colors = cxColor4F::RED;
//            r.coords = cxCoord2F(0, 0);
//        }
//        {
//            cxRenderF &r = rs.Inc();
//            r.vertices = cxPoint3F(200, 200, 0);
//            r.colors = cxColor4F::RED;
//            r.coords = cxCoord2F(1, 0);
//        }
//        {
//            cxRenderF &r = rs.Inc();
//            r.vertices = cxPoint3F(200, -200, 0);
//            r.colors = cxColor4F::RED;
//            r.coords = cxCoord2F(1, 1);
//        }
//        {
//            cxRenderF &r = rs.Inc();
//            r.vertices = cxPoint3F(-200, -200, 0);
//            r.colors = cxColor4F::RED;
//            r.coords = cxCoord2F(0, 1);
//        }
//        Window()->Append(ts);
//    }


    {
        cxSpine *sp = cxSpine::Create("goblins-mesh.atlas","goblins-mesh.json",1.0f);
        sp->SetSkin("goblin");
        sp->SetSize(300);
        sp->SetAnimation(0, "walk", true);
        Window()->Append(sp);
    }
//    {
//        cxSpine *sp = cxSpine::Create("spineboy.atlas","spineboy.json",1.0f);
//        sp->SetPosition(cxPoint2F(800, 0));
//        sp->SetSize(300);
//        sp->SetMix("walk", "jump", 0.2f);
//        sp->SetMix("jump", "run", 0.2f);
//        sp->SetMix("run", "shoot", 0.2f);
//    
//        sp->SetAnimation(0, "walk", true);
//        sp->AddAnimation(0, "jump", false, 3);
//        sp->AddAnimation(0, "run", true, 0);
//        sp->AddAnimation(0, "idle", true, 3);
//        sp->AddAnimation(0, "shoot", true, 0);
//        sp->AddAnimation(0, "death", true, 0);
//        sp->AddAnimation(0, "hit", true, 0);
//        sp->AddAnimation(0, "idle", true, 0);
//        sp->AddAnimation(0, "hit", true, 0);
//        
//        Window()->Append(sp);
//    }
//    return;
//    
//    cxWorld *w = cxWorld::Create();
//    w->SetSize(WinSize());
//    
//    for(cxInt i=0;i<140;i++)
//    {
//        cxCircleBody *cb = cxCircleBody::Create();
//        cb->SetElasticity(1.0f);
//        cxSprite *sp = cxSprite::Create();
//        sp->SetTexture("circle");
//        sp->SetResizeFlags(cxView::ResizeFill);
//        sp->AttachTo(cb);
//        
//        cb->SetSize(50);
//        cb->EnableDir(true);
//
//        cxBody *b = w->AppendBody(cb);
//        b->SetPosition(cxPoint2F(CX_RAND_11f()*200, CX_RAND_11f()*200));
//    }
//    {
//        cxChainBody *cb = cxChainBody::Create();
//        cb->SetSize(cxSize2F(200, 200));
//        cb->SetLoop(true);
//        cb->Points().Append(cxPoint2F(-900, 600));
//        cb->Points().Append(cxPoint2F(-900, -600));
//        cb->Points().Append(cxPoint2F(900, -600));
//        cb->Points().Append(cxPoint2F(900, 600));
//        cb->SetStatic(true);
//        
//        cxPoint2FArray &ps = cb->Points();
//        for(cxInt i=0; i < ps.Size(); i++){
//            cxSprite *sp = cxSprite::Create();
//            sp->SetTexture("point");
//            sp->SetSize(30.0f);
//            sp->AttachTo(cb);
//            sp->SetPosition(ps.At(i));
//        }
//        w->AppendBody(cb);
//    }
//    {
//        cxEdgeBody *cb = cxEdgeBody::Create();
//        cb->SetSize(cxSize2F(2048, 10));
//        cb->SetElasticity(0);
//        cb->SetLinePoint(cxLineF(-1024, -400, 1024, -400));
//        cb->SetStatic(true);
//        
//        cxBody *b = w->AppendBody(cb);
//    }
//    {
//        cxBoxBody *cb = cxBoxBody::Create();
//        cb->SetSize(cxSize2F(2048, 50));
//        cb->SetStatic(true);
//        
//        cxBody *b = w->AppendBody(cb);
//        b->SetPosition(cxPoint2F(0, -400));
//        b->SetTexture("t.png");
//        b->SetCapacity(1);
//        cxBoxPoint3F box;
//        box.Set(-1024, 1024, 25, -25);
//        cxBoxRender &r = b->Inc();
//        r.SetVertices(box);
//    }
//    Window()->Append(w);
}

CX_CPP_END

