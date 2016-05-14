//
//  cxRand.cpp
//  godcity
//
//  Created by xuhua on 9/18/15.
//  Copyright © 2015 xuhua. All rights reserved.
//

#include <core/cxUtil.h>
#include "cxRand.h"

CX_CPP_BEGIN

CX_IMPLEMENT(cxRand);

cxRand::cxRand()
{
    data = nullptr;
    seed = 0;
    cxObject::swap(&data, cxUtil::Assets("data.rtf"));
    CX_ASSERT(data != nullptr, "data.rtf miss");
}

cxRand::~cxRand()
{
    cxObject::release(&data);
}

cxInt cxRand::InitSeed()
{
    cxUtil::SetRandSeed();
    seed = cxUtil::Rand(0, data->Size() - 1);
    return seed;
}

cxInt cxRand::Rand(cxInt min,cxInt max)
{
    return min + (max - min) * RandFloat();
}

void cxRand::SetSeed(cxInt v)
{
    seed = v%data->Size();
}

cxFloat cxRand::RandFloat()
{
    return ((cxFloat)RandByte())/255.0f;
}

cxByte cxRand::RandByte()
{
    cxByte *buf = (cxByte *)data->Buffer();
    cxByte v = buf[seed++];
    if(seed >= data->Size()){
        seed = 0;
    }
    if(seed < 0){
        seed = data->Size() - 1;
    }
    return v;
}

CX_CPP_END

