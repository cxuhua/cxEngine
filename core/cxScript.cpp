//
//  Script.cpp
//  mgame-ios
//
//  Created by 徐华 on 2018/8/26.
//  Copyright © 2018 xuhua. All rights reserved.
//

#include "cxUtil.h"
#include "cxScript.h"

CX_CPP_BEGIN

extern "C"
{
    int luaopen_cjson(lua_State *l);
}

CX_IMPLEMENT(cxScript)

cxScript::cxScript()
{
    L = luaL_newstate();
    luaL_openlibs(L);
    luaopen_cjson(L);
}

cxScript::~cxScript()
{
    lua_close(L);
}

void cxScript::NewValue(cxObject *v)
{
    if(v == nullptr){
        PushNULL();
    }else{
        cxObject::NewValue(L, v->ClassName(), v);
    }
}

void cxScript::NewGlobal(cchars name,cxObject *v)
{
    NewValue(v);
    SetGlobal(name);
}

void cxScript::GCFull()
{
    lua_gc(L, LUA_GCCOLLECT, 0);
}

void cxScript::GCStep(cxInt data)
{
    lua_gc(L, LUA_GCSTEP, data);
}

void cxScript::pushJsonArray(const cxJson *json)
{
    CX_ASSERT(json!=nullptr && json->IsArray(), "json error");
    cxInt i = 0;
    for(cxJson::Iter it = json->Begin();it != json->End();it++){
        cxJson *v = cxJson::Alloc();
        if(it.Value(v)){
            pushJsonArrayValue(i,v);
            i++;
        }
        v->Release();
    }
}

void cxScript::pushJsonObject(const cxJson *json)
{
    CX_ASSERT(json!=nullptr && json->IsObject(), "json error");
    for(cxJson::Iter it = json->Begin();it != json->End();it++){
        cxJson *v = cxJson::Alloc();
        if(it.Value(v)){
            pushJsonObjectValue(it.Key(), v);
        }
        v->Release();
    }
}

void cxScript::SetTable(cxInt i)
{
    lua_rawseti(L, -2, i);
}

void cxScript::SetTable(cxInt i,const cxJson *json)
{
    if(cxJson::IsOK(json)){
        pushJsonArrayValue(i, json);
    }
}

void cxScript::SetNULL(cxInt i)
{
    PushNULL();
    lua_rawseti(L, -2, i);
}

void cxScript::SetPtr(cxInt i, void *ptr)
{
    PushPtr(ptr);
    lua_rawseti(L, -2, i);
}

void cxScript::SetInt(cxInt i, cxInt v)
{
    PushInt(v);
    lua_rawseti(L, -2, i);
}

void cxScript::SetLong(cxInt i, cxLong v)
{
    PushLong(v);
    lua_rawseti(L, -2, i);
}

void cxScript::SetFloat(cxInt i, cxFloat v)
{
    PushFloat(v);
    lua_rawseti(L, -2, i);
}

void cxScript::SetString(cxInt i, const cxStr *s)
{
    SetString(i, s->ToChars());
}

void cxScript::SetString(cxInt i, cchars s)
{
    PushString(s);
    lua_rawseti(L, -2, i);
}

void cxScript::SetBool(cxInt i, cxBool v)
{
    PushBool(v);
    lua_rawseti(L, -2, i);
}

void cxScript::pushJsonArrayValue(cxInt i,const cxJson *json)
{
    if(json->IsNull()){
        SetNULL(i);
    }else if(json->IsInt()){
        SetInt(i, json->ToInt());
    }else if(json->IsFloat()){
        SetFloat(i, json->ToFloat());
    }else if(json->IsString()){
        SetString(i, json->ToChars());
    }else if(json->IsBool()){
        SetBool(i, json->ToBool());
    }else if(json->IsArray()){
        NewTable();
        pushJsonArray(json->ToArray());
        SetTable(i);
    }else if(json->IsObject()){
        NewTable();
        pushJsonObject(json->ToObject());
        SetTable(i);
    }else{
        CX_ERROR("json type error");
    }
}

void cxScript::pushJsonObjectValue(cchars key,const cxJson *json)
{
    if(json->IsNull()){
        SetNULL(key);
    }else if(json->IsInt()){
        SetInt(key, json->ToInt());
    }else if(json->IsFloat()){
        SetFloat(key, json->ToFloat());
    }else if(json->IsString()){
        SetString(key, json->ToStr());
    }else if(json->IsBool()){
        SetBool(key, json->ToBool());
    }else if(json->IsArray()){
        NewTable();
        pushJsonArray(json);
        SetTable(key);
    }else if(json->IsObject()){
        NewTable();
        pushJsonObject(json);
        SetTable(key);
    }else{
        CX_ERROR("json type error");
    }
}

