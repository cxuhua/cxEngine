//
//  cxLoading.h
//  godcity
//
//  Created by xuhua on 7/14/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#ifndef godcity_cxLoading_h
#define godcity_cxLoading_h

#include "cxSequence.h"
#include "cxAsync.h"
#include "cxView.h"

CX_CPP_BEGIN

class cxLoading : public cxView
{
public:
    CX_DECLARE(cxLoading);
protected:
    explicit cxLoading();
    virtual ~cxLoading();
private:
    cxSequence *items;
    cxInt error;
public:
    //加载错误
    cxEvent<cxLoading,cxAsync *> onError;
    //当加载完成 完成
    cxEvent<cxLoading> onCompleted;
    //加载进度 当前/总数
    cxEvent<cxLoading,cxInt,cxInt> onProgress;
    //执行方法
    void Run(std::function<void(cxLoading *pview)> func);
    void Async(cxAsync *async);
    void Stop();
};

CX_CPP_END

#endif


