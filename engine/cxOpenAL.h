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
#include <mpg123/mpg123.h>
#include "OpenAL.h"

CX_CPP_BEGIN

#define MAX_BUFFER  32

class cxALBuffer : public cxObject
{
public:
    CX_DECLARE(cxALBuffer);
public:
    typedef cxUInt DataType;
    static const DataType DataTypeWAV = 1;
    static const DataType DataTypeMP3 = 2;
protected:
    explicit cxALBuffer();
    virtual ~cxALBuffer();
protected:
    ALuint handle;
    cxUInt format;
    cxUInt samplerate;
    cxUInt numberOfSamples;
    cxUInt bytesPerSample;
    cxFloat duration;
    DataType datatype;
public:
    DataType Type();
    virtual cxFloat Duration();
    virtual cxBool Init(const cxStr *data);
    ALuint Handle();
    static cxALBuffer *Create(const cxStr *data,DataType type);
};

class cxMP3Buffer : public cxALBuffer
{
public:
    CX_DECLARE(cxMP3Buffer);
protected:
    explicit cxMP3Buffer();
    virtual ~cxMP3Buffer();
private:
    static ssize_t mp3read(void *ud, void *buffer, size_t size);
    cxInt mp3Read(cxAny buf,cxInt size);
    
    static off_t mp3seek(void *ud, off_t seek, int where);
    cxInt mp3Seek(cxInt off,cxInt where);
    
    static void mp3cleanup(void*ud);
    void mp3Clear();
    
    cxInt mp3position;
    mpg123_handle *mp3hand;
    cxStr *mp3data;

    cxBool newformat();
    cxBool isopen;
    cxBool isfinished;
public:
    cxBool IsOpen();
    cxBool Open();
    void Reset();
    void Close();
    cxBool NextALBuffer(ALuint b);
    cxBool Init(const cxStr *data);
    cxBool IsFinished();
};

class cxALSource : public cxObject
{
public:
    CX_DECLARE(cxALSource);
protected:
    explicit cxALSource();
    virtual ~cxALSource();
protected:
    cxALBuffer *buffer;
    ALuint handle;
    cxFloat gain;
    cxFloat pitch;
public:
    cxBool IsPlaying();
    virtual cxBool Init(cxALBuffer *ab);
    virtual void Play();
    virtual void Stop();
    virtual void Reset();
    void SetGain(cxFloat v);
    void SetPitch(cxFloat v);
    void SetPosition(const cxPoint3F &v);
    void SetPosition(const cxPoint2F &v);
    void SetVelocity(const cxPoint3F &v);
    void SetVelocity(const cxPoint2F &v);
    cxALBuffer *Buffer();
    virtual void Update(cxFloat dt);
    static cxALSource *Create(const cxStr *data,cxALBuffer::DataType type);
};

class cxMP3Source : public cxALSource
{
public:
    CX_DECLARE(cxMP3Source);
protected:
    explicit cxMP3Source();
    virtual ~cxMP3Source();
private:
    ALuint albuf[MAX_BUFFER];
    cxBool isstop;
    cxBool ispause;
public:
    void Play();
    void Stop();
    void Reset();
    void Update(cxFloat dt);
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
    static cxOpenAL *Instance();
    cxALSource *Source(cchars file,cchars key);
    cxALSource *Source(cchars key);
    cxALSource *Source(const cxStr *data,cxALBuffer::DataType type);
};

CX_CPP_END

#endif