void cxScript::PushJson(const cxJson *json)
{
    NewTable();
    if(json->IsArray()){
        pushJsonArray(json);
    }else if(json->IsObject()){
        pushJsonObject(json);
    }else {
        CX_ERROR("not support push json");
    }
}

cxInt cxScript::getArrayLength()
{
    cxInt max  = -1;
    lua_pushnil(L);
    while (lua_next(L, -2) != 0) {
        if(lua_type(L, -2) != LUA_TNUMBER){
            lua_pop(L, 2);
            return -1;
        }
        double k = lua_tonumber(L, -2);
        if (floor(k) == k && k >= 0) {
            if (k > max)
                max = k;
            lua_pop(L, 1);
            continue;
        }
        lua_pop(L, 2);
        return -1;
    }
    return max;
}

void cxScript::appendObjectData(cchars key,cxJson *json)
{
    switch (lua_type(L, -1)) {
        case LUA_TSTRING:{
            json->Set(key, lua_tostring(L, -1));
            break;
        }
        case LUA_TNUMBER:{
            double v = lua_tonumber(L, -1);
            if(floor(v) == v){
                json->Set(key, (cxInt)v);
            }else{
                json->Set(key, (cxFloat)v);
            }
            break;
        }
        case LUA_TBOOLEAN:{
            json->Set(key, lua_toboolean(L, -1)?true:false);
            break;
        }
        case LUA_TTABLE:{
            int len = getArrayLength();
            cxJson *jv = nullptr;
            if(len > 0){
                jv = cxJson::CreateArray();
                appendArray(jv,len);
            }else{
                jv = cxJson::CreateObject();
                appendObject(jv);
            }
            json->Set(key, jv);
            break;
        }
        default:{
            break;
        }
    }
}

void cxScript::appendArrayData(cxInt i,cxJson *json)
{
    switch (lua_type(L, -1)) {
        case LUA_TSTRING:{
            json->Append(lua_tostring(L, -1));
            break;
        }
        case LUA_TNUMBER:{
            double v = lua_tonumber(L, -1);
            if(floor(v) == v){
                json->Append((cxInt)v);
            }else{
                json->Append((cxFloat)v);
            }
            break;
        }
        case LUA_TBOOLEAN:{
            json->Append(lua_toboolean(L, -1)?true:false);
            break;
        }
        case LUA_TTABLE:{
            int len = getArrayLength();
            cxJson *jv = nullptr;
            if(len > 0){
                jv = cxJson::CreateArray();
                appendArray(jv,len);
            }else{
                jv = cxJson::CreateObject();
                appendObject(jv);
            }
            json->Append(jv);
            break;
        }
        default:{
            break;
        }
    }
}

void cxScript::appendArray(cxJson *json,cxInt size)
{
    for (cxInt i = 0; i < size; i++) {
        lua_rawgeti(L, -1, i+1);
        appendArrayData(i,json);
        lua_pop(L, 1);
    }
}

void cxScript::appendObject(cxJson *json)
{
    lua_pushnil(L);
    while (lua_next(L, -2) != 0) {
        char key[64]={0};
        int keytype = lua_type(L, -2);
        if (keytype == LUA_TNUMBER) {
            snprintf(key, 64, "%d",(cxInt)lua_tointeger(L, -2));
        } else if (keytype == LUA_TSTRING) {
            snprintf(key, 64, "%s",lua_tostring(L,  -2));
        } else {
            CX_ERROR("lua table key type error");
            continue;
        }
        appendObjectData(key, json);
        lua_pop(L, 1);
    }
}

cxJson *cxScript::GetJson()
{
    int len = getArrayLength();
    cxJson *rv = nullptr;
    if(len > 0){
        rv = cxJson::CreateArray();
        appendArray(rv, len);
    }else {
        rv = cxJson::CreateObject();
        appendObject(rv);
    }
    return rv;
}

void cxScript::NewTable()
{
    lua_newtable(L);
}

void cxScript::SetTable(cchars key)
{
    lua_setfield(L, -2, key);
}

void cxScript::SetTable(cchars key,const cxJson *json)
{
    if(cxJson::IsOK(json)){
        pushJsonObjectValue(key, json);
    }
}

