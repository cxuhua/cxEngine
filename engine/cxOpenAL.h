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

#define MAX_BUFFER  32

class cxALBuffer : public cxObject
{
public:
    CX_DECLARE(cxALBuffer);
public:
    typedef cxUInt DataType;
    static const DataType DataTypeNone  = 0;
    static const DataType DataTypeWAV   = 1;
    static const DataType DataTypeMP3   = 2;
protected:
    explicit cxALBuffer();
    virtual ~cxALBuffer();
protected:
    ALuint handle;
    cxUInt format;
    cxUInt samplerate;
    DataType datatype;
public:
    DataType Type();
    virtual cxBool Init(const cxStr *data);
    ALuint Handle();
    static cxALBuffer *Create(const cxStr *data,DataType type);
};

class cxALSource : public cxObject
{
public:
    CX_DECLARE(cxALSource);
protected:
    explicit cxALSource();
    virtual ~cxALSource();
private:
    std::string skey;
protected:
    cxALBuffer *buffer;
    ALuint handle;
    cxFloat gain;
    cxFloat pitch;
public:
    void SetKey(cchars key);
    cchars GetKey();
    cxBool IsPlaying();
    virtual cxBool Init(cxALBuffer *ab);
    virtual void Play();
    virtual void Stop();
    virtual void Reset();
    virtual void OnPause();
    virtual void OnResume();
    void SetGain(cxFloat v);
    void SetPitch(cxFloat v);
    void SetLoop(cxBool v);
    void SetPosition(const cxPoint3F &v);
    void SetPosition(const cxPoint2F &v);
    void SetVelocity(const cxPoint3F &v);
    void SetVelocity(const cxPoint2F &v);
    void SetMaxDistance(cxFloat v);
    cxALBuffer *Buffer();
    virtual cxBool Update(cxFloat dt);
    static cxALSource *Create(const cxStr *data,cxALBuffer::DataType type);
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
    cxHash *sources;
public:
    ALCcontext *GetContext();
    static cxOpenAL *Instance();
    void Remove(cchars key);
    void Remove(cxALSource *source);
    cxALSource *Source(cchars file,cchars key);
    cxALSource *Source(cchars key);
    cxALSource *Source(const cxStr *data,cxALBuffer::DataType type);
    void SetPosition(const cxPoint3F &v);
    void SetPosition(const cxPoint2F &v);
    void SetOrientation(const cxPoint3F &v1,const cxPoint3F &v2);
    void SetVelocity(const cxPoint3F &v);
    void SetVelocity(const cxPoint2F &v);
    void SetDistanceModel(ALenum v);
public:
    void OnPause();
    void OnResume();
};

CX_CPP_END

#endif


