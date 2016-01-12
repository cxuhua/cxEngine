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

cxDouble allTime = 0;
cxDouble prev = 0;
cxInt dataLen = 0;

void Game::OnMain()
{
    SetPlanSize(cxSize2F(2048, 1536));
    
    cxTcp *tcp = cxTcp::Create("192.168.199.242", 8899);
    tcp->onData+=[](cxTcp *pav,cchars data, cxInt len){
        cxDouble now = cxUtil::Timestamp();
        dataLen += len;
        if(prev == 0){
            prev = now;
            return;
        }
        cxDouble delta = now - prev;
        prev = now;
        allTime += delta;
        if(allTime < 1.0f){
            return;
        }
        allTime -= 1.0f;
        CX_LOGGER("%d",dataLen);
        dataLen = 0;
    };
    Window()->Append(tcp);
}

CX_CPP_END

