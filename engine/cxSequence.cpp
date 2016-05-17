//
//  cxSequence.cpp
//  cxEngineCore
//
//  Created by xuhua on 7/5/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#include "cxView.h"
#include "cxSequence.h"

CX_CPP_BEGIN

CX_IMPLEMENT(cxSequence);

cxSequence::cxSequence()
{
    Forever();
    index = 0;
    actions = cxArray::Alloc();
}

cxSequence::~cxSequence()
{
    actions->Release();
}

void cxSequence::OnInit()
{
    if(actions->Size() == 0){
        Exit(true);
        return;
    }
}

void cxSequence::OnStep(cxFloat dt)
{
    CX_ASSERT(index < actions->Size(), "index error");
    cxAction *pav = actions->At(index)->To<cxAction>();
    if(pav->Update(dt)){
        onAction.Fire(this,pav);
        index ++;
    }
    if(index >= actions->Size()){
        Exit(true);
    }
}

const cxInt cxSequence::Size() const
{
    return actions->Size();
}

const cxInt cxSequence::Index() const
{
    return index;
}

cxSequence *cxSequence::Append(cxAction *pav,cxView *pview)
{
    CX_ASSERT(pav != nullptr && pview != nullptr, "args error");
    pav->SetView(pview);
    actions->Append(pav);
    return this;
}

CX_CPP_END

