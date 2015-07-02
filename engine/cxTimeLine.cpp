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

#define FIDX(from,to,idx)       ((from<to)?(from+idx):(from-idx))

CX_IMPLEMENT(cxTimePoint);

cxTimePoint::cxTimePoint()
{
    object = nullptr;
    time = 0;
}

cxTimePoint::~cxTimePoint()
{
    cxObject::release(&object);
}

cxFloat cxTimePoint::Time() const
{
    return time;
}

cxObject *cxTimePoint::Object()
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
    previdx = -1;
    idx = 0;
    if(from < 0){
        from = 0;
    }
    if(to < 0){
        to = points->Size() - 1;
    }
    elapsedTime = 0;
    UpdateTime();
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
    times.clear();
    cxFloat time = 0;
    for(cxInt i = 0;i < Size();i++){
        times.push_back(time);
        time +=  At(i)->Time();
    }
    SetTime(time);
}

cxInt cxTimeLine::Index() const
{
    return idx;
}

cxTimePoint *cxTimeLine::At(cxInt i)
{
    return points->At<cxTimePoint>(FIDX(from, to, i));
}

cxTimePoint *cxTimeLine::Push(cxFloat time)
{
    cxTimePoint *p = cxTimePoint::Create()->Init(time);
    points->Append(p);
    return p;
}

cxTimeLine *cxTimeLine::SetRange(cxInt afrom,cxInt ato)
{
    CX_ASSERT(afrom < points->Size() && to < points->Size(), "range error");
    from = afrom;
    to = ato;
    idx = 0;
    return this;
}

cxInt cxTimeLine::Size() const
{
    return abs(to - from) + 1;
}

cxTimeLine::cxTimeLine()
{
    idx = 0;
    from = -1;
    to = -1;
    points = cxArray::Alloc();
}

cxTimeLine::~cxTimeLine()
{
    points->Release();
}

void cxTimeLine::OnStep(cxFloat dt)
{
    elapsedTime += dt;
    for(cxInt i = idx;i<Size();i++){
        if(elapsedTime < times.at(i)){
            break;
        }
        if(previdx == i){
            continue;
        }
        idx = i;
        previdx = i;
        onTime.Fire(this);
    }
}

CX_CPP_END

