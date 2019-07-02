//
//  cxTouch.cpp
//  cxEngineCore
//
//  Created by xuhua on 6/26/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#include <core/cxUtil.h>
#include "cxEngine.h"
#include "cxTouch.h"
#include "cxView.h"

CX_CPP_BEGIN

cxHitInfo::cxHitInfo()
{
    hited = false;
}

cxTouchPoint::cxTouchPoint()
{
    flags = FlagsGestureTypeNone;
}

cxPoint2F cxTouchPoint::Speed() const
{
    if(cxFloatIsEqual(time, 0)){
        return 0.0f;
    }
    return movement/time;
}

cxFloat cxTouchPoint::Length() const
{
    return length;
}

cxBool cxTouchPoint::IsBegan() const
{
    return type == Began;
}

cxBool cxTouchPoint::IsMoved() const
{
    return type == Moved;
}

cxBool cxTouchPoint::IsEnded() const
{
    return type == Ended;
}

cxBool cxTouchPoint::IsTap() const
{
    return time < 0.3f && length < 15.0f;
}

cxTouchPoint::FlagsGestureType *cxTouchPoint::Flags() const
{
    return (FlagsGestureType *)&flags;
}

cxTouchable::cxTouchable()
{
    for(int i=0;i<MAX_TOUCH_SIZE;i++){
        events[i].key = -1;
    }
}

cxTouchable::~cxTouchable()
{
    
}

cxTouchPoint *cxTouchable::findPoint(cxTouchId key)
{
    for(int i=0;i<MAX_TOUCH_SIZE;i++){
        if(events[i].key == key){
            return &events[i];
        }
    }
    return nullptr;
}

void cxTouchable::updateEvent(const cxTouchPoint &e)
{
    cxTouchPoint *op = findPoint(e.key);
    cxDouble now = cxUtil::Timestamp();
    cxTouchPoint *tp = nullptr;
    if(op != nullptr){
        tp = op;
        tp->type = e.type;
        tp->wp = e.wp;
    }else{
        tp = findPoint(-1);
        *tp = e;
    }
    if(e.type == cxTouchPoint::Began){
        tp->movement = 0.0f;
        tp->time = now;
        tp->prev = e.wp;
        tp->length = 0;
    }else if(e.type == cxTouchPoint::Moved){
        tp->delta = e.wp - tp->prev;
        tp->prev = e.wp;
        tp->movement += tp->delta;
        tp->length += tp->delta.Length();
    }else{
        tp->time = now - tp->time;
    }
}

void cxTouchable::removeEvent(const cxTouchPoint &e)
{
    cxTouchPoint *pt = findPoint(e.key);
    if(pt != nullptr){
        pt->key = -1;
    }
}

const cxInt cxTouchable::TouchCount() const
{
    return (cxInt)items.size();
}

const cxTouchPoint *cxTouchable::TouchPoint(cxInt idx) const
{
    return items.at(idx);
}

const cxHitInfo cxTouchable::HitTest(cxView *view,const cxPoint2F &wp) const
{
    cxHitInfo info;
    cxBox4F box = view->BoxPoint().ToBox4F();
    info.point = view->FromPoint(wp);
    info.hited = box.Contain(info.point);
    return info;
}

cxBool cxTouchable::OnDispatch(const cxKey &key)
{
    return false;
}

cxBool cxTouchable::Dispatch(cxKeyType type,cxInt code)
{
    cxEngine *ep = cxEngine::Instance();
    cxKey key;
    key.code = code;
    key.type = type;
    if(ep->IsTouch()){
        if(ep->OnDispatch(key))return true;
        if(ep->Window()->Dispatch(key)) return true;
    }
    return false;
}

cxBool cxTouchable::OnDispatch(const cxTouchable *e)
{
    return false;
}

void cxTouchable::Dispatch(cxTouchId key,cxTouchType type,cxFloat x,cxFloat y)
{
    cxEngine *ep = cxEngine::Instance();
    cxSize2F winsiz = ep->WinSize() * 0.5f;
    cxFloat scale = ep->ScaleFactor();
    cxTouchPoint e;
    e.key  = key;
    e.type = type;
    e.wp = cxPoint2F(x * scale - winsiz.w, winsiz.h - y * scale);
    if(e.type != cxTouchPoint::None){
        updateEvent(e);
    }
    items.clear();
    for(int i=0; i < MAX_TOUCH_SIZE; i++){
        cxTouchPoint *pt = &events[i];
        if(pt->key != -1){
            items.push_back(pt);
        }
    }
    if(ep->IsTouch() && !ep->OnDispatch(this)){
        ep->Window()->Dispatch(this);
    }
    if(e.type == cxTouchPoint::Ended){
        removeEvent(e);
    }
}

CX_CPP_END








