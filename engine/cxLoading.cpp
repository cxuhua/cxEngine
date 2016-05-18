//
//  cxLoading.cpp
//  godcity
//
//  Created by xuhua on 7/14/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#include <core/cxAutoPool.h>
#include "cxLoading.h"

CX_CPP_BEGIN

CX_IMPLEMENT(cxLoading);

cxLoading::cxLoading()
{
    SetResizeFlags(ResizeFill);
    items = cxSequence::Create();
    Append(items);
    items->onAction += [this](cxSequence *pav,cxAction *sub){
        cxInt i = pav->Index();
        cxInt a = pav->Size();
        onProgress.Fire(this, i, a);
    };
    items->onExit += [this](cxAction *pav){
        cxSequence *seq = pav->To<cxSequence>();
        cxInt i = seq->Index();
        cxInt a = seq->Size();
        Remove();
        onCompleted.Fire(this, i == a);
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
        cxAutoPool::Push();
        func(this);
        cxAutoPool::Pop();
        pav->To<cxAsync>()->SetFinished(true);
    };
    async->AttachTo(this);
    items->Append(async);
}

CX_CPP_END

