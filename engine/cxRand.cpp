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

const cxUInt32 maxshort = 65535U;
const cxUInt32 multiplier = 1194211693U;
const cxUInt32 adder = 12345U;

cxRand::cxRand()
{
    randSeed = 0;
}

cxRand::~cxRand()
{
    
}

void cxRand::SetSeed(cxUInt32 s)
{
    randSeed = s;
}

cxUInt32 cxRand::Int()
{
    randSeed = multiplier * randSeed + adder;
    return (cxUInt32)((randSeed >> 16) % maxshort);
}

cxUInt32 cxRand::Int(cxUInt32 min,cxUInt32 max)
{
     return min + (max - min) * Double();
}

cxDouble cxRand::Double()
{
    return (cxDouble)Int()/(cxDouble)maxshort;
}

CX_CPP_END

