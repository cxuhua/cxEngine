//
//  cxOpenAL.cpp
//  cxEngineCore
//
//  Created by xuhua on 7/29/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//
#include <core/cxHash.h>
#include <core/cxUtil.h>
#define MINIMP3_IMPLEMENTATION
#include <ext/minimp3.h>
#include "cxOpenAL.h"

CX_CPP_BEGIN

#define alClearError() alGetError()

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


class cxMP3Buffer : public cxALBuffer
{
public:
    CX_DECLARE(cxMP3Buffer);
protected:
    explicit cxMP3Buffer();
    virtual ~cxMP3Buffer();
private:
    short pcm[MINIMP3_MAX_SAMPLES_PER_FRAME];
    mp3dec_t mp3d;
    mp3dec_frame_info_t info;
    cxInt mp3position;
    cxStr *mp3data;
    cxBool haspcm;
    cxInt sample;
    cxBool readNextFrame();

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

CX_IMPLEMENT(cxMP3Buffer)

cxMP3Buffer::cxMP3Buffer()
{
    isfinished = false;
    isopen = false;
    mp3data = nullptr;
    mp3position = 0;
    haspcm = false;
}

cxMP3Buffer::~cxMP3Buffer()
{
    cxObject::release(&mp3data);
}

cxBool cxMP3Buffer::readNextFrame()
{
    haspcm = false;
    sample = mp3dec_decode_frame(&mp3d, (const uint8_t *)mp3data->Buffer() + mp3position, mp3data->Size() - mp3position, pcm, &info);
    if(sample == 0){
        isfinished = true;
        return false;
    }
    format = info.channels == 2?AL_FORMAT_STEREO16:AL_FORMAT_MONO16;
    samplerate = info.hz;
    mp3position += info.frame_bytes;
    haspcm = true;
    return haspcm;
}

cxBool cxMP3Buffer::Open()
{
    isopen = readNextFrame();
    return isopen;
}

cxBool cxMP3Buffer::Init(const cxStr *data)
{
    CX_ASSERT(cxStr::IsOK(data), "mp3 data error");
    mp3position = 0;
    cxObject::swap(&mp3data, data);
    mp3dec_init(&mp3d);
    return true;
}

void cxMP3Buffer::Reset()
{
    mp3dec_init(&mp3d);
    mp3position = 0;
    isfinished = false;
    isopen = false;
    sample = 0;
    haspcm = false;
}

void cxMP3Buffer::Close()
{
    mp3position = 0;
    isfinished = true;
    isopen = false;
}

cxBool cxMP3Buffer::IsOpen()
{
    return isopen;
}

cxBool cxMP3Buffer::IsFinished()
{
    return isfinished;
}

cxBool cxMP3Buffer::NextALBuffer(ALuint idx)
{
    if(isfinished){
        return false;
    }
    if(haspcm){
        cxInt pcmsize = info.channels * 2 * sample;
        alBufferData(idx, format, pcm, (ALsizei)pcmsize, samplerate);
    }
    return readNextFrame();
}

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
    cxBool Update(cxFloat dt);
};

CX_IMPLEMENT(cxMP3Source)

cxMP3Source::cxMP3Source()
{
    memset(albuf, 0, sizeof(ALuint) * MAX_BUFFER);
    isstop = true;
    ispause = false;
    alGenBuffers(MAX_BUFFER, albuf);
}

cxMP3Source::~cxMP3Source()
{
    Stop();
    alSourceUnqueueBuffers(handle,MAX_BUFFER,albuf);
    alDeleteBuffers(MAX_BUFFER, albuf);
}

void cxMP3Source::Play()
{
    cxMP3Buffer *mb = Buffer()->To<cxMP3Buffer>();
    if(!mb->IsOpen()){
        mb->Open();
    }
    if(!mb->IsOpen()){
        CX_ERROR("can't open buffer");
        return;
    }
    //alSourceUnqueueBuffers(handle,MAX_BUFFER,albuf);
    cxInt num = 0;
    for(cxInt i=0;i<MAX_BUFFER;i++){
        if(!mb->NextALBuffer(albuf[i])){
            break;
        }
        num++;
        alSourceQueueBuffers(handle, 1, &albuf[i]);
    }
    if(num > 0){
        cxALSource::Play();
        isstop = false;
    }
}

