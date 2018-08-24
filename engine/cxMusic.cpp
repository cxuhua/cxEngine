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
    isfollow = false;
    source = nullptr;
    Forever();
}

cxMusic::~cxMusic()
{
    cxObject::release(&source);
}

cxMusic *cxMusic::BindFollow(cxView *pview)
{
    isfollow = true;
    Bind(pview);
    return this;
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

void cxMusic::updateForFollowView()
{
    cxView *pv = FirstBindes()->To<cxView>();
    if(pv == nullptr){
        return;
    }
    onFollow.Fire(this, pv);
}

void cxMusic::OnStep(cxFloat dt)
{
    if(isfollow){
        updateForFollowView();
    }
    if(source->Update(dt)){
        Expire();
    }
    cxAction::OnStep(dt);
}

cxALSource *cxMusic::GetSource()
{
    return source;
}

cxBool cxMusic::Init(cchars file,cchars key)
{
    if(key == nullptr){
        key = file;
    }
    cxALSource *src = cxOpenAL::Instance()->Source(file, key);
    if(src == nullptr){
        CX_ERROR("create music action error for file=%s",file);
        return false;
    }
    cxObject::swap(&source, src);
    return true;
}

cxBool cxMusic::Init(const cxStr *data,cxALBuffer::DataType type)
{
    cxALSource *src = cxOpenAL::Instance()->Source(data, type);
    if(src == nullptr){
        CX_ERROR("create music action error for data len=%d",data->Size());
        return false;
    }
    cxObject::swap(&source, src);
    return true;
}

cxMusic *cxMusic::Create(const cxStr *data,cxALBuffer::DataType type)
{
    cxMusic *ret = cxMusic::Create();
    ret->Init(data, type);
    return ret;
}

cxMusic *cxMusic::Create(cchars file,cchars key)
{
    cxMusic *ret = cxMusic::Create();
    ret->Init(file,key);
    return ret;
}

CX_CPP_END

