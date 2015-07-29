//
//  cxOpenAL.cpp
//  cxEngineCore
//
//  Created by xuhua on 7/29/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//
#include <core/cxHash.h>
#include <core/cxUtil.h>
#include "cxOpenAL.h"

CX_CPP_BEGIN

/// WAV File-header
struct WAVFileHdr
{
    ALubyte  id[4];
    ALsizei  size;
    ALubyte  type[4];
    cxBool IsWave()
    {
        return id[0]=='R'&&id[1]=='I'&&id[2]=='F'&&id[3]=='F';
    }
};
//// WAV Fmt-header
struct WAVFmtHdr
{
    ALushort format;
    ALushort channels;
    ALuint   samplesPerSec;
    ALuint   bytesPerSec;
    ALushort blockAlign;
    ALushort bitsPerSample;
};
/// WAV FmtEx-header
struct WAVFmtExHdr
{
    ALushort size;
    ALushort samplesPerBlock;
};
/// WAV Chunk-header
struct WAVChunkHdr
{
    ALubyte  id[4];
    ALuint   size;
    cxBool IsFmt()
    {
        return id[0]=='f'&&id[1]=='m'&&id[2]=='t'&&id[3]==0x20;
    }
    cxBool IsData()
    {
        return id[0]=='d'&&id[1]=='a'&&id[2]=='t'&&id[3]=='a';
    }
};


#define alClearError() alGetError()

CX_IMPLEMENT(cxALBuffer);

cxALBuffer::cxALBuffer()
{
    format = 0;
    handle[0] = 0;
    samplerate = 0;
    numberOfSamples = 0;
    bytesPerSample = 0;
    duration = 0;
}

cxALBuffer::~cxALBuffer()
{
    alDeleteBuffers(1, &handle[0]);
}

cxBool cxALBuffer::Init(cchars file)
{
    const cxStr *data = cxUtil::Assets(file);
    if(!cxStr::IsOK(data)) {
        return false;
    }
    //file hdr
    cchars begin = data->Buffer();
    cchars end = begin + data->Size();
    
    WAVFileHdr fhdr;
    memcpy(&fhdr, begin, sizeof(WAVFileHdr));
    begin += sizeof(WAVFileHdr);
    if(fhdr.size > data->Size()){
        CX_ERROR("wav format error");
        return false;
    }
    if(!fhdr.IsWave()){
        CX_ERROR("wav format error");
        return false;
    }
    WAVFmtHdr fmt;
    cchars dataptr = nullptr;
    cxInt size = 0;
    //chunk hdt
    while(begin < end){
        WAVChunkHdr chdr;
        memcpy(&chdr, begin, sizeof(WAVChunkHdr));
        begin += sizeof(WAVChunkHdr);
        if(chdr.IsFmt()){
            memcpy(&fmt, begin, sizeof(WAVFmtHdr));
        }else if(chdr.IsData()){
            dataptr = begin;
            size = chdr.size;
        }
        begin += chdr.size;
    }
    if(dataptr == nullptr || size == 0){
        CX_ERROR("wave data miss");
        return false;
    }
    if(fmt.channels == 1){
        format = fmt.bitsPerSample==8?AL_FORMAT_MONO8:AL_FORMAT_MONO16;
    }else{
        format = fmt.bitsPerSample==8?AL_FORMAT_STEREO8:AL_FORMAT_STEREO16;
    }
    samplerate = fmt.samplesPerSec;
    numberOfSamples = size/(fmt.bitsPerSample/8);
    duration = (cxFloat)numberOfSamples/(cxFloat)samplerate;
    alClearError();
    alGenBuffers(1, &handle[0]);
    if(alGetError() != AL_NO_ERROR){
        CX_ERROR("gen al buffer error");
        return false;
    }
    alBufferData(handle[0], format, dataptr, size, samplerate);
    if(alGetError() != AL_NO_ERROR){
        CX_ERROR("al buffer data error");
        return false;
    }
    return true;
}

cxFloat cxALBuffer::Duration()
{
    return duration;
}

