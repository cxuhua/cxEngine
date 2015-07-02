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
#include "IOSEngine.h"

CX_CPP_BEGIN

CX_IMPLEMENT(IOSEngine);

IOSEngine::IOSEngine()
{
    
}

IOSEngine::~IOSEngine()
{
    
}

cxView *v;

void IOSEngine::OnUpdate(cxFloat dt)
{
//    static cxFloat a= 0;
//    static cxFloat b = 0;
//    b += 0.0005f;
//    if(b >=0.5f){
//        b = -0.5f;
//    }
//    v->SetAxis(cxPoint3F::AxisY);
//    v->SetAngle(a);
//    a+=0.01f;
    cxEngine::OnUpdate(dt);
}

void IOSEngine::test(cxButton *sender)
{
    CX_LOGGER("jhaha");
}

void IOSEngine::OnMain()
{
    cxTexture::Create()->From("animate.png")->Atlas("animate.json")->gcpush<cxTexture>("animate");
    cxTexture::Create()->From("t.png")->gcpush<cxTexture>("t.png");
    cxTexture::Create()->From("bg.jpg")->gcpush<cxTexture>("bg");
    

    
    cxContainer *c = cxContainer::Create();
    c->SetResizeFlags(cxView::ResizeFill);
    
    cxSprite *body = cxSprite::Create("bg");
    body->SetAnchor(cxPoint2F(-0.0f, 0.0f));//left
    body->SetSize(cxSize2F(2560, 1920));
    c->Append(body);
    
    cxEmitter *v = cxEmitter::Create(250);
    v->SetSize(60.0f);
    v->SetTexture("t.png");
    c->Append(v);
    
    Window()->Append(c);
//
//
//    cxView *layer = cxView::Create();
//    layer->SetResizeFlags(cxView::ResizeFill);
//    body->Append(layer);
    
//    cxAtlas *a = cxAtlas::Create();
//    a->SetSize(cxSize2F(500, 300));
//    a->SetTexture("animate");
//    a->SetTexKey("8.0.png");
//    a->SetZ(1);
//    a->SetScale9(20);
//    
//    cxButton *btn = cxButton::Create();
//    btn->SetSize(cxSize2F(500, 300));
//    btn->SetIsPass(true);
//    btn->Append(a);

//    btn->onTap += cxBindEvent(this,IOSEngine::test);
//    body->Append(btn);
    
//    Window()->Append(c);
//    
//    cxSprite *sp1 = cxSprite::Create();
//    sp1->SetTexture("t.png");
//    sp1->SetSize(30.f);
//    layer->Append(sp1);
    
//    cxSprite *sp1 = cxSprite::Create();
//    sp1->SetTexture("t.png");
////    sp1->SetAlignFlags(cxView::AutoAlignLeftBottom);
//    sp1->SetSize(500.0f);
////    sp1->SetAnchor(-0.5f);
//
//    {
//        cxSprite *sp2 = cxSprite::Create();
//        sp2->SetTexture("t.png");
//        sp2->SetColor(cxColor4F::RED);
//        sp2->SetSize(100.0f);
//        sp2->SetAlignFlags(cxView::AutoAlignTopSide|cxView::AutoAlignRightSide);
//        sp2->SetAlignBox(10.0f);
////        sp2->SetAnchor(-0.2f);
//        sp1->Append(sp2);
//    }
    
//    {
//        cxSprite *sp2 = cxSprite::Create();
//        sp2->SetTexture("t.png");
//        sp2->SetColor(cxColor4F::RED);
//        sp2->SetSize(100.0f);
//        sp2->SetAlignFlags(cxView::AutoAlignRightBottom);
//        sp2->SetAlignBox(20.0f);
//        sp2->SetAnchor(0.2f);
//        sp1->Append(sp2);
//    }
//    
//    {
//        cxSprite *sp2 = cxSprite::Create();
//        sp2->SetTexture("t.png");
//        sp2->SetColor(cxColor4F::RED);
//        sp2->SetSize(100.0f);
//        sp2->SetAlignFlags(cxView::AutoAlignLeftTop);
//        sp2->SetAlignBox(30.0f);
//        sp2->SetAnchor(-0.2f);
//        sp1->Append(sp2);
//    }
//    
//    {
//        cxSprite *sp2 = cxSprite::Create();
//        sp2->SetTexture("t.png");
//        sp2->SetColor(cxColor4F::RED);
//        sp2->SetSize(100.0f);
//        sp2->SetAlignFlags(cxView::AutoAlignLeftBottom);
//        sp2->SetAlignBox(40.0f);
//        sp2->SetAnchor(-0.4f);
//        sp1->Append(sp2);
//    }
//    
//    {
//        cxSprite *sp2 = cxSprite::Create();
//        sp2->SetTexture("t.png");
//        sp2->SetColor(cxColor4F::RED);
//        sp2->SetSize(100.0f);
//        sp1->Append(sp2);
//    }
//    
//    Window()->Append(sp1);
//    return;
    
//    cxAtlas *a = cxAtlas::Create();
//    a->SetSize(cxSize2F(500, 300));
//    a->SetTexture("animate");
//    a->SetTexKey("8.0.png");
//    a->SetZ(1);
//    a->SetScale9(20);
//    
//    cxButton *btn = cxButton::Create();
//    btn->SetSize(cxSize2F(500, 300));
//    btn->Append(a);
//    
//    btn->onTap += cxBindEvent(this,IOSEngine::test);
//    
//    btn->onTap += [](cxButton *sender){
//        sender->SetAutoAlignFlags(cxView::AutoAlignRightBottom);
//        sender->SetAutoAlignBox(20.0f);
//    };
//    btn->onPress += [](cxButton *sender){
//        sender->StopAction(100);
//        cxScaleTo *to = cxScaleTo::Create(0.95f, 0.1f);
//        to->SetID(100);
//        sender->Append(to);
//    };
//    btn->onRelease += [](cxButton *sender){
//        sender->StopAction(100);
//        cxScaleTo *to = cxScaleTo::Create(1.00f, 0.1f);
//        to->SetID(100);
//        sender->Append(to);
//    };
//    btn->SetZ(10);
//    
//    Window()->Append(btn);
//    Window()->Append(a);
//
//    {
//        cxSprite *x = cxSprite::Create();
//        x->SetTexture("t.png");
//        x->SetSize(1536);
//        Window()->Append(x);
//    }
//    Window()->Sort();
//
//    cxSprite *x1 = nullptr;
//    {
//        cxSprite *x = cxSprite::Create();
//        x->SetTexture("t.png");
//        x->SetSize(1536/2);
//        Window()->Append(x);
//        x1 = x;
//    }
//    for(cxInt i=0; i < 1000; i++){
//        cxSprite *sp = cxSprite::Create();
//        sp->SetTexture("animate");
//        sp->SetTexKey("1201100.0.png");
//        sp->SetSize(cxSize2F(400, 400));
//        sp->SetPosition(cxPoint2F(cxUtil::Rand(-1024, 1024),cxUtil::Rand(-1024, 1024)));
//        sp->SetAnchor(-0.5f);
//        Window()->Append(sp);
//        cxRotateBy *by = cxRotateBy::Create(cxDegreesToRadians(60), 1000);
//        sp->Append(by);
//    }
//    {
//        cxSprite *x = cxSprite::Create();
//        x->SetTexture("t.png");
//        x->SetSize(1536/6);
//        x->SetClip(true);
//        Window()->Append(x);
//        
//        cxSprite *s1 = cxSprite::Create();
//        s1->SetTexture("t.png");
//        s1->SetSize(1536/4);
//        x->Append(s1);
//    }
//    {
//        cxLabel *label = cxLabel::Create()->SetText(cxStr::UTF8("中国人民银行"));
//        label->SetFontSize(60);
//        label->SetStroke(3, cxColor4F::BLACK, 1);
//        label->SetBold(true);
//        label->SetZ(10);
//        btn->Append(label);
//    }
}

CX_CPP_END

