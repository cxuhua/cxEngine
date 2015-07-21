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
#include "IOSEngine.h"
#include <core/cxKDTree.h>
#include "Sea.h"


CX_CPP_BEGIN

CX_IMPLEMENT(IOSEngine);


IOSEngine::IOSEngine()
{
    
}

IOSEngine::~IOSEngine()
{
    
}

cxKDTree *tree;

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
    
//    cxArray *subviews = Window()->Subviews();
//    for(cxArray::FIter it=subviews->FBegin();it!=subviews->FEnd();it++){
//        cxView *v = (*it)->To<cxView>();
//        cxPoint2F pos = v->Position();
//        v->SetColor(cxColor4F::WHITE);
//    }
//
//    ofx::KDTree<Vec2f>::SearchResults res;
//    ofx::KDTree<Vec2f> points(ps);
//    
//    cxEngine::OnUpdate(dt);
//    
//    points.buildIndex();
//    
//    res.clear();
//    
//    points.findPointsWithinRadius(Vec2f(rp.x, rp.y), 200, res);
//    
//    ofx::KDTree<Vec2f>::SearchResults::iterator it = res.begin();
//    while(it != res.end()){
//        Vec2f p = ps[(*it).first];
////        cxSprite *sp = (cxSprite *)p.tag;
////        sp->SetColor(cxColor4F::RED);
//        it++;
//    }
    
//    tree->Build();
//    if(tree->Nearst(cxPoint2F(rp.x, rp.y), 200) > 0){
//        const cxKDTree::Results &res = tree->GetResults();
//        cxKDTree::Results::const_iterator it = res.begin();
//        while(it != res.end()){
//            const KDTreeNode &node = tree->GetTreeNode(it);
//            cxSprite *sp = (cxSprite *)node.tag;
//            sp->SetColor(cxColor4F::RED);
//            it ++;
//        }
//    }
}

void IOSEngine::test(cxButton *sender)
{
    CX_LOGGER("jhaha");
}

