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

CX_IMPLEMENT(cxMP3Buffer)

cxMP3Buffer::cxMP3Buffer()
{
    isopen = false;
    duration = INT_MAX;
    mp3data = nullptr;
    mp3position = 0;
}

cxMP3Buffer::~cxMP3Buffer()
{
    if(mp3hand != nullptr){
        Close();
        mpg123_delete(mp3hand);
        mp3hand = nullptr;
    }
    cxObject::release(&mp3data);
}

ssize_t cxMP3Buffer::mp3read(void *ud, void *buffer, size_t size)
{
    cxMP3Buffer *ab = static_cast<cxMP3Buffer *>(ud);
    return (ssize_t)ab->mp3Read(buffer, (cxInt)size);;
}

off_t cxMP3Buffer::mp3seek(void *ud, off_t seek, int where)
{
    cxMP3Buffer *ab = static_cast<cxMP3Buffer *>(ud);
    return (off_t)ab->mp3Seek((cxInt)seek, where);
}

void cxMP3Buffer::mp3cleanup(void*ud)
{
    cxMP3Buffer *ab = static_cast<cxMP3Buffer *>(ud);
    ab->mp3Clear();
}

cxInt cxMP3Buffer::mp3Read(cxAny buf,cxInt size)
{
    cxInt bytes = size;
    cxInt remain = mp3data->Size() - mp3position;
    if(remain <= 0){
        return 0;
    }
    if(remain < size){
        bytes = remain;
    }
    memcpy(buf, mp3data->Buffer() + mp3position, bytes);
    mp3position += bytes;
    return size;
}

cxInt cxMP3Buffer::mp3Seek(cxInt off,cxInt where)
{
    if(where == SEEK_CUR){
        mp3position += off;
    }else if(where == SEEK_END){
        CX_ASSERT(off <= 0, "off error");
        mp3position = mp3data->Size() + off;
    }else if(where == SEEK_SET){
        mp3position = off;
    }
    return mp3position;
}

void cxMP3Buffer::mp3Clear()
{
    mp3position = 0;
}

cxBool cxMP3Buffer::newformat()
{
    long rate;
    int channgles;
    int encoding;
    if(mpg123_getformat(mp3hand, &rate, &channgles, &encoding) != MPG123_OK){
        CX_ERROR("mpg123 get format error");
        return false;
    }
    if(!((encoding & MPG123_ENC_16) && (encoding && MPG123_ENC_8))){
        CX_ERROR("support 8 or 16 bit");
        return false;
    }
    cxBool mb8 = (encoding & MPG123_ENC_8);
    if(channgles == MPG123_MONO){
        format = mb8?AL_FORMAT_MONO8:AL_FORMAT_MONO16;
    }else if(channgles == MPG123_STEREO){
        format = mb8?AL_FORMAT_STEREO8:AL_FORMAT_STEREO16;
    }
    samplerate = (cxUInt)rate;
    off_t len = mpg123_length(mp3hand);
    int spf = mpg123_spf(mp3hand);
    duration = (len/spf) * mpg123_tpf(mp3hand);
    return true;
}

cxBool cxMP3Buffer::Open()
{
    mp3position = 0;
    
    mp3hand = mpg123_new(nullptr, nullptr);
    CX_ASSERT(mp3hand != nullptr, "mpg123 new error");
    mpg123_replace_reader_handle(mp3hand, mp3read, mp3seek, mp3cleanup);
    
    isopen =(mpg123_open_handle(mp3hand, this) == MPG123_OK);
    if(!IsOpen()){
        return false;
    }
    size_t bytes = 0;
    unsigned char *buf = nullptr;
    off_t framenum = 0;
    int ret = mpg123_decode_frame(mp3hand, &framenum, (unsigned char **)&buf, (size_t *)&bytes);
    if(ret == MPG123_NEW_FORMAT){
        newformat();
        return true;
    }
    CX_ERROR("mp3 buffer open error");
    return false;
}

cxBool cxMP3Buffer::Init(cchars file)
{
    const cxStr *data = cxUtil::Assets(file);
    if(!cxStr::IsOK(data)) {
        return false;
    }
    cxObject::swap(&mp3data, data);
    return Open();
}

void cxMP3Buffer::Reset()
{
    mpg123_seek_frame(mp3hand, 0, SEEK_SET);
}

void cxMP3Buffer::Close()
{
    mpg123_close(mp3hand);
    isopen = false;
}

cxBool cxMP3Buffer::IsOpen()
{
    return isopen;
}

cxBool cxMP3Buffer::NextALBuffer(ALuint idx)
{
    size_t bytes = 0;
    unsigned char *buf = nullptr;
    off_t framenum = 0;
    int ret = mpg123_decode_frame(mp3hand, &framenum, (unsigned char **)&buf, (size_t *)&bytes);
    if(ret == MPG123_NEW_FORMAT){
        newformat();
        ret = mpg123_decode_frame(mp3hand, &framenum, (unsigned char **)&buf, (size_t *)&bytes);
    }
    if(ret != MPG123_OK || bytes == 0){
        return false;
    }
    alClearError();
    alBufferData(idx, format, buf, (ALsizei)bytes, samplerate);
    if(alGetError() != AL_NO_ERROR){
        CX_ERROR("al buffer data error");
        return false;
    }
    return true;
}

