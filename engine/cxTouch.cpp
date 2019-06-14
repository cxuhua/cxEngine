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
    
}

cxTouchable::~cxTouchable()
{
    
}

void cxTouchable::updateEvent(const cxTouchPoint &e)
{
    Events::iterator it = events.find(e.key);
    cxDouble now = cxUtil::Timestamp();
    cxTouchPoint newtp = e;
    cxTouchPoint *tp = nullptr;
    cxBool add = false;
    if(it != events.end()){
        tp = &it->second;
        tp->type = e.type;
        tp->wp = e.wp;
    }else{
        tp = &newtp;
        add = true;
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
    if(add){
        events[e.key]= newtp;
    }
}

void cxTouchable::removeEvent(const cxTouchPoint &e)
{
    events.erase(e.key);
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
    cxBool ret = false;
    if(ep->IsTouch()){
        ret = ep->OnDispatch(key);
        ret = (!ret) && ep->Window()->Dispatch(key);
    }
    return ret;
}

void cxTouchable::OnDispatch(const cxTouchable *e)
{
    
}

void cxTouchable::Dispatch(cxTouchId key,cxTouchType type,cxFloat x,cxFloat y)
{
    cxEngine *ep = cxEngine::Instance();
    cxSize2F winsiz = cxEngine::Instance()->WinSize() * 0.5f;
    cxFloat scale = cxEngine::Instance()->ScaleFactor();
    cxTouchPoint e;
    e.key  = key;
    e.type = type;
    e.wp = cxPoint2F(x * scale - winsiz.w, winsiz.h - y * scale);
    if(e.type != cxTouchPoint::None){
        updateEvent(e);
    }
    items.clear();
    for(auto v : events){
        items.push_back(&v.second);
    }
    if(ep->IsTouch()){
        ep->OnDispatch(this);
        ep->Window()->Dispatch(this);
    }
    if(e.type == cxTouchPoint::Ended){
        removeEvent(e);
    }
}

CX_CPP_END








