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
    cxStr *data;
    cxInt seed;
public:
    //随机初始化一个seed
    cxInt InitSeed();
    //设置固定的seed
    void SetSeed(cxInt v);
    //获取一个随机值
    cxFloat RandFloat();
    cxByte RandByte();
    cxInt Rand(cxInt min,cxInt max);
};

CX_CPP_END

#endif


