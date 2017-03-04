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
#include "public.pb.h"
#include <pb_encode.h>
#include <pb_decode.h>


CX_CPP_BEGIN

CX_IMPLEMENT(Game);

Game::Game()
{
    
}

Game::~Game()
{
    
}

void Game::test()
{
    LoadTexture("jl.lqt");
}

void Game::OnMain()
{
    SetPlanSize(cxSize2F(2048, 1536));
//    test();

//    LoadTexture("bg.jpg");
//    {
//        cxTriangles *v1 = cxTriangles::Create();
//        v1->SetTexture("bg.jpg");
//        v1->SetType(cxRenderState::TrianglesVBO);
//        v1->SetSize(cxSize2F(600, 600));
//        v1->SetPosition(cxPoint2F(0, 300));
//        {
//            cxRenderF p1;
//            p1.colors = cxColor4F::RED;
//            p1.vertices = cxPoint3F(-200,0,0);
//            p1.coords = cxCoord2F(0,0.5);
//            v1->Append(0);
//            v1->Append(p1);
//            
//            cxRenderF p2;
//            p2.colors = cxColor4F::GREEN;
//            p2.vertices = cxPoint3F(0,300,0);
//            p2.coords = cxCoord2F(0.5,0.0);
//            v1->Append(1);
//            v1->Append(p2);
//            
//            cxRenderF p3;
//            p3.colors = cxColor4F::BLUE;
//            p3.vertices = cxPoint3F(200,0,0);
//            p3.coords = cxCoord2F(1.0,0.5);
//            v1->Append(2);
//            v1->Append(p3);
//        }
//        {
//            cxRenderF p1;
//            p1.colors = cxColor4F::RED;
//            p1.vertices = cxPoint3F(-200,0,0);
//            p1.coords = cxCoord2F(0,0.5);
//            v1->Append(3);
//            v1->Append(p1);
//            
//            cxRenderF p2;
//            p2.colors = cxColor4F::GREEN;
//            p2.vertices = cxPoint3F(0,-300,0);
//            p2.coords = cxCoord2F(0.5,1.0);
//            v1->Append(4);
//            v1->Append(p2);
//            
//            cxRenderF p3;
//            p3.colors = cxColor4F::BLUE;
//            p3.vertices = cxPoint3F(200,0,0);
//            p3.coords = cxCoord2F(1.0,0.5);
//            v1->Append(5);
//            v1->Append(p3);
//        }
//        
//        Window()->Append(v1);
//    }
//
//    {
//        cxTriangles *v1 = cxTriangles::Create();
//        v1->SetTexture("bg.jpg");
//        v1->SetType(cxRenderState::TrianglesVBO);
//        v1->SetSize(cxSize2F(600, 600));
//        v1->SetPosition(cxPoint2F(0, -300));
//        v1->SetColor(cxColor4F::RED);
//        
//        cxRenderF p1;
//        p1.colors = cxColor4F::WHITE;
//        p1.vertices = cxPoint3F(-200,0,0);
//        p1.coords = cxCoord2F(0,0.5);
//        v1->Append(0);
//        v1->Append(p1);
//        
//        cxRenderF p2;
//        p2.colors = cxColor4F::WHITE;
//        p2.vertices = cxPoint3F(0,-300,0);
//        p2.coords = cxCoord2F(0.5,1.0);
//        v1->Append(1);
//        v1->Append(p2);
//        
//        cxRenderF p3;
//        p3.colors = cxColor4F::WHITE;
//        p3.vertices = cxPoint3F(200,0,0);
//        p3.coords = cxCoord2F(1.0,0.5);
//        v1->Append(2);
//        v1->Append(p3);
//        
//        Window()->Append(v1);
//    }
//
//    cxSprite *sp = cxSprite::Create();
//    sp->SetResizeFlags(cxView::ResizeFill);
//    sp->SetTexture("bg.jpg");
////    sp->SetColor("#0000F0");
//    Window()->Append(sp);
//
//    cxSprite *b = cxSprite::Create();
//
////    aa(b);
//
////    b->SetColor(cxColor4F::RED)
//    
//    b->SetSize(cxSize2F(350, 350));
//    b->SetTexture("1.png");
//    b->SetPosition(cxPoint2F(0, 50));
//    Window()->Append(b);
//
//    cxEmitterXml *xml = cxEmitterXml::CreateXml("penhuo1_1");    
//    cxEmitter *v = xml->Emitter();
//    Window()->Append(v);
    
//    cxView *v2 = v->Clone();
//    v2->SetPosition(cxPoint2F(0, -300));
//    Window()->Append(v2);
    
    
//
//    const cxStr *data = nullptr;
//    {
//        cxStr *code = cxStr::UTF8("8347834");
//        IndexType message = IndexType_init_zero;
//        message.tid = 200;
//        message.code = CX_STR_PB_ENCODE(code);
//        data = cxStr::PBEncode(IndexType_fields, &message);
//        cxUtil::Instance()->WriteDocument("test.dat", data, true);
//    }
//    {
//        cxStr *code = cxStr::Create();
//        IndexType m2 = IndexType_init_zero;
//        m2.code = CX_STR_PB_DECODE(code);
//        cxBool ok = data->PBDecode(IndexType_fields, &m2);
//        CX_LOGGER("%d",ok);
//    }
    
//    LoadTexture("grid.png","grid");
//
//    cxSprite *sp = cxSprite::Create();
//    sp->SetSize(cxSize2F(64));
//    sp->SetTexture("grid");
//    Window()->Append(sp);
//    
//    cxMoveTo::Create(cxPoint2F(0, 512), 6)->AttachTo(sp)->onExit+=cxAction::Remove;
//    
//    {
//        cxSprite *sp2 = cxSprite::Create();
//        sp2->SetSize(cxSize2F(64));
//        sp2->SetTexture("grid");
//        sp2->SetColor(cxColor4F::RED);
//        Window()->Append(sp2);
//        
//        cxFollow::Create(sp, 50)->AttachTo(sp2);
//    }
    
//    Controller *cv = Controller::Create(6, 6);
//    cv->SetResizeFlags(cxView::ResizeBottom);
//    Window()->Append(cv);
//
//    return;
    
    cxSprite *sp = cxSprite::Create("t.png");
    sp->SetSize(800);
    Window()->Append(sp);
    //加载纹理
    LoadTexture("jl.lqt");
    LoadTexture("jlbox.lqt");
    LoadTexture("jl1000.lqt");
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
        const cxActionAttr *move = ag->Action("move");
        //创建动画
        cxAnimate *animate = fs->Animate();
        animate->onFrame+=[](cxAnimate *pav,cxInt frame){
            
        };
        animate->onKey+=[](cxAnimate *pav,cxInt key){
            CX_LOGGER("%d",key);
        };
        animate->SetAction(move, 1);
        //创建载体
//        cxAtlas *atlas = cxAtlas::Create();
        cxTriangles *atlas = cxTriangles::Create();
//        atlas->SetFlipX(true);
        atlas->SetSize(800);
        atlas->Append(animate);//加入动画
        Window()->Append(atlas);
    }
    
    {
        //获得move动作
        const cxActionAttr *move = ag->Action("move");
        //创建动画
        cxAnimate *animate = fs->Animate();
        animate->onFrame+=[](cxAnimate *pav,cxInt frame){
            
        };
        animate->onKey+=[](cxAnimate *pav,cxInt key){
            CX_LOGGER("%d",key);
        };
        animate->SetAction(move, 0);
        //创建载体
//            cxAtlas *atlas = cxAtlas::Create();
        cxTriangles *atlas = cxTriangles::Create();
//        atlas->SetFlipY(true);
        atlas->SetPosition(cxPoint2F(0, 800));
        atlas->SetSize(800);
        atlas->Append(animate);//加入动画
        Window()->Append(atlas);
    }
    
    {
        //获得move动作
        const cxActionAttr *move = ag->Action("move");
        //创建动画
        cxAnimate *animate = fs->Animate();
        animate->onFrame+=[](cxAnimate *pav,cxInt frame){
            
        };
        animate->onKey+=[](cxAnimate *pav,cxInt key){
            CX_LOGGER("%d",key);
        };
        animate->SetAction(move, 2);
        //创建载体
//            cxAtlas *atlas = cxAtlas::Create();
        cxTriangles *atlas = cxTriangles::Create();
//        atlas->SetFlipX(true);
//        atlas->SetFlipY(true);
        atlas->SetPosition(cxPoint2F(0, -800));
        atlas->SetSize(800);
        atlas->Append(animate);//加入动画
        Window()->Append(atlas);
    }
}

CX_CPP_END

