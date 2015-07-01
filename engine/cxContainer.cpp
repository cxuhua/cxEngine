//
//  cxContainer.cpp
//  cxEngineCore
//
//  Created by xuhua on 6/28/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//


#include "cxScale.h"
#include "cxMove.h"
#include "cxContainer.h"

CX_CPP_BEGIN

CX_IMPLEMENT(cxContainer);

cxContainer *cxContainer::SetScaleRange(const cxRange2F &v)
{
    scalerange = v;
    return this;
}

cxContainer *cxContainer::EnableScale(cxBool v)
{
    isscaling = v;
    return this;
}

cxContainer *cxContainer::EnableSliding(cxBool v)
{
    issliding = v;
    return this;
}

cxContainer *cxContainer::SetSlidingType(SlidingType v)
{
    slidingtype = v;
    if(slidingtype & Left){
        bodyanchor.x = -0.5f;
    }
    if(slidingtype & Right){
        bodyanchor.x = +0.5f;
    }
    if(slidingtype & Top){
        bodyanchor.y = +0.5f;
    }
    if(slidingtype & Bottom){
        bodyanchor.y = -0.5f;
    }
    return this;
}

cxContainer *cxContainer::SetSlidingSpeed(cxFloat v)
{
    slidingspeed = v;
    return this;
}

cxContainer *cxContainer::SetSlidingTime(cxFloat v)
{
    slidingtime = v;
    return this;
}

cxContainer::cxContainer()
{
    bodyanchor = 0.0f;
    scaleinc = 0.3f;
    scalerange = cxRange2F(0.9f,2.0f);
    scaling = 0.05f;
    isscaling = true;
    issliding = true;
    slidingtype = Horizontal|Vertical;
    slidingtime = 1.0f;
    slidingspeed = 0.1f;
}

cxContainer::~cxContainer()
{
    
}

cxContainer *cxContainer::FixPosition()
{
    cxPoint2F pos = Body()->Position();
    pos = fixPosition(pos);
    Body()->SetPosition(pos);
    return this;
}

void cxContainer::OnAppend(cxView *nview)
{
    if(nview == Body()){
        Body()->SetAnchor(bodyanchor);
        FixPosition();
    }
}

cxBool cxContainer::scale(const cxengine::cxTouchable *e)
{
    const cxTouchPoint *ep0 = e->TouchPoint(0);
    cxHitInfo hit0 = Body()->HitTest(ep0->wp);
    
    const cxTouchPoint *ep1 = e->TouchPoint(1);
    cxHitInfo hit1 = Body()->HitTest(ep1->wp);
    
    if(hit0.hited && hit1.hited && (ep0->IsBegan() || ep1->IsBegan())){
        Body()->StopAction(CX_CONTAINER_MOVE_ID);
        scaledis = cxPoint2F::Distance(ep0->wp,ep1->wp);
        cxPoint2F cp = cxPoint2F::Center(hit0.point, hit1.point);
        cxPoint2F ap = Body()->ToAnchor(cp);
        Body()->SetAnchor(ap,true);
        FixPosition();
        return false;
    }
    if(ep0->IsEnded() || ep1->IsEnded()){
        cxPoint2F scale = Body()->Scale();
        if(scale.x > scalerange.max || scale.y > scalerange.max){
            cxScaleTo *sa = cxScaleTo::Create(scalerange.max, 0.5f);
            sa->SetTiming(cxTiming::CubicOut);
            Body()->Append(sa);
        }
        return false;
    }
    if(ep0->IsMoved() || ep1->IsMoved()){
        cxFloat dis = cxPoint2F::Distance(ep0->wp,ep1->wp);
        cxFloat delta = dis - scaledis;
        scaledis = dis;
        cxPoint2F scale = Body()->Scale();
        if(delta > 0){
            scale.x += scaling;
            scale.y += scaling;
        }else if(delta < 0){
            scale.x -= scaling;
            scale.y -= scaling;
        }else{
            return false;
        }
        scale.x = cxFloatClamp(scale.x, scalerange.min, scalerange.max + scaleinc);
        scale.y = cxFloatClamp(scale.y, scalerange.min, scalerange.max + scaleinc);
        Body()->SetScale(scale);
        FixPosition();
        return true;
    }
    return false;
}

cxBool cxContainer::OnDispatch(const cxTouchable *e)
{
    if(e->TouchCount() == 2 && isscaling){
        return scale(e);
    }
    if(e->TouchCount() != 1 || !issliding){
        return false;
    }
    const cxTouchPoint *ep = e->TouchPoint(0);
    cxHitInfo hit = HitTest(ep->wp);
    if(!hit.hited){
        return false;
    }
    if(ep->IsBegan()){
        Body()->StopAction(CX_CONTAINER_MOVE_ID);
        return true;
    }
    if(ep->IsMoved()){
        cxPoint2F pos = Body()->Position();
        if(slidingtype & Horizontal){
            pos.x += ep->delta.x;
        }
        if(slidingtype & Vertical){
            pos.y += ep->delta.y;
        }
        pos = fixPosition(pos);
        Body()->SetPosition(pos);
        return true;
    }
    if(ep->IsEnded()){
        cxPoint2F sp = ep->Speed();
        if(sp.Length() < 1300){
            return false;
        }
        cxPoint2F pos = Body()->Position();
        if(slidingtype & Horizontal){
            pos.x += slidingspeed * sp.x;
        }
        if(slidingtype & Vertical){
            pos.y += slidingspeed * sp.y;
        }
        pos = fixPosition(pos);
        cxMoveTo *move = cxMoveTo::Create(pos, slidingtime);
        move->SetID(CX_CONTAINER_MOVE_ID);
        move->SetTiming(cxTiming::CubicOut);
        Body()->Append(move);
        return true;
    }
    return false;
}

cxPoint2F cxContainer::fixPosition(const cxPoint2F &pos)
{
    cxPoint2F rv = pos;
    cxView *body = Body();
    cxBox4F pbox = BoxPoint().ToBox4F();
    cxBox4F vbox = body->ToBox4F() * body->TransScale();
    cxBox4F nbox = vbox + pos;
    if(nbox.l > pbox.l){
        rv.x -= (nbox.l - pbox.l);
    }
    if(nbox.r < pbox.r){
        rv.x -= (nbox.r - pbox.r);
    }
    if(nbox.t < pbox.t){
        rv.y -= (nbox.t - pbox.t);
    }
    if(nbox.b > pbox.b){
        rv.y -= (nbox.b - pbox.b);
    }
    return rv;
}

cxView *cxContainer::Body()
{
    CX_ASSERT(Subviews()->Size() > 0, "first subview use body");
    return Subviews()->At<cxView>(0);
}

CX_CPP_END