void cxMP3Source::Reset()
{
    cxMP3Buffer *mb = Buffer()->To<cxMP3Buffer>();
    mb->Reset();
    alSourceStop(handle);
    alSourceUnqueueBuffers(handle,MAX_BUFFER,albuf);
    alDeleteBuffers(MAX_BUFFER, albuf);
    memset(albuf, 0, sizeof(ALuint) * MAX_BUFFER);
    isstop = true;
    ispause = false;
    alGenBuffers(MAX_BUFFER, albuf);
}

void cxMP3Source::Stop()
{
    cxMP3Buffer *mb = Buffer()->To<cxMP3Buffer>();
    mb->Close();
    alSourceStop(handle);
    isstop = true;
}

cxBool cxMP3Source::Update(cxFloat dt)
{
    if(isstop){
        return true;
    }
    cxMP3Buffer *mb = Buffer()->To<cxMP3Buffer>();
    ALint processed = 0;
    alGetSourceiv(handle, AL_BUFFERS_PROCESSED, &processed);
    alClearError();
    while(processed > 0){
        alClearError();
        ALuint b = 0;
        alSourceUnqueueBuffers(handle, 1, &b);
        if(alGetError() != AL_NO_ERROR){
            continue;
        }
        if(b == 0){
            continue;
        }
        if(mb->NextALBuffer(b)){
            alSourceQueueBuffers(handle, 1, &b);
        }
        processed--;
    }
    ALint queued = 0;
    alGetSourceiv(handle, AL_BUFFERS_QUEUED, &queued);
    return mb->IsFinished() && queued == 0;
}

CX_IMPLEMENT(cxALBuffer);

cxALBuffer::cxALBuffer()
{
    format = 0;
    handle = 0;
    samplerate = 0;
}

cxALBuffer::~cxALBuffer()
{
    alDeleteBuffers(1, &handle);
}

cxBool cxALBuffer::Init(const cxStr *data)
{
    CX_ASSERT(cxStr::IsOK(data), "bufer init ok");
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
    //find fmt and data
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
    alClearError();
    alGenBuffers(1, &handle);
    if(alGetError() != AL_NO_ERROR){
        CX_ERROR("gen al buffer error");
        return false;
    }
    alClearError();
    alBufferData(handle, format, dataptr, size, samplerate);
    if(alGetError() != AL_NO_ERROR){
        CX_ERROR("al buffer data error");
        return false;
    }
    return true;
}

ALuint cxALBuffer::Handle()
{
    return handle;
}

cxALBuffer::DataType cxALBuffer::Type()
{
    return datatype;
}

cxALBuffer *cxALBuffer::Create(const cxStr *data,DataType type)
{
    CX_ASSERT(cxStr::IsOK(data), "args error");
    cxALBuffer *b = nullptr;
    
    if(type == DataTypeWAV){
        b = cxALBuffer::Create();
    }else if(type == DataTypeMP3){
        b = cxMP3Buffer::Create();
    }
    b->datatype = type;
    if(!b->Init(data)){
        return nullptr;
    }
    return b;
}

CX_IMPLEMENT(cxALSource)

cxALSource::cxALSource()
{
    gain = 1.0f;
    pitch = 1.0f;
    buffer = nullptr;
    handle = 0;
}

cxALSource::~cxALSource()
{
    if(IsPlaying()){
        Stop();
    }
    cxObject::release(&buffer);
    alSourcei(handle, AL_BUFFER, 0);
    alDeleteSources(1, &handle);
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
    ALfloat zeros[] = {0,0,0};
    alListenerfv(AL_POSITION, zeros);
    alListenerfv(AL_VELOCITY, zeros);
    return true;
}

cxBool cxALSource::Update(cxFloat dt)
{
    ALint state;
    alGetSourcei(handle, AL_SOURCE_STATE, &state);
    return state == AL_STOPPED;
}

