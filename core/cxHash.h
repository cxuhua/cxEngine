//
//  cxHash.h
//  cxEngineCore
//
//  Created by xuhua on 5/27/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#ifndef cxEngineCore_cxHash_h
#define cxEngineCore_cxHash_h

#include <ext/xxhash.h>
#include <unordered_map>
#include "cxObject.h"
#include "cxStr.h"

CX_CPP_BEGIN

#define CX_HASH_MAX_KEY  64

struct cxHashKey
{
    cxInt length;
    cxByte data[CX_HASH_MAX_KEY + 1];
    cxHashKey();
    cxHashKey(cxInt key);
    cxHashKey(cxUInt64 key);
    cxHashKey(cchars key);
    cxHashKey(cxUInt32 key);
    cxHashKey(const cxStr *key);
    cchars Key() const;
    static cxHashKey Format(cchars fmt,...);
    static cxHashKey Format(cchars fmt,va_list ap);
};

struct cxHasher
{
    size_t operator()(const cxHashKey &k) const;
    bool operator()(const cxHashKey &lhs, const cxHashKey &rhs) const;
};

typedef std::unordered_map<cxHashKey, cxObject *, cxHasher, cxHasher> cxHashMap;

class cxArray;
class cxHash : public cxObject
{
public:
    CX_DECLARE(cxHash);
protected:
    explicit cxHash();
    virtual ~cxHash();
private:
    cxHashMap mh;
public:
    typedef cxHashMap::iterator Iter;
    typedef cxHashMap::const_iterator CIter;
public:
    static cxBool IsOK(const cxHash *v);
    //move src[key] to dst
    static void Move(const cxHashKey &key,cxHash *src,cxHash *dst);
public:
    Iter Begin();
    CIter Begin() const;
    
    Iter End();
    CIter End() const;
    
    Iter Remove(Iter &iter);
    Iter Remove(CIter &iter);
public:
    cxInt Size() const;
    cxBool IsEmpty() const;
    cxHash *Clear();
    void Del(const cxHashKey &key);
    cxHash *Set(const cxHashKey &key,cxObject *obj);
    cxBool Has(const cxHashKey &key);
    template<class T>
    T *Get(const cxHashKey &key)
    {
        return Get(key)->To<T>();
    }
    cxObject *Get(const cxHashKey &key);
    cxArray *Values();
    template<class T>
    void Elements(std::function<void(const cxHashKey &,T *)> func)
    {
        for(Iter it=Begin();it!=End();it++){
            func(it->first,it->second->To<T>());
        }
    }
};
CX_CPP_END

#endif


