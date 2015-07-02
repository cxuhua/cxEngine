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
#elif CX_TARGET_PLATFORM == CX_PLATFORM_ANDROID
#include <android/cxAndroidUtil.h>
#endif

CX_CPP_BEGIN

CX_IMPLEMENT(cxUtil);

cxUtil *cxUtil::instance = nullptr;

cxUtil::cxUtil()
{
    
}

cxUtil::~cxUtil()
{
    
}

const cxFloat cxUtil::ScaleFactor() const
{
    return 1.0f;
}

#if CX_TARGET_PLATFORM == CX_PLATFORM_IOS
cxUtil *cxUtil::Instance()
{
    if(instance == nullptr){
        instance = cxIOSUtil::Alloc();
    }
    return instance;
}
#elif CX_TARGET_PLATFORM == CX_PLATFORM_ANDROID
cxUtil *cxUtil::Instance()
{
    if(instance == nullptr){
        instance = cxAndroidUtil::Alloc();
    }
    return instance;
}
#endif

const cxStr *cxUtil::Assets(cchars file)
{
    CX_ASSERT(cxStr::IsOK(file), "args error");
    return Instance()->AssetsData(cxStr::UTF8(file));
}

const cxStr *cxUtil::Document(cchars file)
{
    CX_ASSERT(cxStr::IsOK(file), "args error");
    return Instance()->DocumentData(cxStr::UTF8(file));
}

const cxStr *cxUtil::Content(cchars file)
{
    CX_ASSERT(cxStr::IsOK(file), "args error");
    const cxStr *data = Document(file);
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
    free(buffer);
}

cxDouble cxUtil::Timestamp()
{
    struct timeval val = {0};
    gettimeofday(&val, NULL);
    return val.tv_sec + (cxDouble)val.tv_usec/(cxDouble)1000000.0;
}

void cxUtil::SetRandSeed()
{
    srandom(time(nullptr));
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

const cxStr *cxUtil::ReadFileData(const cxStr *path)
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

cxBool cxUtil::FileExists(const cxStr *path,cxInt *length) const
{
    CX_ASSERT(cxStr::IsOK(path), "path error");
    struct stat stat={0};
    cxBool rv = (lstat(path->Data(), &stat) == 0);
    if(rv && length != nullptr){
        *length = (int)stat.st_size;
    }
    return rv;
}

const cxStr *cxUtil::AssetsPath(const cxStr *file)
{
    return file;
}

const cxStr *cxUtil::AssetsData(const cxStr *file)
{
    CX_ASSERT(cxStr::IsOK(file), "path error");
    const cxStr *path = AssetsPath(file);
    if(!cxStr::IsOK(path)){
        return nullptr;
    }
    return ReadFileData(path);
}

const cxStr *cxUtil::DocumentPath(const cxStr *file)
{
    return file;
}

cxBool cxUtil::DeleteDocument(const cxStr *file)
{
    CX_ASSERT(cxStr::IsOK(file), "path or data error");
    const cxStr *path = DocumentPath(file);
    return remove(path->Data()) == 0;
}

cxBool cxUtil::WriteDocument(const cxStr *file,const cxStr *data,cxBool replace)
{
    CX_ASSERT(cxStr::IsOK(file) && cxStr::IsOK(data), "path or data error");
    const cxStr *path = DocumentPath(file);
    if(!cxStr::IsOK(path)){
        return false;
    }
    return WriteFileData(path, data, replace);
}

const cxStr *cxUtil::DocumentData(const cxStr *file)
{
    CX_ASSERT(cxStr::IsOK(file), "path error");
    const cxStr *path = DocumentPath(file);
    if(!cxStr::IsOK(path)){
        return nullptr;
    }
    return ReadFileData(path);
}
CX_CPP_END