ALuint cxALBuffer::Handle(cxInt idx)
{
    return handle[idx];
}

cxALBuffer *cxALBuffer::Create(cchars file)
{
    cxALBuffer *b = cxALBuffer::Create();
    if(!b->Init(file)){
        return nullptr;
    }
    return b;
}

CX_IMPLEMENT(cxALSource)

cxALSource::cxALSource()
{
    gain = 1.0f;
    pitch = 1.0f;
    loop = false;
    buffer = nullptr;
}

cxALSource::~cxALSource()
{
    if(IsPlaying()){
        Stop();
    }
    alSourcei(handle, AL_BUFFER, 0);
    alDeleteSources(1, &handle);
    cxObject::release(&buffer);
}

cxBool cxALSource::IsPlaying()
{
    ALint state;
    alGetSourcei(handle, AL_SOURCE_STATE, &state);
    return (state == AL_PLAYING);
}

cxBool cxALSource::Init(cxALBuffer *ab)
{
    cxObject::swap(&buffer, ab);
    alClearError();
    alGenSources(1, &handle);
    if(alGetError() != AL_NO_ERROR){
        CX_ERROR("al gen source error");
        return false;
    }
    alSourcei(handle, AL_BUFFER, ab->Handle());
    if(alGetError() != AL_NO_ERROR){
        CX_ERROR("al source set buffer error");
        return false;
    }
    return false;
}

void cxALSource::Update(cxFloat dt)
{
    
}

void cxALSource::SetPosition(const cxPoint3F &v)
{
    alSourcei(handle, AL_SOURCE_RELATIVE, AL_FALSE);
    alSource3f(handle, AL_POSITION, v.x, v.y, v.z);
}

void cxALSource::SetPosition(const cxPoint2F &v)
{
    alSourcei(handle, AL_SOURCE_RELATIVE, AL_TRUE);
    alSource3f(handle, AL_POSITION, v.x, v.y, 1.0f);
}

cxALBuffer *cxALSource::Buffer()
{
    return buffer;
}

void cxALSource::Play(cxBool loop)
{
    if(IsPlaying()){
        Stop();
    }
    SetLoop(loop);
    alSourcePlay(handle);
}

void cxALSource::Stop()
{
    alSourceStop(handle);
}

void cxALSource::SetGain(cxFloat v)
{
    alSourcef(handle, AL_GAIN,v);
    gain = v;
}

void cxALSource::SetPitch(cxFloat v)
{
    alSourcef(handle, AL_PITCH, v);
    pitch = v;
}

void cxALSource::SetLoop(cxBool v)
{
    alSourcei(handle, AL_LOOPING, v);
    loop = v;
}

CX_IMPLEMENT(cxOpenAL);

cxOpenAL *cxOpenAL::instance = nullptr;

cxOpenAL::cxOpenAL()
{
    device = alcOpenDevice(nullptr);
    CX_ASSERT(device != nullptr, "alc open device error");
    context = alcCreateContext(device, nullptr);
    CX_ASSERT(context != nullptr, "alc create context error");
    alcMakeContextCurrent(context);
    files = cxHash::Alloc();
}

cxOpenAL::~cxOpenAL()
{
    files->Release();
    alcMakeContextCurrent(nullptr);
    alcCloseDevice(device);
}

cxALSource *cxOpenAL::Source(cchars key)
{
    cxObject *obj = files->Get(key);
    if(obj != nullptr){
        return obj->To<cxALSource>();
    }
    return nullptr;
}

cxALSource *cxOpenAL::Source(cchars key,cchars file)
{
    cxALSource *s = Source(key);
    if(s != nullptr){
        return s;
    }
    cxALBuffer *b = cxALBuffer::Create(file);
    if(b == nullptr){
        return nullptr;
    }
    s = cxALSource::Create();
    if(s->Init(b)){
        return nullptr;
    }
    files->Set(key, s);
    return s;
}

cxOpenAL *cxOpenAL::Instance()
{
    return cxCore::One<cxOpenAL>(&instance);
}

CX_CPP_END

