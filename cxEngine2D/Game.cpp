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
#include <core/cxNumber.h>
#include <engine/cxTable.h>
#include <engine/cxPath.h>
#include <engine/cxTriangles.h>
#include <engine/cxMusic.h>
#include <engine/cxMultiple.h>
#include <engine/cxPoints.h>
#include <engine/cxForward.h>
#include "Game.h"
#include <core/cxKDTree.h>
#include "Sea.h"
#include <engine/cxScript.h>

#include <engine/cxFrames.h>
#include <engine/cxActionGroup.h>
#include <engine/cxLoading.h>
#include <engine/cxRand.h>

#include "Controller.h"
#include "Map.h"
#include "Shader.h"


CX_CPP_BEGIN


CX_IMPLEMENT(Game);

Game::Game()
{
    //设置设计图分辨率
    SetPlanSize(cxSize2F(1242, 2208));
}

Game::~Game()
{
    
}
cxFloat x=0,y=0;
cxMusic *m = NULL;

void Game::OnDispatch(const cxTouchable *e)
{
    cxEngine::OnDispatch(e);
    if(e->TouchCount() > 0 && m !=  NULL){
        cxSize2F size = WinSize();
        const cxTouchPoint *tp = e->TouchPoint(0);
        CX_LOGGER("%f %f",tp->wp.x/size.w,tp->wp.y/size.h);
        m->GetSource()->SetPosition(cxPoint2F(tp->wp.x/size.w,tp->wp.y/size.h));
    }
}

void Game::OnMain()
{
    m = cxMusic::Create("test.mp3");
//    m->GetSource()->SetVelocity(cxPoint2F(100, 100));
//    m->onStep+=[](cxAction *pav,cxFloat step){
//        pav->To<cxMusic>()->GetSource()->SetPosition(cxPoint2F(x, y));
//        x-=1;
//        CX_LOGGER("%f",x);
//    };
//    m->GetSource()->SetPosition(cxPoint2F(100,0));
    m->SetRepeat(1000);
    Window()->Append(m);
    
    return;
    
//    cxSize2F siz = WinSize();
//    LoadTexture("grid.png");
//    Controller *m = Controller::Create(8, 8, cxSize2F(siz.w, siz.w));
//    m->SetResizeFlags(cxView::ResizeBottom);
//    Window()->Append(m);
//    return;
    
    LoadTexture("t.png");
    cxSprite *sp = cxSprite::Create();
    sp->SetTexture("t.png");
    sp->SetSize(80);
//    sp->SetEnableDir(true);
    Window()->Append(sp);
//
//    cxForward *a = cxForward::Create();
//    a->SetSpeed(100);
//    a->SetAngle(cxDegreesToRadians(30));
//    sp->Append(a);
//
//    cxTimer *t = cxTimer::Forever(3.0f);
//    t->onArrive +=[a](cxTimer *pav){
//        a->SetAngle(cxDegreesToRadians(CX_RAND_01f() * 360));
//    };
//    Window()->Append(t);
//
////
    cxPoint2FArray ps;

//    for(cxInt i=0;i<50;i++){
//        cxFloat x = CX_RAND_11f() * 500;
//        cxFloat y = CX_RAND_11f() * 500;
//        ps.Append(cxPoint2F(x, y));
//    }

    sp->SetPosition(cxPoint2F(0, 900));
    ps.Append(cxPoint2F(0, 900));
    ps.Append(cxPoint2F(0, 0));
//    ps.Append(cxPoint2F(-717,0 ));
    


//    sp->SetEnableDir(true);
//    sp->SetPosition(cxPoint2F(600, 0));

    cxPoints *a = cxPoints::Create();
    a->SetPoints(ps);
    a->SetMoveSpeed(500.0f);

//    a->onExit +=[sp](cxAction *pav){
//        sp->SetColor(cxColor4F::RED);
//        sp->Append(pav->Reverse());
//    };

    const cxPoint2FArray &pv = a->Points();
    for(cxInt i=0;i<pv.Size();i++){
        cxPoint2F v = pv.At(i);
        cxSprite *sp = cxSprite::Create();
        sp->SetTexture("t.png");
        sp->SetSize(15);
        sp->SetPosition(v);
        Window()->Append(sp);
    }

    sp->Append(a);
    return;
    
//    cxMusic *m = cxMusic::Create("o.wav");
//    m->SetRepeat(3);
//    Window()->Append(m);
    //加载纹理
    LoadTexture("jl.lqt");
//    LoadTexture("jlbox.lqt");
//    LoadTexture("jl1000.lqt");
    //加载帧序列
    LoadFrames("frames.csv");
    //加载动作组
    LoadActions("actions.csv");
    //获取法师帧序列
    const cxFrames *fs = GetFrames("Mage");
    //获取法师的动作列表
    const cxActionGroup *ag = GetActions("Mage");
    {
        //获得move动作
        const cxActionAttr *attack = ag->Action("attack");
        //创建动画
        cxAnimate *animate = fs->Animate();
        animate->onFrame+=[](cxAnimate *pav,cxInt frame){
            
        };
        animate->onKey+=[](cxAnimate *pav,cxInt key){
            CX_LOGGER("%d",key);
        };
        animate->SetAction(attack, 1);
        //创建载体
//        cxAtlas *atlas = cxAtlas::Create();
        cxTriangles *atlas = cxTriangles::Create();
        atlas->SetFlipX(true);
        atlas->SetSize(100);
        atlas->Append(animate);//加入动画
        Window()->Append(atlas);
    }
}

CX_CPP_END

