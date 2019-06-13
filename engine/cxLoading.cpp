//
//  cxLoading.cpp
//  godcity
//
//  Created by xuhua on 7/14/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#include <core/cxLooper.h>
#include "cxLoading.h"

CX_CPP_BEGIN

CX_IMPLEMENT(cxLoading);

cxLoading::cxLoading()
{
    error = 0;
    SetResizeFlags(ResizeFill);
    
    items = cxSequence::Create();
    Append(items);
    items->onAction += [this](cxSequence *pav,cxAction *sub){
        cxAsync *async = sub->To<cxAsync>();
        //has error
        if(error != 0){
            return;
        }
        //invoke error
        error = async->ErrorCode();
        if(error != 0){
            onError.Fire(this, async);
            Stop();
        }else{
            cxInt i = pav->Index();
            cxInt a = pav->Size();
            onProgress.Fire(this, i, a);
        }
    };
    items->onExit += [this](cxAction *pav){
        //not error invoke onCompleted
        if(error == 0){
            onCompleted.Fire(this);
        }
        Remove();
    };
}

cxLoading::~cxLoading()
{
    
}

void cxLoading::Stop()
{
    items->Exit(true);
}

void cxLoading::Async(cxAsync *async)
{
    async->AttachTo(this);
    items->Append(async);
}

void cxLoading::Run(std::function<void(cxLoading *pview)> func)
{
    cxAsync *async = cxAsync::Create();
    async->onStep += [this,func](cxAction *pav,cxFloat dt){
        cxLooper::Looper()->Push();
        func(this);
        cxLooper::Looper()->Pop();
        pav->To<cxAsync>()->SetFinished(true);
    };
    async->AttachTo(this);
    items->Append(async);
}

CX_CPP_END