CX_IMPLEMENT(cxMP3Source)

cxMP3Source::cxMP3Source()
{
    memset(albuf, 0, sizeof(ALuint) * MAX_BUFFER);
    isstop = true;
    ispause = false;
}

cxMP3Source::~cxMP3Source()
{
    Stop();
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
    cxInt num = 0;
    for(cxInt i=0; i<MAX_BUFFER;i++){
        if(!mb->NextALBuffer(albuf[i])){
            break;
        }
        num++;
        alSourceQueueBuffers(handle, 1, &albuf[i]);
    }
    if(num > 0){
        alSourcePlay(handle);
        isstop = false;
    }
}

void cxMP3Source::Stop()
{
    cxMP3Buffer *mb = Buffer()->To<cxMP3Buffer>();
    mb->Close();
    alSourceStop(handle);
    isstop = true;
}

cxBool cxMP3Source::Init(cxALBuffer *ab)
{
    cxObject::swap(&buffer, ab);
    alClearError();
    alGenSources(1, &handle);
    if(alGetError() != AL_NO_ERROR){
        CX_ERROR("al gen source error");
        return false;
    }
    alClearError();
    alGenBuffers(MAX_BUFFER, albuf);
    if(alGetError() != AL_NO_ERROR){
        CX_ERROR("al gen buffer error");
        return false;
    }
    return true;
}

void cxMP3Source::Update(cxFloat dt)
{
    if(isstop){
        return;
    }
    cxMP3Buffer *mb = Buffer()->To<cxMP3Buffer>();
    ALint processed = 0;
    alGetSourceiv(handle, AL_BUFFERS_PROCESSED, &processed);
    while(processed > 0){
        ALuint b;
        alSourceUnqueueBuffers(handle, 1, &b);
        if(mb->NextALBuffer(b)){
            alSourceQueueBuffers(handle, 1, &b);
        }
        processed--;
    }
    ALint queued = 0;
    alGetSourceiv(handle, AL_BUFFERS_QUEUED, &queued);
    if(queued == 0){
        mb->Close();
    }
}

CX_IMPLEMENT(cxALBuffer);

cxALBuffer::cxALBuffer()
{
    format = 0;
    handle = 0;
    samplerate = 0;
    numberOfSamples = 0;
    bytesPerSample = 0;
    duration = 0;
}

cxALBuffer::~cxALBuffer()
{
    alDeleteBuffers(1, &handle);
}

cxBool cxALBuffer::Init(cchars file)
{
    const cxStr *data = cxUtil::Assets(file);
    if(!cxStr::IsOK(data)) {
        return false;
    }
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
    bytesPerSample = fmt.bitsPerSample/8;
    if(fmt.channels == 1){
        format = fmt.bitsPerSample==8?AL_FORMAT_MONO8:AL_FORMAT_MONO16;
    }else{
        format = fmt.bitsPerSample==8?AL_FORMAT_STEREO8:AL_FORMAT_STEREO16;
    }
    samplerate = fmt.samplesPerSec;
    numberOfSamples = size/bytesPerSample;
    duration = (cxFloat)numberOfSamples/(cxFloat)samplerate;
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

cxFloat cxALBuffer::Duration()
{
    return duration;
}

ALuint cxALBuffer::Handle()
{
    return handle;
}

cxALBuffer::DataType cxALBuffer::Type()
{
    return datatype;
}

cxALBuffer *cxALBuffer::Create(cchars file)
{
    CX_ASSERT(cxStr::IsOK(file), "args error");
    cxALBuffer *b = nullptr;
    cchars ext = strrchr(file, '.');
    if(!cxStr::IsOK(ext)){
        CX_ERROR("file %s name error",file);
        return nullptr;
    }
    if(cxStr::IsCaseEqu(ext, ".wav")){
        b = cxALBuffer::Create();
        b->datatype = DataTypeWAV;
    }else if(cxStr::IsCaseEqu(ext, ".mp3")){
        b = cxMP3Buffer::Create();
        b->datatype = DataTypeMP3;
    }
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
    return true;
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

void cxALSource::Play()
{
    if(IsPlaying()){
        Stop();
    }
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

cxALSource *cxALSource::Create(cchars file)
{
    CX_ASSERT(cxStr::IsOK(file), "args error");
    cxALBuffer *b = cxALBuffer::Create(file);
    if(b == nullptr){
        return nullptr;
    }
    cxALSource *s = nullptr;
    cchars ext = strrchr(file, '.');
    if(!cxStr::IsOK(ext)){
        return nullptr;
    }
    if(b->Type() == cxALBuffer::DataTypeWAV){
        s = cxALSource::Create();
    }else if(b->Type() == cxALBuffer::DataTypeMP3){
        s = cxMP3Source::Create();
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
    if(mpg123_init() != MPG123_OK){
        CX_ERROR("mpg123 Init failed");
    }
}

cxOpenAL::~cxOpenAL()
{
    mpg123_exit();
    sources->Release();
    alcMakeContextCurrent(nullptr);
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

cxALSource *cxOpenAL::Source(cchars key,cchars file)
{
    cxALSource *s = Source(key);
    if(s != nullptr){
        return s;
    }
    s = cxALSource::Create(file);
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

