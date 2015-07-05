//
//  cxObject.h
//  cxEngineCore
//
//  Created by xuhua on 5/24/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#ifndef __cxEngineCore__cxObject__
#define __cxEngineCore__cxObject__

#include <set>
#include <string>
#include <math.h>
#include <float.h>
#include "cxCore.h"
#include "cxEvent.h"

CX_CPP_BEGIN

void cxUtilInfo(cchars file,int line,cchars format, ...) CX_ATTR_FORMAT(3,4);

void cxUtilError(cchars file,int line,cchars format, ...) CX_ATTR_FORMAT(3,4);

void cxUtilWarn(cchars file,int line,cchars format, ...) CX_ATTR_FORMAT(3,4);

void cxUtilAssert(cchars file,int line,cchars format, ...) CX_ATTR_FORMAT(3,4);

class cxStr;
class cxJson;
class cxObject
{
public:
    CX_DECLARE(cxObject);
protected:
    explicit cxObject();
    virtual ~cxObject();
private:
    cxInt refcount;
    cxLong tag;
public:
    //swap object,src release,dst retain
    template<class T1,class T2>
    static void swap(T1 **ptr,T2 *dst);
    
    //release object if not null,and set null
    template<class T>
    static void release(T **ptr);
    
    //autorelease object if not null
    template<class T>
    static T *autorelease(T *ptr);
    
    //retain object if not null
    template<class T>
    static void retain(T *ptr);
    
    //for global pull object
    template<class T>
    static T *gcpull(cchars key);
    
    //for global push object
    template<class T>
    T *gcpush(cchars key);
    
    //for remove global cache
    static void gcremove(cchars key);
    
    static void gcclear();
    
    //alloc or create name object
    static cxObject *alloc(cchars name);
    static cxObject *create(cchars name);
public:
    virtual cxULong Hash() const;
    
    cxLong Tag() const;
    void SetTag(cxLong value);

    template<class T> const T *To() const;
    template<class T> T *To();
    
    cxInt Refcount() const;
    void Retain();
    cxObject *AutoRelease();
    void Release();
    
    virtual cxJson *Serialize();
    
    void Init(const cxJson *json);
    virtual void SetProperty(cchars key, const cxJson *json);
};

template<class T>
CX_INLINE T *cxObject::To()
{
    return static_cast<T *>(this);
}

template<class T>
CX_INLINE const T *cxObject::To() const
{
    return static_cast<T *>(this);
}

template<class T>
CX_INLINE T *cxObject::gcpush(cchars key)
{
    cxCore::Instance()->Push(key,this);
    return static_cast<T *>(this);
}

template<class T>
CX_INLINE T *cxObject::gcpull(cchars key)
{
    return static_cast<T *>(cxCore::Instance()->Pull(key));
}

CX_INLINE void cxObject::gcclear()
{
    cxCore::Instance()->Clear();
}

CX_INLINE void cxObject::gcremove(cchars key)
{
    return cxCore::Instance()->Remove(key);
}

template<class T1,class T2>
CX_INLINE void cxObject::swap(T1 **ptr,T2 *dst)
{
    CX_ASSERT(ptr != nullptr, " ptr error");
    CX_ASSERT(*ptr == nullptr || *ptr != dst, "self swap self disable");
    if(*ptr != nullptr){
        (*ptr)->Release();
    }
    (*ptr) = (T1 *)dst;
    if((*ptr) != nullptr){
        (*ptr)->Retain();
    }
}
template<class T>
CX_INLINE void cxObject::release(T **ptr)
{
    if(*ptr == nullptr){
        return;
    }
    (*ptr)->Release();
    (*ptr) = nullptr;
}
template<class T>
CX_INLINE T *cxObject::autorelease(T *ptr)
{
    if(ptr == nullptr){
        return nullptr;
    }
    return ptr->AutoRelease();
}
template<class T>
CX_INLINE void cxObject::retain(T *ptr)
{
    if(ptr == nullptr){
        return;
    }
    ptr->Retain();
}

CX_CPP_END

#endif /* defined(__cxEngineCore__cxObject__) */
