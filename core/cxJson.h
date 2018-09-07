//
//  cxJson.h
//  cxEngineCore
//
//  Created by xuhua on 5/24/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#ifndef __cxEngineCore__cxJson__
#define __cxEngineCore__cxJson__

#include <ext/jansson.h>
#include "cxObject.h"
#include "cxStr.h"

CX_CPP_BEGIN

class cxJson : public cxObject
{
public:
    CX_DECLARE(cxJson);
protected:
    explicit cxJson();
    virtual ~cxJson();
public:
    static cxJson *Create(cchars fmt,...);
public:
    struct Iter
    {
        cxInt number;
        cxInt step;
        void *objIt;
        json_t *json;
        Iter(json_t *);
        Iter &operator++(int);
        Iter &operator++();
        cxBool operator==(const Iter &v);
        cxBool operator!=(const Iter &v);
        Iter &operator+=(cxInt n);
        cxBool IsOK();
        Iter &Next();
        cxBool Value(cxJson *j);
        cxJson *Alloc();
        cxJson *Value();
        cxInt Index();
        cchars Key();
    };
    const Iter Begin() const;
    const Iter &End() const;
private:
    static cxInt jsonDumpFunc(cchars buffer, size_t size, void *data);
    json_t *json;
public:
    
    static cxBool IsOK(const cxJson *json);
    
    static cxJson *CreateObject();
    static cxJson *CreateArray();
    
    const cxStr *Dumps(cxInt flags=0) const;
    
    cxJson *Append(cxBool value);
    cxJson *Append(cxInt value);
    cxJson *Append(cxInt64 value);
    cxJson *Append(cxFloat value);
    cxJson *Append(cchars value);
    cxJson *Append(const cxStr *value);
    cxJson *Append(const cxJson *value);
    
    const cxJson *At(cxInt idx) const;
    const cxJson *At(const cxStr *key) const;
    const cxJson *At(cchars key) const;
    
    const cxJson *Select(cchars sel) const;
    
    cxJson *Set(cchars key,cxBool value);
    cxJson *Set(cchars key,cxInt value);
    cxJson *Set(cchars key,cxInt64 value);
    cxJson *Set(cchars key,cxFloat value);
    cxJson *Set(cchars key,cchars value);
    cxJson *Set(cchars key,const cxStr *value);
    cxJson *Set(cchars key,const cxJson *value);
    
    cxBool Get(cchars key,cxBool value) const;
    cxInt Get(cchars key,cxInt value) const;
    cxInt64 Get(cchars key,cxInt64 value) const;
    cxFloat Get(cchars key,cxFloat value) const;
    cchars Get(cchars key,cchars value) const;
    const cxStr *Get(cchars key) const;
    
    cxBool Get(cxInt idx,cxBool value) const;
    cxInt Get(cxInt idx,cxInt value) const;
    cxInt64 Get(cxInt idx,cxInt64 value) const;
    cxFloat Get(cxInt idx,cxFloat value) const;
    cchars Get(cxInt idx,cchars value) const;
    const cxStr *Get(cxInt idx) const;
    
    
    cxInt Size() const;
    
    cxJson *ToAny() const;
    cxJson *ToArray() const;
    cxJson *ToObject() const;
    cxBool ToBool() const;
    cxInt ToInt() const;
    cxInt64 ToInt64() const;
    cxFloat ToFloat() const;
    cchars ToChars() const;
    const cxStr *ToStr() const;
    json_t *ToJson() const;

    cxJson *Array();
    cxJson *Object();
    cxJson *From(cxInt value);
    cxJson *From(cxFloat value);
    cxJson *From(cxBool value);
    cxJson *From(cchars str);
    cxJson *From(cchars buf,cxInt size);
    cxJson *From(json_t *js);
    cxJson *From(const cxStr *str);
    cxJson *From(const cxJson *json);
    
    cxBool IsObject() const;
    cxBool IsArray() const;
    cxBool IsBool() const;
    cxBool IsString() const;
    cxBool IsFloat() const;
    cxBool IsInt() const;
    cxBool IsNumber() const;
    cxBool IsNull() const;
};

CX_CPP_END

#endif /* defined(__cxEngineCore__cxJson__) */