void IOSEngine::OnMain()
{
    SetPlanSize(cxSize2F(2048, 1536));
    
//    cxShader *ws = Sea::Create()->Init("water.vsh","water.fsh");
//    cxTexture *wt = cxTexture::Create()->From("water.png");
//    wt->SetParams(cxTextureParams::Repeat);
//
//    cxSprite *sp = cxSprite::Create();
//    sp->SetColor(cxColor4F(0, 0, 0, 0));
//    sp->SetSize(cxSize2F(512, 512));
//    sp->SetTexture(wt);
//    sp->SetShader(ws);
//    Window()->Append(sp);
    
    cxTexture::Create()->From("animate.png")->Atlas("animate.json")->gcpush<cxTexture>("animate");
    cxTexture::Create()->From("t.png")->gcpush<cxTexture>("t.png");
    cxTexture::Create()->From("bg.jpg")->gcpush<cxTexture>("bg");
    cxTexture::Create()->From("grid.png")->gcpush<cxTexture>("grid");
    
    cxPath *a = cxPath::Create(200);
    a->Append(100);
    a->Append(200);
    a->Append(cxPoint2F(300, 0));
    a->Append(cxPoint2F(-600, 0));
    {
        cxSprite *sp= cxSprite::Create()->SetTexture("t.png");
        sp->SetSize(cxSize2F(10, 10));
        sp->SetPosition(100);
        Window()->Append(sp);
    }
    {
        cxSprite *sp= cxSprite::Create()->SetTexture("t.png");
        sp->SetSize(cxSize2F(10, 10));
        sp->SetPosition(200);
        Window()->Append(sp);
    }
    {
        cxSprite *sp= cxSprite::Create()->SetTexture("t.png");
        sp->SetSize(cxSize2F(10, 10));
        sp->SetPosition(cxPoint2F(300, 0));
        Window()->Append(sp);
    }
    {
        cxSprite *sp= cxSprite::Create()->SetTexture("t.png");
        sp->SetSize(cxSize2F(10, 10));
        sp->SetPosition(cxPoint2F(-600, 0));
        Window()->Append(sp);
    }
    
    cxSprite *sp= cxSprite::Create()->SetTexture("t.png");
    sp->SetSize(cxSize2F(50, 50));
    Window()->Append(sp);
    
    sp->Append(a);
    
//    tree = cxKDTree::Alloc();
//    
//    for(cxInt j=0;j<20;j++)
//    for(cxInt i=0;i<20;i++){
//        cxFloat x = i * 50 - 500;
//        cxFloat y = j * 50 - 500;
//        
//        cxSprite *sp= cxSprite::Create()->SetTexture("grid");
//        sp->SetSize(cxSize2F(50, 50));
//        sp->SetPosition(cxPoint2F(x, y));
//        Window()->Append(sp);
//        
//        tree->Append(cxPoint2F(x, y), (cxLong)sp);
//    }
//    
//    
//    cxSprite *tmp= cxSprite::Create()->SetTexture("t.png");
//    tmp->SetSize(cxSize2F(20, 20));
//    Window()->Append(tmp);
//    
//    cxTimer * timer = cxTimer::Forever(1.0f);
//    timer->onArrive += [this,tmp](cxTimer *pav){
//        rp.x = cxUtil::Rand(-500, +500);
//        rp.y = cxUtil::Rand(-500, +500);
//        tmp->SetPosition(rp);
//    };
//    timer->AttachTo(Window());
    
//    cxCircle *c = cxCircle::Create();
//    c->SetTexture("t.png");
//    c->SetSize(cxSize2F(500, 500));
//    c->SetPercent(0.7);
//    Window()->Append(c);
    
//    cxTriangles *ts = cxTriangles::Create();
//    ts->SetTexture("t.png");
//    ts->SetSize(cxSize2F(300, 300));
//    
//    cxRenderF p1;
//    p1.colors = cxColor4F::WHITE;
//    p1.vertices = cxPoint3F(0, 0, 0);
//    p1.coords = cxCoord2F(0.5, 0.5);
//    ts->Append(p1);
//    
//    p1.colors = cxColor4F::WHITE;
//    p1.vertices = cxPoint3F(400, 0, 0);
//    p1.coords = cxCoord2F(1.0, 0.5);
//    ts->Append(p1);
//    
//    p1.colors = cxColor4F::WHITE;
//    p1.vertices = cxPoint3F(0, 400, 0);
//    p1.coords = cxCoord2F(0.5, 0.0);
//    ts->Append(p1);
//    
//
//    p1.colors = cxColor4F::WHITE;
//    p1.vertices = cxPoint3F(-400, 0, 0);
//    p1.coords = cxCoord2F(0.0, 0.5);
//    ts->Append(p1);
//    
//    p1.colors = cxColor4F::WHITE;
//    p1.vertices = cxPoint3F(0, -400, 0);
//    p1.coords = cxCoord2F(0.5, 1.0);
//    ts->Append(p1);
//    
//    p1.colors = cxColor4F::WHITE;
//    p1.vertices = cxPoint3F(400, 0, 0);
//    p1.coords = cxCoord2F(1.0, 0.5);
//    ts->Append(p1);
//    
//    Window()->Append(ts);
    
//    cxTable *table = cxTable::Create();
//    table->SetColor(cxColor4F::RED);
//    table->SetTexture("grid");
//    table->SetSize(cxSize2F(1200, 1200));
//    table->SetPixel(4.0f);
//    table->SetPadding(10);
//    table->SetMargin(30);
//    
//    {
//        cxSprite *item = cxSprite::Create()->SetTexture("t.png");
//        item->SetSize(400);
//        item->SetScale(0.0f);
//        cxScaleTo *scale = cxScaleTo::Create(1.0f, 0.5f);
//        scale->SetTiming(cxTiming::CubicOut);
//        scale->AttachTo(item);
//        table->Append(item);
//    }
//    {
//        cxSprite *item = cxSprite::Create()->SetTexture("t.png");
//        item->SetSize(400);
//        item->SetScale(0.0f);
//        cxScaleTo *scale = cxScaleTo::Create(1.0f, 0.5f);
//        scale->AttachTo(item);
//        scale->SetTiming(cxTiming::CubicOut);
//        table->Append(item);
//    }
//    {
//        cxSprite *item = cxSprite::Create()->SetTexture("t.png");
//        item->SetSize(400);
//        item->SetScale(0.0f);
//        cxScaleTo *scale = cxScaleTo::Create(1.0f, 0.5f);
//        scale->AttachTo(item);
//        scale->SetTiming(cxTiming::CubicOut);
//        table->Append(item);
//    }
//    {
//        cxSprite *item = cxSprite::Create()->SetTexture("t.png");
//        item->SetSize(400);
//        item->SetScale(0.0f);
//        cxScaleTo *scale = cxScaleTo::Create(1.0f, 0.5f);
//        scale->AttachTo(item);
//        scale->SetTiming(cxTiming::CubicOut);
//        table->Append(item);
//    }
//    
//    Window()->Append(table);
    
//    cxSprite *vv = cxSprite::Create()->SetTexture("grid");
//    
//    cxProgress *v = cxProgress::Create(bg, vv, 0.5f);
//    v->SetSize(cxSize2F(300,50));
//    
//    Window()->Append(v);
    
//    cxSpline *x = cxSpline::Create();
//    x->Append(cxPoint2F(300, 0));
//    x->Append(cxPoint2F(300, 300));
//    x->Append(cxPoint2F(0, 300));
//    x->Append(cxPoint2F(-300, 300));
//    x->Append(cxPoint2F(-300, 0));
//    x->Append(cxPoint2F(-300, -300));
//    x->Append(cxPoint2F(0, -300));
//    x->Append(0.0f);
//    x->SetTime(10);
//    
//    cxAction *func = cxAction::Create(0.4f);
//    func->onStop +=[](cxAction *pav){
//        CX_LOGGER("run event");
//    };
//    func->AttachTo(Window());
    
//    cxBezier *xx1 = cxBezier::Create(cxPoint2F(300,600), cxPoint2F(500,1000),cxPoint2F(600,-600), 4.0f);
////    xx1->SetTiming(cxTiming::CubicIn);
////    cxTintTo *to1 = cxTintTo::Create(0.0f, 3.0f);
////    cxTintTo *to2 = cxTintTo::Create(1.0f, 3.0f);
//    cxSequence *run = cxSequence::Create();
//    run->Append(xx1)->Append(xx1->Reverse());
//    
//    run->onStop += [](cxAction *pav){
//        CX_LOGGER("run action stop");
//    };
    
//    cxSprite *sp = cxSprite::Create()->SetTexture("t.png");
//    sp->EnableDir(true);
//    sp->SetFrame(-800, 0, 100, 100);
    
//    cxJump *j = cxJump::Create(cxPoint2F(800,0), 100, 6, 5);
//    j->AttachTo(sp);
    
//    run->AttachTo(sp);
    
//    cxResizeTo *to = cxResizeTo::Create(300.0f, 2.0f);
//    to->AttachTo(sp);
//    to->onStop += [](cxAction *pav){
//        pav->View()->Append(pav->Reverse());
//    };
    
//    Window()->Append(sp);
    
//    cxSprite *fp = cxSprite::Create()->SetTexture("t.png");
//    fp->EnableDir(true);
//    fp->SetColor(cxColor4F::RED);
//    fp->SetFrame(500, 500, 100, 100);
//    
//    cxFollow *fa = cxFollow::Create(sp, 100);
//    fa->onCollide += [](cxFollow *pav){
//        pav->SetExit(true);
//    };
//    fa->AttachTo(fp);
//    
//    Window()->Append(fp);
    
//    cxHttp::Get("http://www.sina.com.cn")->onCompleted += [](cxHttp *http){
//        CX_LOGGER("%s",http->Body()->Data());
//    };
    
//    cxTcp *tcp = cxTcp::Alloc();
//    tcp->Connect("www.sina.com.cn", 80);

    
//    cxContainer *c = cxContainer::Create();
//    c->SetResizeFlags(cxView::ResizeFill);
//    
//    cxSprite *body = cxSprite::Create("bg");
//    body->SetAnchor(cxPoint2F(-0.0f, 0.0f));//left
//    body->SetSize(cxSize2F(2560, 1920));
//    c->Append(body);
//    
//    cxEmitter *v = cxEmitter::Create(250);
//    v->SetSystemTime(-1);
//    v->SetSize(60.0f);
//    v->SetTexture("t.png");
//    c->Append(v);
    
//    cxTimeLine *ani = cxTimeLine::Create();
//    ani->Append(1);//0
//    ani->Append(2);//1
//    ani->Append(3);//2
//    ani->Append(4);//3
//    ani->Append(1);//4
//    ani->Append(1);//5
//    ani->SetRange(3, 1);
//    ani->onStop += [](cxAction *s){
//        CX_LOGGER("stop");
//    };
//    ani->onTime += [](cxTimeLine *pav,const cxTimePoint *tp){
//        CX_LOGGER("%d",pav->Index());
//    };
////
//    Window()->Append(ani);
    
//    cxAnimate *animate = cxAnimate::Create();
//    animate->SetTexture("animate");
//    for(cxInt i=0;i<5;i++){
//        animate->AppFmt(0.5f, "120110%d.0.png",i+2);
//    }
//    
//    cxSprite *sp = cxSprite::Create();
//    sp->SetSize(cxSize2F(300, 300));
//    Window()->Append(sp);
//    
//    sp->Append(animate->SetRepeat(INT_MAX));
    
//    cxTimer *timer = cxTimer::Forever(1.0f);
//    timer->SetSpeed(2);
//    timer->onArrive +=[](cxTimer *pav){
//        CX_LOGGER("arrive");
//    };
//    Window()->Append(timer);
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
//        cxSprite *sp = cxObject::create("cxSprite")->To<cxSprite>();
//        sp->SetTexture("animate");
//        sp->SetFixScale(0.2f);
//        sp->SetTexKey("1201100.0.png");
//        sp->SetSize(cxSize2F(400, 400));
//        sp->SetPosition(cxPoint2F(cxUtil::Rand(-1024, 1024),cxUtil::Rand(-1024, 1024)));
//        sp->SetAnchor(-0.5f);
//        Window()->Append(sp);
//        
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

