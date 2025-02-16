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
    return time < 0.3f && length < 6.0f;
}

cxTouchable::cxTouchable()
{
    
}

cxTouchable::~cxTouchable()
{
    
}

void cxTouchable::updateEvent(const cxTouchPoint &e)
{
    std::map<cxTouchId,cxTouchPoint>::iterator it = events.find(e.key);
    cxDouble now = cxUtil::Timestamp();
    cxTouchPoint newEvent = e;
    cxTouchPoint *event = nullptr;
    cxBool add = false;
    if(it != events.end()){
        event = &it->second;
        event->type = e.type;
        event->wp = e.wp;
    }else{
        event = &newEvent;
        add = true;
    }
    if(e.type == cxTouchPoint::Began){
        event->movement = 0.0f;
        event->time = now;
        event->prev = e.wp;
        event->length = 0;
    }else if(e.type == cxTouchPoint::Moved){
        event->delta = e.wp - event->prev;
        event->prev = e.wp;
        event->movement += event->delta;
        event->length += event->delta.Length();
    }else{
        event->time = now - event->time;
    }
    if(add){
        events.emplace(e.key,newEvent);
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

cxBool cxTouchable::Dispatch(cxKeyType type,cxInt code)
{
    cxKey key;
    key.code = code;
    key.type = type;
    if(cxEngine::Instance()->IsTouch()){
        return cxEngine::Instance()->Window()->Dispatch(key);
    }
    return false;
}

void cxTouchable::OnDispatch(const cxTouchable *e)
{
    
}

void cxTouchable::Dispatch(cxTouchId key,cxTouchType type,cxFloat x,cxFloat y)
{
    cxSize2F winsiz = cxEngine::Instance()->WinSize() * 0.5f;
    cxFloat scale = cxEngine::Instance()->ScaleFactor();
    cxTouchPoint e;
    e.key  = key;
    e.type = type;
    //convert to window coord
    e.wp = cxPoint2F(x * scale - winsiz.w, winsiz.h - y * scale);
    //
    if(e.type != cxTouchPoint::None){
        updateEvent(e);
    }
    items.clear();
    for(std::map<cxTouchId,cxTouchPoint>::iterator it=events.begin();it!=events.end();it++){
        items.push_back(&it->second);
    }
    //
    if(cxEngine::Instance()->IsTouch()){
        cxEngine::Instance()->OnDispatch(this);
        cxEngine::Instance()->Window()->Dispatch(this);
    }
    //
    if(e.type == cxTouchPoint::Ended){
        removeEvent(e);
    }
}

CX_CPP_END








