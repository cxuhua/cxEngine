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
    cxHash *caches;
    uv_key_t autoKey;
    std::vector<cxAny> ones;
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
















