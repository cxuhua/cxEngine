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
#include <engine/cxTriangles.h>
#include <engine/cxPath.h>
#include <engine/cxCircle.h>
#include <engine/cxMusic.h>
#include "Game.h"
#include <core/cxKDTree.h>
#include "Sea.h"
#include <engine/cxBox2D.h>


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
    cxTexture::Create()->From("t.png")->gcSet<cxTexture>("t.png");
    
    cxBox2D::cxWorld *w = cxBox2D::cxWorld::Create();
    w->SetSize(WinSize());
    
    {
        cxBox2D::cxCircleBody *cb = cxBox2D::cxCircleBody::Create();
        cb->SetAngularVelocity(3.0f);
        
        cxSprite *sp = cxSprite::Create();
        sp->SetTexture("t.png");
        sp->SetResizeFlags(cxView::ResizeFill);
        sp->AttachTo(cb);
        
        cb->SetSize(100);

        cxBox2D::cxBody *b = w->AppendBody(cb);
        b->SetAngle(0.3f);
        b->SetPosition(cxPoint2F(-400, 400));
    }
    {
        cxBox2D::cxCircleBody *cb = cxBox2D::cxCircleBody::Create();
        cb->SetAngularVelocity(3.0f);
        
        cxSprite *sp = cxSprite::Create();
        sp->SetTexture("t.png");
        sp->SetResizeFlags(cxView::ResizeFill);
        sp->AttachTo(cb);
        
        cb->SetSize(100);
        
        cxBox2D::cxBody *b = w->AppendBody(cb);
        b->SetAngle(0.3f);
        b->SetPosition(cxPoint2F(-400, 500));
    }
    {
        cxBox2D::cxEdgeBody *cb = cxBox2D::cxEdgeBody::Create();
        cb->SetSize(cxSize2F(2048, 10));
        
        cb->SetLinePoint(cxLineF(-1024, -400, 1024, -400));
        cb->SetStatic(true);
        
        cxBox2D::cxBody *b = w->AppendBody(cb);
    }
//    {
//        cxBox2D::cxBoxBody *cb = cxBox2D::cxBoxBody::Create();
//        cb->SetSize(cxSize2F(2048, 50));
//        cb->SetStatic(true);
//        
//        cxBox2D::cxBody *b = w->AppendBody(cb);
//        b->SetPosition(cxPoint2F(0, -400));
//        b->SetTexture("t.png");
//        b->SetCapacity(1);
//        cxBoxPoint3F box;
//        box.Set(-1024, 1024, 25, -25);
//        cxBoxRender &r = b->Inc();
//        r.SetVertices(box);
//    }
    Window()->Append(w);
}

CX_CPP_END

