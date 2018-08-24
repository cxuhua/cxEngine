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

void cxSequence::OnExit()
{
    for(cxInt i=0;i<actions->Size();i++){
        actions->At(i)->To<cxAction>()->Exit(true);
    }
    cxAction::OnExit();
}

void cxSequence::OnInit()
{
    cxAction::OnInit();
    if(actions->Size() == 0){
        cxAction::Exit(true);
    }else{
        actions->At(index)->To<cxAction>()->Resume();
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

void cxSequence::Stop()
{
    for(cxInt i=0;i<actions->Size();i++){
        actions->At(i)->To<cxAction>()->Stop();
    }
    cxAction::Stop();
}

void cxSequence::Exit(cxBool v)
{
    for(cxInt i=0;i<actions->Size();i++){
        actions->At(i)->To<cxAction>()->Exit(v);
    }
    cxAction::Exit(v);
}

void cxSequence::actionExit(cxAction *pav)
{
    index ++;
    onAction.Fire(this,pav);
    if(actions->Size() == index){
        cxAction::Exit(true);
    }else{
        actions->At(index)->To<cxAction>()->Resume();
    }
}

cxSequence *cxSequence::Append(cxAction *pav)
{
    CX_ASSERT(pav != nullptr, "args error");
    pav->onExit += [this](cxAction *pav){
        actionExit(pav);
    };
    pav->Pause();
    actions->Append(pav);
    return this;
}

CX_CPP_END

