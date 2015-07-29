//
//  cxOpenAL.h
//  cxEngineCore
//
//  Created by xuhua on 7/29/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#ifndef cxEngineCore_cxOpenAL_h
#define cxEngineCore_cxOpenAL_h

#include <core/cxObject.h>
#include <math/cxPoint3F.h>
#include <math/cxPoint2F.h>
#include "OpenAL.h"

CX_CPP_BEGIN

class cxALBuffer : public cxObject
{
public:
    CX_DECLARE(cxALBuffer);
protected:
    explicit cxALBuffer();
    virtual ~cxALBuffer();
private:
    cxUInt format;
    ALuint handle;
    cxUInt samplerate;
    cxUInt numberOfSamples;
    cxUInt bytesPerSample;
    cxFloat duration;
public:
    cxFloat Duration();
    cxBool Init(cchars file);
    ALuint Handle();
    static cxALBuffer *Create(cchars file);
};

class cxALSource : public cxObject
{
public:
    CX_DECLARE(cxALSource);
protected:
    explicit cxALSource();
    virtual ~cxALSource();
private:
    cxALBuffer *buffer;
    ALuint handle;
    cxBool loop;
    cxFloat gain;
    cxFloat pitch;
public:
    cxBool IsPlaying();
    cxBool Init(cxALBuffer *ab);
    void Play(cxBool loop=false);
    void Stop();
    void SetGain(cxFloat v);
    void SetPitch(cxFloat v);
    void SetLoop(cxBool v);
    void SetPosition(const cxPoint3F &v);
    void SetPosition(const cxPoint2F &v);
    cxALBuffer *Buffer();
    virtual void Update(cxFloat dt);
};

class cxOpenAL : public cxObject
{
    friend cxCore;
private:
    CX_DECLARE(cxOpenAL);
protected:
    explicit cxOpenAL();
    virtual ~cxOpenAL();
private:
    ALCdevice *device;
    ALCcontext *context;
    static cxOpenAL *instance;
    cxHash *files;
public:
    static cxOpenAL *Instance();
    cxALSource *Source(cchars key,cchars file);
    cxALSource *Source(cchars key);
};

CX_CPP_END

#endif