void cxScript::SetNULL(cchars key)
{
    PushNULL();
    lua_setfield(L, -2, key);
}

void cxScript::SetPtr(cchars key, void *ptr)
{
    PushPtr(ptr);
    lua_setfield(L, -2, key);
}

void cxScript::SetInt(cchars key, cxInt v)
{
    PushInt(v);
    lua_setfield(L, -2, key);
}

void cxScript::SetLong(cchars key, cxLong v)
{
    PushLong(v);
    lua_setfield(L, -2, key);
}

void cxScript::SetFloat(cchars key, cxFloat v)
{
    PushFloat(v);
    lua_setfield(L, -2, key);
}

void cxScript::SetString(cchars key, const cxStr *s)
{
    PushString(s);
    lua_setfield(L, -2, key);
}

void cxScript::SetString(cchars key, cchars s)
{
    PushString(s);
    lua_setfield(L, -2, key);
}

void cxScript::SetBool(cchars key, cxBool v)
{
    PushBool(v);
    lua_setfield(L, -2, key);
}

void cxScript::SetFunc(cchars key,lua_CFunction f)
{
    PushFunc(f);
    lua_setfield(L, -2, key);
}

void cxScript::PushNULL()
{
    lua_pushnil(L);
}

void cxScript::PushPtr(void *ptr)
{
    lua_pushlightuserdata(L, ptr);
}

void cxScript::PushInt(cxInt v)
{
    lua_pushinteger(L, v);
}

void cxScript::PushLong(cxLong v)
{
    lua_pushinteger(L, v);
}

void cxScript::PushFloat(cxFloat v)
{
    lua_pushnumber(L, v);
}

void cxScript::PushString(const cxStr *s)
{
    if(cxStr::IsOK(s)){
        PushString(s->ToChars());
    }else{
        lua_pushnil(L);
    }
}

void cxScript::PushString(cchars s)
{
    if(cxStr::IsOK(s)){
        lua_pushstring(L, s);
    }else{
        lua_pushnil(L);
    }
}

void cxScript::PushFunc(lua_CFunction f)
{
    lua_pushcfunction(L, f);
}

void cxScript::PushBool(cxBool v)
{
    lua_pushboolean(L, v);
}

cxInt cxScript::GetGlobal(cchars name)
{
    return lua_getglobal(L, name);
}

cxInt cxScript::GetField(cxInt idx,cchars key)
{
    return lua_getfield(L, idx, key);
}

void cxScript::SetField(cxInt idx,cchars key)
{
    lua_setfield(L, idx, key);
}

void cxScript::DelGlobal(cchars name)
{
    PushNULL();
    SetGlobal(name);
}

void cxScript::SetGlobal(cchars name)
{
    lua_setglobal(L, name);
}

void cxScript::SetGlobal(cchars name,cxInt v)
{
    PushInt(v);
    SetGlobal(name);
}

void cxScript::SetGlobal(cchars name,cchars v)
{
    PushString(v);
    SetGlobal(name);
}

void cxScript::SetGlobal(cchars name,const cxStr *v)
{
    PushString(v);
    SetGlobal(name);
}

void cxScript::SetGlobal(cchars name,cxFloat v)
{
    PushFloat(v);
    SetGlobal(name);
}

void cxScript::SetGlobal(cchars name,cxBool v)
{
    PushBool(v);
    SetGlobal(name);
}

void cxScript::OnError(cchars error)
{
    onError.Fire(this, error);
}

cxInt cxScript::Top()
{
    return lua_gettop(L);
}

void cxScript::Pop(cxInt n)
{
    lua_pop(L, n);
}

cxBool cxScript::Call(cxInt anum,cxInt rnum)
{
    cxInt ret  = lua_pcall(L, anum, rnum, 0);
    if(ret != LUA_OK){
        cchars err = lua_tostring(L, -1);
        CX_LOGGER("call lua script error %d-%s",ret,err);
        OnError(err);
        Pop(1);
    }
    return ret == LUA_OK;
}

cxBool cxScript::Load(const cxStr *script,cxBool run)
{
    CX_ASSERT(cxStr::IsOK(script), "args error");
    cxInt ret = luaL_loadstring(L, script->ToChars());
    if(ret != LUA_OK){
        cchars err = lua_tostring(L, -1);
        CX_ERROR("load lua string error %d-%s",ret,err);
        OnError(err);
        Pop(1);
        return false;
    }
    if(run){
        return Call(0, 0);
    }
    return ret == LUA_OK;
}

CX_CPP_END

