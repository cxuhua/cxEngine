//
//  cxRand.h
//  godcity
//  可控制的随机数生成
//  Created by xuhua on 9/18/15.
//  Copyright © 2015 xuhua. All rights reserved.
//

#ifndef godcity_cxRand_h
#define godcity_cxRand_h

#include <core/cxStr.h>

CX_CPP_BEGIN

class cxRand : public cxObject
{
public:
    CX_DECLARE(cxRand);
protected:
    explicit cxRand();
    virtual ~cxRand();
private:
    cxUInt32 randSeed;
public:
    void SetSeed(cxUInt32 s = 0);
    cxUInt32 Int();
    cxUInt32 Int(cxUInt32 min,cxUInt32 max);
    cxDouble Double();
};

CX_CPP_END

#endif


