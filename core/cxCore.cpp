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

size_t cxCore::cxCoreHasher::operator()(const std::string &k) const
{
    return (size_t)XXH32(k.data(), (int)k.size(), 0);
}

bool cxCore::cxCoreHasher::operator()(const std::string &lhs, const std::string &rhs) const
{
    return lhs == rhs;
}

cxObject *cxHelper::Alloc()
{
    return func();
}

cchars cxHelper::Name() const
{
    return name.data();
}

cxHelper::cxHelper(cchars aname,cxCore::AllocFunc f)
{
    name = std::string(aname);
    func = f;
    cxCore::registerType(aname, *this);
}
cxHelper::~cxHelper()
{
    
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

void cxCore::registerType(cchars name,cxHelper &helper)
{
    CX_ASSERT(cxStr::IsOK(name), "name or func error");
    cxCore::Instance()->classes.emplace(name,helper);
}

cxObject *cxCore::alloc(cchars name)
{
    CX_ASSERT(cxStr::IsOK(name), "name error");
    cxTypes::iterator it = cxCore::Instance()->classes.find(name);
    CX_ASSERT(it != cxCore::Instance()->classes.end(), "class %s not register",name);
    return it->second.Alloc();
}

void cxCore::Destroy()
{
    if(cxCore::gCore != nullptr){
        delete cxCore::gCore;
        cxCore::gCore = nullptr;
    }
}

cxCore::cxCore()
{
    caches = cxHash::Alloc();
    uv_key_create(&autoKey);
}

cxCore::~cxCore()
{
    caches->Release();
    cxObject::release(&cxUtil::instance);
    cxObject *obj = static_cast<cxObject *>(uv_key_get(&autoKey));
    cxObject::release(&obj);
    uv_key_delete(&autoKey);
    classes.clear();
}

void cxCore::Clear()
{
    caches->Clear();
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

void *cxCore::GetAutoPool()
{
    return uv_key_get(&autoKey);
}

void cxCore::SetAutoPool(void *pool)
{
    uv_key_set(&autoKey, pool);
}

CX_CPP_END




