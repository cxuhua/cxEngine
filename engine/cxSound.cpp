//
//  cxSound.cpp
//  cxEngineCore
//
//  Created by xuhua on 7/29/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#include "cxEngine.h"
#include <core/cxStr.h>
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
    if(source != nullptr){
        source->Play();
    }else{
        CX_ASSERT(Time() > 0, "load time error");
        cxSound::Play();
    }
}

void cxSound::OnExit()
{
    if(source == nullptr){
        cxSound::StopMusic();
    }
}

cxSound *cxSound::Create(cchars file)
{
    cxSound *ret = cxSound::Create();
    cchars ext = strrchr(file, '.');
    if(cxStr::IsCaseEqu(ext, ".mp3")){
        ret->SetID(cxEngineIdMusic);
        ret->SetTime(Load(file));
        return ret;
    }
    cxALSource *source = cxOpenAL::Instance()->Source(file, file);
    if(source == nullptr){
        return ret;
    }
    cxObject::swap(&ret->source, source);
    ret->SetTime(source->Buffer()->Duration());
    return ret;
}

CX_CPP_END

