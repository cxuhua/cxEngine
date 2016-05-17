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
    if(actions->Size() == 0){
        Exit(true);
        return;
    }
}

void cxMultiple::OnStep(cxFloat dt)
{
    index = 0;
    for(cxInt i=0; i < actions->Size();i++){
        cxAction *pav = actions->At(i)->To<cxAction>();
        if(pav->IsExit()){
            index++;
            continue;
        }
        if(pav->Update(dt)){
            onAction.Fire(this,pav);
            index++;
            continue;
        }
    }
    if(actions->Size() == index){
        Exit(true);
    }
}

const cxInt cxMultiple::Index() const
{
    return index;
}

const cxInt cxMultiple::Size() const
{
    return actions->Size();
}

cxMultiple *cxMultiple::Append(cxAction *pav,cxView *pview)
{
    CX_ASSERT(pav != nullptr && pview != nullptr, "args error");
    pav->SetView(pview);
    actions->Append(pav);
    return this;
}

CX_CPP_END

