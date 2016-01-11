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
    cxOpenGL::Instance()->SetClearColor(cxColor4F::RED);
    SetPlanSize(cxSize2F(2048, 1536));
    {
    cxSprite *sp = cxSprite::Create();
    sp->SetSize(300);
    sp->SetResizeFlags(cxView::ResizeLeftTop);
    sp->SetTexture(cxTexture::Create()->From("t.png"));
    Window()->Append(sp);
    cxRotateBy *a = cxRotateBy::Create(1.0, 60);
    a->AttachTo(sp);
    }
    {
        cxSprite *sp = cxSprite::Create();
        sp->SetSize(300);
        sp->SetTexture(cxTexture::Create()->From("t.png"));
        Window()->Append(sp);
        cxRotateBy *a = cxRotateBy::Create(1.0, 60);
        a->AttachTo(sp);
    }
    {
        cxSprite *sp = cxSprite::Create();
        sp->SetSize(300);
        sp->SetResizeFlags(cxView::ResizeRightTop);
        sp->SetTexture(cxTexture::Create()->From("t.png"));
        Window()->Append(sp);
        cxRotateBy *a = cxRotateBy::Create(1.0, 60);
        a->AttachTo(sp);
    }
    {
        cxSprite *sp = cxSprite::Create();
        sp->SetSize(300);
        sp->SetResizeFlags(cxView::ResizeRightBottom);
        sp->SetTexture(cxTexture::Create()->From("t.png"));
        Window()->Append(sp);
        cxRotateBy *a = cxRotateBy::Create(1.0, 60);
        a->AttachTo(sp);
    }
    {
        cxSprite *sp = cxSprite::Create();
        sp->SetSize(300);
        sp->SetResizeFlags(cxView::ResizeLeftBottom);
        sp->SetTexture(cxTexture::Create()->From("t.png"));
        Window()->Append(sp);
        cxRotateBy *a = cxRotateBy::Create(1.0, 60);
        a->AttachTo(sp);
    }
}

CX_CPP_END

