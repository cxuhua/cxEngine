//
//  cxTimeLine.cpp
//  cxEngineCore
//
//  Created by xuhua on 7/2/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#include "cxSprite.h"
#include "cxTimeLine.h"

CX_CPP_BEGIN

CX_IMPLEMENT(cxTimePoint);

cxTimePoint::cxTimePoint()
{
    object = nullptr;
    time = 0.1f;
}

cxTimePoint::~cxTimePoint()
{
    cxObject::release(&object);
}

cxBool cxTimePoint::SetTime(cxFloat v)
{
    cxBool equ = !cxFloatIsEqual(time, v);
    time = v;
    return equ;
}

cxFloat cxTimePoint::Time() const
{
    return time;
}

cxObject *cxTimePoint::Object() const
{
    return object;
}

void cxTimePoint::SetObject(cxObject *pobj)
{
    cxObject::swap(&object, pobj);
}

cxTimePoint *cxTimePoint::Init(cxFloat at)
{
    time = at;
    return this;
}

CX_IMPLEMENT(cxTimeLine);

void cxTimeLine::OnInit()
{
    prev = -1;
    idx = 0;
    if(from < 0){
        from = 0;
    }
    if(to < 0){
        to = points->Size() - 1;
    }
    UpdateTime();
}

void cxTimeLine::OnTime(const cxTimePoint *tp)
{
    onTime.Fire(this,tp);
}

const cxTimePoint *cxTimeLine::TimePoint() const
{
    return At(idx);
}

cxAction *cxTimeLine::Reverse()
{
    cxTimeLine *rv = cxTimeLine::Create();
    rv->from = to;
    rv->to = from;
    cxObject::swap(&rv->points, points);
    return rv;
}
cxAction *cxTimeLine::Clone()
{
    cxTimeLine *rv = cxTimeLine::Create();
    rv->from = from;
    rv->to = to;
    cxObject::swap(&rv->points, points);
    return rv;
}

void cxTimeLine::UpdateTime()
{
    if(!isdirty){
        return;
    }
    times.clear();
    cxFloat time = 0;
    for(cxInt i = 0;i < Length();i++){
        if(timeType == TimeTypeLine){
            time = At(i)->Time();
            times.push_back(time);
        }else if(looptime > 0){
            times.push_back(time);
            time += looptime/(cxFloat)Length();
        }else{
            times.push_back(time);
            time +=  At(i)->Time();
        }
    }
    SetTime(time);
    isdirty = false;
}

const cxInt cxTimeLine::Index() const
{
    return (from < to) ? (from + idx) : (from - idx);
}

const cxTimePoint *cxTimeLine::At(cxInt i) const
{
    cxInt idx = (from < to) ? (from + i) : (from - i);
    return points->At(idx)->To<cxTimePoint>();
}

cxTimePoint *cxTimeLine::At(cxInt i)
{
    cxInt idx = (from < to) ? (from + i) : (from - i);
    return points->At(idx)->To<cxTimePoint>();
}

cxTimeLine *cxTimeLine::SetPoints(const cxArray *ps)
{
    CX_ASSERT(ps != nullptr && ps->Size() > 0, "points empty");
    cxObject::swap(&points, ps);
    return this;
}

cxTimePoint *cxTimeLine::Append(cxFloat time)
{
    cxTimePoint *p = cxTimePoint::Create()->Init(time);
    points->Append(p);
    return p;
}

const cxArray *cxTimeLine::Points() const
{
    return points;
}

cxArray *cxTimeLine::Points()
{
    return points;
}

cxTimeLine *cxTimeLine::SetRange(cxInt afrom,cxInt ato)
{
    CX_ASSERT(afrom < points->Size() && to < points->Size(), "range error");
    if(from == afrom && to == ato){
        return this;
    }
    isdirty = true;
    from = afrom;
    to = ato;
    Reset();
    return this;
}

void cxTimeLine::SetLoopTime(cxFloat v)
{
    looptime = v;
}

const cxInt cxTimeLine::Length() const
{
    return abs(to - from) + 1;
}

const cxInt cxTimeLine::From() const
{
    return from;
}

const cxInt cxTimeLine::To() const
{
    return to;
}

void cxTimeLine::SetTimeType(TimeType v)
{
    timeType = v;
}

cxTimeLine::cxTimeLine()
{
    timeType = TimeTypeFrame;
    looptime = 0;
    isdirty = true;
    idx = -1;
    from = -1;
    to = -1;
    points = cxArray::Alloc();
    Forever();
}

cxTimeLine::~cxTimeLine()
{
    points->Release();
}

void cxTimeLine::OnStep(cxFloat dt)
{
    cxFloat elapsed = Elapsed();
    for(cxInt i = idx;i<Length();i++){
        if(elapsed < times.at(i)){
            break;
        }
        if(prev == i){
            continue;
        }
        idx = i;
        prev = i;
        OnTime(At(idx));
    }
}

CX_CPP_END

