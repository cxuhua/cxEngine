//
//  cxTouch.h
//  cxEngineCore
//
//  Created by xuhua on 6/26/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#ifndef cxEngineCore_cxTouch_h
#define cxEngineCore_cxTouch_h

#include <map>
#include <vector>
#include <math/cxMath.h>
#include <math/cxPoint2F.h>

CX_CPP_BEGIN

class cxView;

typedef cxUInt cxTouchType;
typedef cxULong cxTouchId;

struct cxHitInfo
{
    cxPoint2F point;
    cxBool hited;
    cxHitInfo();
};

struct cxTouchPoint
{
    cxTouchId key;
    cxTouchType type;
    cxUInt tap;
    cxPoint2F wp;
    cxPoint2F prev;
    cxPoint2F movement;
    cxFloat length;
    cxPoint2F delta;
    cxDouble time;
    
    static const cxTouchType None   = 0;
    static const cxTouchType Began  = 1;
    static const cxTouchType Moved  = 2;
    static const cxTouchType Ended  = 3;
    
    cxTouchPoint();
    
    cxFloat Length() const;
    cxPoint2F Speed() const;
    cxBool IsBegan() const;
    cxBool IsMoved() const;
    cxBool IsEnded() const;
    cxBool IsTap() const;
};

class cxTouchable
{
protected:
    explicit cxTouchable();
    virtual ~cxTouchable();
private:
    
    std::map<cxTouchId,cxTouchPoint> events;
    std::vector<cxTouchPoint *> items;
    
    void updateEvent(const cxTouchPoint &e);
    void removeEvent(const cxTouchPoint &e);
public:
    const cxInt TouchCount() const;
    const cxTouchPoint *TouchPoint(cxInt idx) const;
    const cxHitInfo HitTest(cxView *view,const cxPoint2F &wp) const;
    void Dispatch(cxTouchId key,cxTouchType type,cxUInt tap,cxFloat x,cxFloat y);
};

CX_CPP_END

#endif


