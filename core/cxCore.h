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
public:
    typedef cxObject *(*AllocFunc)();
private:
    static cxCore *gCore;
    struct cxCoreHasher
    {
        size_t operator()(const std::string &k) const;
        bool operator()(const std::string &lhs, const std::string &rhs) const;
    };
    typedef std::unordered_map<std::string,cxHelper&,cxCoreHasher,cxCoreHasher> cxTypes;
    cxTypes classes;
    cxHash *caches;
    uv_key_t autoKey;
    std::vector<cxAny> ones;
public:
    template<class T>
    static T *One(cxAny gv)
    {
        T **ptr = (T **)gv;
        if(*ptr == nullptr){
            *ptr = T::Alloc();
            gCore->ones.push_back(gv);
        }
        return *ptr;
    }
    static cxObject *alloc(cchars name);
    static void registerType(cchars name,cxHelper &helper);
    static cxCore *Instance();
    static void Destroy();
    cxStack *GetAutoPool();
    void SetAutoPool(cxStack *pool);
    void Clear();
    void Remove(cchars key);
    void Push(cchars key,cxObject *pobj);
    cxObject *Pull(cchars key);
};

class cxHelper
{
private:
    std::string name;
    cxCore::AllocFunc func;
public:
    cxObject *Alloc();
    explicit cxHelper(cchars aname,cxCore::AllocFunc f);
    virtual ~cxHelper();
    cchars Name() const;
};

CX_CPP_END

#endif /* defined(__cxEngineCore__cxCore__) */
















