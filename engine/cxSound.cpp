//
//  cxSound.cpp
//  cxEngineCore
//
//  Created by xuhua on 7/29/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#include "cxSound.h"

CX_CPP_BEGIN

CX_IMPLEMENT(cxSound);

cxSound::cxSound()
{
    source = nullptr;
}

cxSound::~cxSound()
{
    cxObject::release(&source);
}

void cxSound::OnInit()
{
    CX_ASSERT(source != nullptr, "source miss");
    source->Play();
}

void cxSound::OnStep(cxFloat dt)
{
    source->Update(dt);
}

cxSound *cxSound::Create(cchars file)
{
    cxSound *ret = cxSound::Create();
    cxALSource *source = cxOpenAL::Instance()->Source(file, file);
    if(source == nullptr){
        return ret;
    }
    cxObject::swap(&ret->source, source);
    ret->SetTime(source->Buffer()->Duration());
    return ret;
}

CX_CPP_END

