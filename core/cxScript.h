//
// Script.h
// mgame-ios
//
// Created by 徐华 on 2018/8/26.
//Copyright © 2018 xuhua. All rights reserved.
//

#ifndef cxScript_h
#define cxScript_h

#include "cxJson.h"
#include "cxHash.h"

CX_CPP_BEGIN

class cxScript : public cxObject
{
public:
    CX_DECLARE(cxScript);
protected:
    explicit cxScript();
    virtual ~cxScript();
protected:
    lua_State *L;
    void OnError(cchars error);
public:
    cxInt Top();
    void Pop(cxInt n);
    cxBool Load(const cxStr *script,cxBool run=true);
    cxBool Call(cxInt anum,cxInt rnum);
    cxInt GetGlobal(cchars name);
    void DelGlobal(cchars name);
    void SetGlobal(cchars name);
    void SetGlobal(cchars name,cxInt v);
    void SetGlobal(cchars name,cchars v);
    void SetGlobal(cchars name,const cxStr *v);
    void SetGlobal(cchars name,cxFloat v);
    void SetGlobal(cchars name,cxBool v);
    cxInt GetField(cxInt idx,cchars key);
    void SetField(cxInt idx,cchars key);
private:
    void pushJsonObjectValue(cchars key, const cxJson *json);
    void pushJsonArrayValue(cxInt i,const cxJson *json);
    void pushJsonArray(const cxJson *json);
    void pushJsonObject(const cxJson *json);
    cxInt getArrayLength();
    void appendArrayData(cxInt i,cxJson *json);
    void appendObjectData(cchars key,cxJson *json);
    void appendArray(cxJson *json,cxInt size);
    void appendObject(cxJson *json);
public:
    void GCFull();
    void GCStep(cxInt data=0);
public:
    void NewValue(cchars type,cxObject *v);
    void NewGlobal(cchars type,cchars name,cxObject *v);
public:
    void PushNULL();
    void PushPtr(void *ptr);
    void PushInt(cxInt v);
    void PushLong(cxLong v);
    void PushFloat(cxFloat v);
    void PushString(const cxStr *s);
    void PushString(cchars s);
    void PushBool(cxBool v);
    void PushFunc(lua_CFunction f);
    
    //use Set* method,set table filed
    void NewTable();
    
    void SetTable(cchars key);
    void SetTable(cchars key,const cxJson *json);
    void SetNULL(cchars key);
    void SetPtr(cchars key, void *ptr);
    void SetInt(cchars key, cxInt v);
    void SetLong(cchars key, cxLong v);
    void SetFloat(cchars key, cxFloat v);
    void SetString(cchars key, const cxStr *s);
    void SetString(cchars key, cchars s);
    void SetBool(cchars key, cxBool v);
    void SetFunc(cchars key,lua_CFunction f);
    
    void SetTable(cxInt i);
    void SetTable(cxInt i,const cxJson *json);
    void SetNULL(cxInt i);
    void SetPtr(cxInt i, void *ptr);
    void SetInt(cxInt i, cxInt v);
    void SetLong(cxInt i, cxLong v);
    void SetFloat(cxInt i, cxFloat v);
    void SetString(cxInt i, const cxStr *s);
    void SetString(cxInt i, cchars s);
    void SetBool(cxInt i, cxBool v);
    
    //将json转为table压入lua
    void PushJson(const cxJson *json);
    //从顶部的table转为json
    cxJson *GetJson();
public:
    cxEvent<cxScript, cchars> onError;
};
CX_CPP_END

#endif
