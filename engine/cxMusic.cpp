//
//  cxMusic.cpp
//  cxEngineCore
//
//  Created by xuhua on 7/29/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#include "cxEngine.h"
#include <core/cxStr.h>
#include "cxMusic.h"

CX_CPP_BEGIN

CX_IMPLEMENT(cxMusic);

cxMusic::cxMusic()
{
    source = nullptr;
}

cxMusic::~cxMusic()
{
    cxObject::release(&source);
}

void cxMusic::OnInit()
{
    source->Play();
}

void cxMusic::OnExit()
{
    source->Stop();
}

void cxMusic::OnStep(cxFloat dt)
{
    source->Update(dt);
}

cxMusic *cxMusic::Create(cchars file)
{
    cxMusic *ret = cxMusic::Create();
    cxALSource *source = cxOpenAL::Instance()->Source(file, file);
    if(source == nullptr){
        return ret;
    }
    cxObject::swap(&ret->source, source);
    ret->SetTime(source->Buffer()->Duration());
    return ret;
}

CX_CPP_END

