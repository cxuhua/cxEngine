//
//  cxCore.cpp
//  cxEngineCore
//
//  Created by xuhua on 5/23/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#include <ext/xxhash.h>
#include "cxHash.h"
#include "cxAutoPool.h"
#include "cxCore.h"
#include "cxUtil.h"
#include "cxNotice.h"
#include "cxLocalized.h"

CX_CPP_BEGIN

void cxUtilInfo(cchars  file,int line,cchars  format, ...)
{
    va_list ap;
    va_start(ap, format);
    cxUtil::Instance()->Logger("INFO", file, line, format, ap);
    va_end(ap);
}

void cxUtilError(cchars  file,int line,cchars  format, ...)
{
    va_list ap;
    va_start(ap, format);
    cxUtil::Instance()->Logger("ERROR", file, line, format, ap);
    va_end(ap);
}

void cxUtilWarn(cchars  file,int line,cchars  format, ...)
{
    va_list ap;
    va_start(ap, format);
    cxUtil::Instance()->Logger("WARN", file, line, format, ap);
    va_end(ap);
}

void cxUtilAssert(cchars  file,int line,cchars  format, ...)
{
    va_list ap;
    va_start(ap, format);
    cxUtil::Instance()->Logger("ASSERT", file, line, format, ap);
    va_end(ap);
}


cxCore *cxCore::gCore = nullptr;

cxCore *cxCore::Instance()
{
    if(cxCore::gCore == nullptr){
        cxCore::gCore = new cxCore();
        atexit(cxCore::Destroy);
    }
    return cxCore::gCore;
}

void cxCore::Destroy()
{
    if(cxCore::gCore != nullptr){
        delete cxCore::gCore;
        cxCore::gCore = nullptr;
    }
}

cxBool cxCore::IsIOS()
{
    return(CX_TARGET_PLATFORM == CX_PLATFORM_IOS);
}

cxBool cxCore::IsAndroid()
{
    return(CX_TARGET_PLATFORM == CX_PLATFORM_ANDROID);
}

cxBool cxCore::IsMac()
{
    return(CX_TARGET_PLATFORM == CX_PLATFORM_MAC);
}

cxBool cxCore::HasType(cchars name)
{
    cxCore *core = cxCore::Instance();
    TypesMap::iterator it = core->classes.find(name);
    return it != core->classes.end();
}

cxObject *cxCore::Create(cchars name)
{
    return cxCore::Alloc(name)->AutoRelease();
}

cxObject *cxCore::Alloc(cchars name)
{
    cxCore *core = cxCore::Instance();
    TypesMap::iterator it = core->classes.find(name);
    if(it == core->classes.end()){
        return nullptr;
    }
    return it->second();
}

cxLong cxCore::_RegClass_(cchars name,AllocFunc func)
{
    cxCore::Instance()->classes.emplace(name,func);
    return (cxLong)name;
}

cxCore::cxCore()
{
    caches = cxHash::Alloc();
    uv_key_create(&autoKey);
}

cxCore::~cxCore()
{
    Clear();
    caches->Release();
    uv_key_delete(&autoKey);
}

void cxCore::Clear()
{
    //one object
    for(AnyArray::iterator it=ones.begin();it!=ones.end();it++){
        cxObject::release((cxObject **)*it);
    }
    ones.clear();
    //cache clear
    caches->Clear();
    //clean main thread auto release pool
    cxAutoPool::Clear();
}

void cxCore::Push(cchars key,cxObject *pobj)
{
    CX_ASSERT(!caches->Has(key), "key object exists");
    caches->Set(key, pobj);
}

void cxCore::Remove(cchars key)
{
    caches->Del(key);
}

cxObject *cxCore::Pull(cchars key)
{
    return caches->Get(key);
}

cxStack *cxCore::GetAutoPool()
{
    return (cxStack *)uv_key_get(&autoKey);
}

void cxCore::SetAutoPool(cxStack *pool)
{
    uv_key_set(&autoKey, pool);
}

CX_CPP_END




