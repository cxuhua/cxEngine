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
    time = 0;
}

cxTimePoint::~cxTimePoint()
{
    
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
        to = frames->Size() - 1;
    }
    elapsedTime = 0;
    times.clear();
    cxFloat time = 0;
    for(cxInt i = from;i < Size();i++){
        cxTimePoint *frame = At(i);
        times.push_back(time);
        time += frame->time;
    }
    SetTime(time);
}

cxInt cxTimeLine::Index() const
{
    return idx;
}

cxTimePoint *cxTimeLine::At(cxInt idx)
{
    return frames->At<cxTimePoint>(from + idx);
}

cxTimePoint *cxTimeLine::Push(cxFloat time)
{
    cxTimePoint *p = cxTimePoint::Create()->Init(time);
    frames->Append(p);
    return p;
}

cxTimeLine *cxTimeLine::SetRange(cxInt afrom,cxInt ato)
{
    CX_ASSERT(afrom < Size() && to < Size(), "range error");
    from = afrom;
    to = ato;
    idx = 0;
    return this;
}

cxInt cxTimeLine::Size() const
{
    return abs(to - from + 1);
}

cxTimeLine::cxTimeLine()
{
    idx = 0;
    from = -1;
    to = -1;
    frames = cxArray::Alloc();
}

cxTimeLine::~cxTimeLine()
{
    frames->Release();
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

