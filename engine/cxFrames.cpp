//
//  cxFrames.cpp
//  cxEngineCore
//
//  Created by xuhua on 7/8/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//


#include "cxFrames.h"

CX_CPP_BEGIN

CX_IMPLEMENT(cxFrames);

cxFrames::cxFrames()
{
    mapnum = 0;
    layer = 0;
    group = 0;
    offset = 0;
    time = 0.1f;
    count = 0;
    ptex = nullptr;
    points = cxArray::Alloc();
}

cxFrames::~cxFrames()
{
    cxObject::release(&ptex);
    points->Release();
}

const cxInt cxFrames::MapNum() const
{
    return mapnum;
}

const cxInt *cxFrames::Map() const
{
    return map;
}

void cxFrames::InitMaps(const cxStr *str)
{
    const cxArray *maps = str->Split(',');
    CX_ASSERT(maps->Size() > 0, "map set error");
    mapnum = maps->Size();
    for(cxInt i=0;i<mapnum;i++){
        map[i] = maps->At(i)->To<cxStr>()->ToInt();
        CX_ASSERT(map[i] < layer, "map value error");
    }
}

cxFrames *cxFrames::SetTexture(const cxTexture *atex)
{
    cxObject::swap(&ptex, atex);
    return this;
}

const cxTexture *cxFrames::Texture() const
{
    return ptex;
}

cxFrames *cxFrames::SetOffset(cxInt v)
{
    offset = v;
    return this;
}

const cxInt cxFrames::Offset() const
{
    return offset;
}

cxFrames *cxFrames::SetGroup(cxInt v)
{
    group = v;
    return this;
}

const cxInt cxFrames::Group() const
{
    return group;
}

cxFrames *cxFrames::SetLayer(cxInt v)
{
    layer = v;
    return this;
}

const cxInt cxFrames::Layer() const
{
    return layer;
}

cxFrames *cxFrames::SetCount(cxInt v)
{
    count = v;
    return this;
}

const cxInt cxFrames::Count() const
{
    return count;
}

cxFrames *cxFrames::SetTime(cxFloat v)
{
    time = v;
    return this;
}

const cxFloat cxFrames::Time() const
{
    return time;
}

const cxTimePoint *cxFrames::At(cxInt idx) const
{
    return points->At(idx)->To<cxTimePoint>();
}

cxTimePoint *cxFrames::Append(cxFloat time)
{
    cxTimePoint *tp = cxTimePoint::Create()->Init(time);
    points->Append(tp);
    return tp;
}

const cxArray *cxFrames::Points() const
{
    return points;
}

CX_CPP_END

