//
//  cxMultiple.cpp
//  cxEngineCore
//
//  Created by xuhua on 5/17/16.
//  Copyright © 2016 xuhua. All rights reserved.
//


#include "cxMultiple.h"

CX_CPP_BEGIN

CX_IMPLEMENT(cxMultiple);


cxMultiple::cxMultiple()
{
    Forever();
    actions = cxArray::Alloc();
    index = 0;
}

cxMultiple::~cxMultiple()
{
    actions->Release();
}

void cxMultiple::OnInit()
{
    cxAction::OnInit();
    if(actions->Size() == 0){
        Exit(true);
    }
}

void cxMultiple::OnExit()
{
    for(cxInt i=0;i<actions->Size();i++){
        actions->At(i)->To<cxAction>()->Exit(true);
    }
    cxAction::OnExit();
}

void cxMultiple::Stop()
{
    for(cxInt i=0;i<actions->Size();i++){
        actions->At(i)->To<cxAction>()->Stop();
    }
    cxAction::Stop();
}

void cxMultiple::Exit(cxBool v)
{
    for(cxInt i=0;i<actions->Size();i++){
        actions->At(i)->To<cxAction>()->Exit(v);
    }
    cxAction::Exit(v);
}

const cxInt cxMultiple::Index() const
{
    return index;
}

const cxInt cxMultiple::Size() const
{
    return actions->Size();
}

void cxMultiple::actionExit(cxAction *pav)
{
    index ++;
    onAction.Fire(this,pav);
    if(actions->Size() == index){
        cxAction::Exit(true);
    }
}

cxMultiple *cxMultiple::Append(cxAction *pav)
{
    CX_ASSERT(pav != nullptr, "args error");
    pav->onExit +=[this](cxAction *pav){
        actionExit(pav);
    };
    actions->Append(pav);
    return this;
}

CX_CPP_END

