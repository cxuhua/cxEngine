//
//  cxObject.h
//  cxEngineCore
//
//  Created by xuhua on 5/24/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#ifndef __cxEngineCore__cxObject__
#define __cxEngineCore__cxObject__

#include <map>
#include <set>
#include <string>
#include <math.h>
#include <atomic>
#include <float.h>
#include "cxCore.h"
#include "cxEvent.h"

CX_CPP_BEGIN

void cxUtilInfo(cchars file,int line,cchars format, ...) CX_ATTR_FORMAT(3,4);

void cxUtilError(cchars file,int line,cchars format, ...) CX_ATTR_FORMAT(3,4);

void cxUtilWarn(cchars file,int line,cchars format, ...) CX_ATTR_FORMAT(3,4);

void cxUtilAssert(cchars file,int line,cchars format, ...) CX_ATTR_FORMAT(3,4);

class cxArray;
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
    //support bind
    typedef std::map<void *,cxLong> BindMap;
    BindMap bindes;
    BindMap binded;
    std::atomic_int refcount;
    cxLong tag;
    cxObject *initFromJson(const cxJson *json);
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
    static T *gcGet(cchars key);
    
    //for global push object
    template<class T>
    T *gcSet(cchars key);
    
    //for remove global cache
    static void gcDel(cchars key);
    
    //clear gc
    static void gcClear();

public:
    virtual cxULong Hash() const;
    cxLong Tag() const;
    void SetTag(cxLong value);
public:
    template<class T> const T *To() const
    {
        return static_cast<const T *>(this);
    }
    template<class T> T *To()
    {
        return static_cast<T *>(this);
    }
    template<class T> static T *To(cxObject *pobj)
    {
        return static_cast<T *>(pobj);
    }
public:
    //mempory ref manager
    cxInt Refcount() const;
    void Retain();
    cxObject *AutoRelease();
    void Release();
public:
    //serialize support
    virtual cxJson *Serialize();
    virtual void SetProperty(cchars key, const cxJson *json);
    virtual const cxJson *GetProperty(cchars key);
public:
    //bind support, not thread safe
    const cxInt BindesSize() const;
    const cxInt BindedSize() const;
    //
    cxInt EachBindes(std::function<cxBool(cxObject *pobj)> func);
    const cxArray *GetBindes();
    cxObject *GetBindes(cxLong tag);
    //
    cxInt EachBinded(std::function<cxBool(cxObject *pobj)> func);
    const cxArray *GetBinded();
    cxObject *GetBinded(cxLong tag);
    //if this bind obj
    const cxBool HasBindes(cxObject *pobj) const;
    //if this binded obj
    const cxBool HasBinded(cxObject *pobj) const;
    //
    void Bind(cxObject *pobj,cxLong tag=0);
    void UnBind(cxObject *pobj);
    void UnBind();
public:
    cxEvent<cxObject> onFree;
    cxEvent<cxObject> onInit;
};

template<class T>
CX_INLINE T *cxObject::gcSet(cchars key)
{
    cxCore::Instance()->Push(key,this);
    return static_cast<T *>(this);
}

template<class T>
CX_INLINE T *cxObject::gcGet(cchars key)
{
    return static_cast<T *>(cxCore::Instance()->Pull(key));
}

CX_INLINE void cxObject::gcClear()
{
    cxCore::Instance()->Clear();
}

CX_INLINE void cxObject::gcDel(cchars key)
{
    return cxCore::Instance()->Remove(key);
}

template<class T1,class T2>
CX_INLINE void cxObject::swap(T1 **ptr,T2 *dst)
{
    CX_ASSERT(ptr != nullptr, " ptr error");
    if(*ptr == dst){
        return;
    }
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
