//
//  cxCore.cpp
//  cxEngineCore
//
//  Created by xuhua on 5/23/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

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




