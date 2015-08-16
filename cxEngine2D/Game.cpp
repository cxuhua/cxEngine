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

    cxSprite *clip = cxSprite::Create();
    clip->SetSize(cxSize2F(700, 700));
    clip->SetTexture(cxTexture::Create()->From("t.png"));
    clip->SetClip(true);
    Window()->Append(clip);
    
    for(cxInt i=0; i < 1000;i++){
        cxSprite *sp = cxSprite::Create();
        sp->SetSize(cxSize2F(60, 60));
        
        sp->SetPosition(cxPoint2F(CX_RAND_11f() * 1024, CX_RAND_11f() * 1024));
        sp->SetTexture("t.png");
        clip->Append(sp);
    }
    
    
//    cxRotateBy::Create(3.14/2, 15)->AttachTo(clip);
    
}

CX_CPP_END