void cxALSource::SetPosition(const cxPoint3F &v)
{
    alSource3f(handle, AL_POSITION, v.x, v.y, v.z);
}

void cxALSource::SetPosition(const cxPoint2F &v)
{
    alSource3f(handle, AL_POSITION, v.x, v.y, 0.0f);
}

void cxALSource::SetVelocity(const cxPoint3F &v)
{
    alSource3f(handle, AL_VELOCITY, v.x, v.y, v.z);
}

void cxALSource::SetVelocity(const cxPoint2F &v)
{
    alSource3f(handle, AL_VELOCITY, v.x, v.y, 0.0f);
}

cxALBuffer *cxALSource::Buffer()
{
    return buffer;
}

void cxALSource::Play()
{
    if(handle > 0){
        alSourcePlay(handle);
    }
}

void cxALSource::OnPause()
{
    if(handle > 0 && IsPlaying()){
        alSourceStop(handle);
    }
}

void cxALSource::OnResume()
{
    if(handle > 0 && !IsPlaying()){
        alSourcePlay(handle);
    }
}


void cxALSource::Reset()
{
    if(handle > 0 && IsPlaying()){
        Stop();
    }
}

void cxALSource::Stop()
{
    if(handle > 0){
        alSourceStop(handle);
    }
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

cxALSource *cxALSource::Create(const cxStr *data, cxALBuffer::DataType type)
{
    CX_ASSERT(cxStr::IsOK(data), "args error");
    cxALBuffer *b = cxALBuffer::Create(data,type);
    if(b == nullptr){
        return nullptr;
    }
    cxALSource *s = nullptr;
    if(b->Type() == cxALBuffer::DataTypeWAV){
        s = cxALSource::Create();
    }else if(b->Type() == cxALBuffer::DataTypeMP3){
        s = cxMP3Source::Create();
    }else{
        CX_ASSERT(false, "type %d not support",type);
    }
    if(!s->Init(b)){
        return nullptr;
    }
    return s;
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
    sources = cxHash::Alloc();
}

cxOpenAL::~cxOpenAL()
{
    sources->Release();
    alcMakeContextCurrent(nullptr);
    alcDestroyContext(context);
    alcCloseDevice(device);
}

cxALSource *cxOpenAL::Source(cchars key)
{
    cxObject *obj = sources->Get(key);
    if(obj != nullptr){
        return obj->To<cxALSource>();
    }
    return nullptr;
}

cxALSource *cxOpenAL::Source(const cxStr *data,cxALBuffer::DataType type)
{
    return cxALSource::Create(data, type);
}

void cxOpenAL::OnPause()
{
    for(cxHash::Iter it = sources->Begin();it != sources->End();it++){
        cxALSource *source = it->second->To<cxALSource>();
        source->OnPause();
    }
}

void cxOpenAL::OnResume()
{
    for(cxHash::Iter it = sources->Begin();it != sources->End();it++){
        cxALSource *source = it->second->To<cxALSource>();
        source->OnResume();
    }
}

void cxOpenAL::Remove(cchars key)
{
    cxObject *obj = sources->Get(key);
    if(obj == nullptr){
        return;
    }
    obj->To<cxALSource>()->Stop();
    sources->Del(key);
}

cxALSource *cxOpenAL::Source(cchars file,cchars key)
{
    cxALSource *s = Source(key);
    if(s != nullptr){
        return s;
    }
    const cxStr *data = cxUtil::Assets(file);
    if(!cxStr::IsOK(data)){
        CX_ERROR("data get error");
        return nullptr;
    }
    cxALBuffer::DataType type = 0;
    cchars ext = strrchr(file, '.');
    if(cxStr::IsEqu(ext, ".mp3")){
        type = cxALBuffer::DataTypeMP3;
    }else if(cxStr::IsEqu(ext, ".wav")){
        type = cxALBuffer::DataTypeWAV;
    }
    s = Source(data,type);
    if(s == nullptr){
        return nullptr;
    }
    sources->Set(key, s);
    return s;
}

cxOpenAL *cxOpenAL::Instance()
{
    return cxCore::One<cxOpenAL>(&instance);
}

CX_CPP_END


