//
//  cxUtil.cpp
//  cxEngineCore
//
//  Created by xuhua on 5/30/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#include <sys/stat.h>
#include <sys/time.h>
#include <unistd.h>
#include <fstream>
#include "cxUtil.h"

#if CX_TARGET_PLATFORM == CX_PLATFORM_IOS
#include <ios/cxIOSUtil.h>
cxengine::cxUtil *cxengine::cxUtil::Instance()
{
    return cxCore::One<cxIOSUtil>(&instance);
}
#elif CX_TARGET_PLATFORM == CX_PLATFORM_ANDROID
#include <android/cxAndroidUtil.h>
cxengine::cxUtil *cxengine::cxUtil::Instance()
{
    return cxCore::One<cxAndroidUtil>(&instance);
}
#elif CX_TARGET_PLATFORM == CX_PLATFORM_MAC
#include <mac/cxMACUtil.h>
cxengine::cxUtil *cxengine::cxUtil::Instance()
{
    return cxCore::One<cxMACUtil>(&instance);
}
#else
cxengine::cxUtil *cxengine::cxUtil::Instance()
{
    return cxCore::One<cxUtil>(&instance);
}
#endif


CX_CPP_BEGIN

CX_IMPLEMENT(cxUtil);

cxUtil *cxUtil::instance = nullptr;

cxUtil::cxUtil()
{
    SetRandSeed();
}

cxUtil::~cxUtil()
{
    
}

cxFloat cxUtil::ScaleFactor()
{
    return 1.0f;
}

cxStr *cxUtil::GetLang()
{
    return cxStr::UTF8("zh");
}

cxStr *cxUtil::GetCountry()
{
    return cxStr::UTF8("CN");
}

cxStr *cxUtil::LocalizedKey()
{
    return GetCountry();
}

cxStr *cxUtil::Assets(cchars file)
{
    CX_ASSERT(cxStr::IsOK(file), "args error");
    return Instance()->AssetsData(file);
}

cxStr *cxUtil::Document(cchars file)
{
    CX_ASSERT(cxStr::IsOK(file), "args error");
    return Instance()->DocumentData(file);
}

cxStr *cxUtil::Content(cchars file)
{
    CX_ASSERT(cxStr::IsOK(file), "args error");
    cxStr *data = Document(file);
    if(cxStr::IsOK(data)){
        return data;
    }
    return Assets(file);
}

void cxUtil::Logger(const char* type,const char*file,int line,const char* format,va_list ap)
{
    char *buffer = nullptr;
    vasprintf(&buffer, format, ap);
    printf("[%s:%d] %s:%s\n",file,line,type,buffer);
    fflush(stdout);
    free(buffer);
}

cxDouble cxUtil::Timestamp()
{
    struct timeval val = {0};
    gettimeofday(&val, NULL);
    return val.tv_sec + (cxDouble)val.tv_usec/(cxDouble)1000000.0;
}

cxUInt64 cxUtil::Now()
{
    struct timeval val = {0};
    gettimeofday(&val, NULL);
    return val.tv_sec * 1000000L + val.tv_usec;
}

void cxUtil::SetRandSeed()
{
    srandom((unsigned int)time(nullptr));
}

cxFloat cxUtil::Rand()
{
    return ((2.0f*((cxFloat)(rand()%RAND_MAX)/(cxFloat)RAND_MAX))-1.0f);
}

cxInt cxUtil::Rand(cxInt min,cxInt max)
{
    cxInt x = rand();
    return (min + x % (max + 1 - min));
}

cxBool cxUtil::WriteFileData(const cxStr *path,const cxStr *data,cxBool replace)
{
    CX_ASSERT(cxStr::IsOK(path) && cxStr::IsOK(data), "path or data error");
    int length = 0;
    if(FileExists(path, &length) && !replace){
        CX_WARN("file exists not write data");
        return false;
    }
    std::ofstream fs;
    fs.open(path->Data(),std::ios::out|std::ios::binary);
    if(!fs.is_open()){
        return false;
    }
    fs.write(data->Data(), data->Size());
    fs.close();
    return true;
}

cxStr *cxUtil::ReadFileData(const cxStr *path)
{
    CX_ASSERT(cxStr::IsOK(path), "path error");
    int length = 0;
    if(!FileExists(path, &length)){
        return nullptr;
    }
    std::ifstream fs;
    fs.open(path->Data(),std::ios::in|std::ios::binary);
    if(!fs.is_open()){
        return nullptr;
    }
    cxStr *ret = cxStr::Create()->Init(length, 0);
    chars buffer = ret->Buffer();
    fs.read(buffer, length);
    fs.close();
    return ret;
}

cxBool cxUtil::FileExists(const cxStr *path,cxInt *length)
{
    CX_ASSERT(cxStr::IsOK(path), "path error");
    struct stat stat={0};
    cxBool rv = (lstat(path->Data(), &stat) == 0);
    if(rv && length != nullptr){
        *length = (int)stat.st_size;
    }
    return rv;
}

cxStr *cxUtil::UUID()
{
    char buf[64]={0};
    snprintf(buf, 64, "%llu", uv_hrtime());
    return cxStr::Create(buf);
}

cxStr *cxUtil::AssetsPath(cchars file)
{
    return cxStr::UTF8(file);
}

cxStr *cxUtil::AssetsData(cchars file)
{
    CX_ASSERT(cxStr::IsOK(file), "path error");
    const cxStr *path = AssetsPath(file);
    if(!cxStr::IsOK(path)){
        return nullptr;
    }
    return ReadFileData(path);
}

cxStr *cxUtil::DocumentPath(cchars file)
{
    return cxStr::UTF8(file);
}

cxBool cxUtil::DeleteDocument(cchars file)
{
    CX_ASSERT(cxStr::IsOK(file), "path or data error");
    const cxStr *path = DocumentPath(file);
    return remove(path->Data()) == 0;
}

cxBool cxUtil::WriteDocument(cchars file,const cxStr *data,cxBool replace)
{
    CX_ASSERT(cxStr::IsOK(file) && cxStr::IsOK(data), "path or data error");
    const cxStr *path = DocumentPath(file);
    if(!cxStr::IsOK(path)){
        return false;
    }
    return WriteFileData(path, data, replace);
}

cxStr *cxUtil::DocumentData(cchars file)
{
    CX_ASSERT(cxStr::IsOK(file), "path error");
    const cxStr *path = DocumentPath(file);
    if(!cxStr::IsOK(path)){
        return nullptr;
    }
    return ReadFileData(path);
}
CX_CPP_END

