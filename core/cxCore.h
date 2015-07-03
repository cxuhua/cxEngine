//
//  cxCore.h
//  cxEngineCore
//
//  Created by xuhua on 5/23/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#ifndef __cxEngineCore__cxCore__
#define __cxEngineCore__cxCore__

#include "cxDefine.h"

CX_CPP_BEGIN

class cxHash;
class cxObject;
class cxStr;
class cxJson;
class cxCore
{
protected:
    explicit cxCore();
    virtual ~cxCore();
private:
    cxHash *caches;
    static cxCore *gCore;
    uv_key_t autoKey;
public:
    //
    static cxCore *Instance();
    static void Destroy();
    //
    void *GetAutoPool();
    void SetAutoPool(void *pool);
    //cache opt
    void Clear();
    void Remove(cchars key);
    void Push(cchars key,cxObject *pobj);
    cxObject *Pull(cchars key);
};

CX_CPP_END

#endif /* defined(__cxEngineCore__cxCore__) */
















