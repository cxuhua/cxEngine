//
//  cxCore.h
//  cxEngineCore
//
//  Created by xuhua on 5/23/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#ifndef __cxEngineCore__cxCore__
#define __cxEngineCore__cxCore__

#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include "cxDefine.h"

CX_CPP_BEGIN

class cxHash;
class cxObject;
class cxStr;
class cxJson;
class cxHelper;
class cxArray;
class cxStack;

class cxCore
{
protected:
    explicit cxCore();
    virtual ~cxCore();
private:
    typedef cxObject *(*AllocFunc)();
    typedef std::map<std::string, AllocFunc> TypesMap;
    static cxCore *gCore;
    cxHash *caches;
    uv_key_t autoKey;
    std::vector<cxAny> ones;
    TypesMap classes;
public:
    static cxObject *Create(cchars name);
    static cxObject *Alloc(cchars name);
public:
    template<class T>
    static T *One(cxAny gv);
    static cxCore *Instance();
    static void Destroy();
    cxStack *GetAutoPool();
    void SetAutoPool(cxStack *pool);
    void Clear();
    void Remove(cchars key);
    void Push(cchars key,cxObject *pobj);
    cxObject *Pull(cchars key);
public:
    static cxLong _RegClass_(cchars name,AllocFunc func);
};

template<class T>
T *cxCore::One(cxAny gv)
{
    T **ptr = (T **)gv;
    if(*ptr == nullptr){
        *ptr = T::Alloc();
        cxCore::Instance()->ones.push_back(gv);
    }
    return *ptr;
}
CX_CPP_END

#endif /* defined(__cxEngineCore__cxCore__) */
















