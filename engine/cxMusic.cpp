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
    CX_ASSERT(source != nullptr, "source not set");
    source->Play();
    cxAction::OnInit();
}

void cxMusic::OnReset()
{
    source->Reset();
    cxAction::OnReset();
}

void cxMusic::OnExit()
{
    source->Stop();
    cxAction::OnExit();
}

void cxMusic::OnStep(cxFloat dt)
{
    source->Update(dt);
    cxAction::OnStep(dt);
}

cxMusic *cxMusic::Create(const cxStr *data,cxALBuffer::DataType type)
{
    cxMusic *ret = cxMusic::Create();
    cxALSource *source = cxOpenAL::Instance()->Source(data, type);
    if(source == nullptr){
        return ret;
    }
    cxObject::swap(&ret->source, source);
    ret->SetTime(source->Buffer()->Duration());
    return ret;
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

