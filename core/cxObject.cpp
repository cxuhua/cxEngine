//
//  cxObject.cpp
//  cxEngineCore
//
//  Created by xuhua on 5/24/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#include "cxStr.h"
#include "cxJson.h"
#include "cxObject.h"
#include "cxAutoPool.h"
#include "cxNotice.h"

CX_CPP_BEGIN

CX_IMPLEMENT(cxObject);

cxObject::~cxObject()
{
    UnBind();
}

cxObject::cxObject():refcount(1),tag(0)
{
    OnInit();
}

cxInt cxObject::__LuaIndex(lua_State *l)
{
    cxObject **ext = (cxObject **)lua_touserdata(l, 1);
    //remove this args
    lua_remove(l, 1);
    return (*ext)->LuaIndex(l);
}

cxInt cxObject::__LuaNewIndex(lua_State *l)
{
    cxObject **ext = (cxObject **)lua_touserdata(l, 1);
    //remove this args
    lua_remove(l, 1);
    return (*ext)->LuaNewIndex(l);
}

cxInt cxObject::__LuaCall(lua_State *l)
{
    cxObject **ext = (cxObject **)lua_touserdata(l, 1);
    //remove this args
    lua_remove(l, 1);
    return (*ext)->LuaCall(l);
}

cxInt cxObject::__LuaLT(lua_State *l)
{
    cxObject **ext = (cxObject **)lua_touserdata(l, 1);
    //remove this args
    lua_remove(l, 1);
    return (*ext)->LuaLT(l);
}

cxInt cxObject::__LuaGC(lua_State *l)
{
    cxObject **obj = (cxObject **)lua_touserdata(l, 1);
    lua_remove(l, 1);
    (*obj)->LuaGC(l);
    (*obj)->Release();
    return 0;
}

cxInt cxObject::LuaNewRef(lua_State *l)
{
    return luaL_ref(l, LUA_REGISTRYINDEX);
}

void cxObject::LuaGetRef(lua_State *l,cxInt ref)
{
    lua_rawgeti(l,LUA_REGISTRYINDEX,ref);
}

void cxObject::LuaDelRef(lua_State *l,cxInt ref)
{
    luaL_unref(l, LUA_REGISTRYINDEX, ref);
}

cxInt cxObject::LuaToInt(lua_State *l,cxInt idx,cxInt dv)
{
    if(lua_gettop(l) < idx){
        return dv;
    }
    if(lua_type(l, idx) == LUA_TNUMBER){
        return (cxInt)lua_tointeger(l, idx);
    }
    if(lua_type(l, idx) == LUA_TSTRING){
        return atoi(lua_tostring(l, idx));
    }
    return dv;
}

cxLong cxObject::LuaToLong(lua_State *l,cxInt idx,cxLong dv)
{
    if(lua_gettop(l) < idx){
        return dv;
    }
    if(lua_type(l, idx) == LUA_TNUMBER){
        return (cxLong)lua_tointeger(l, idx);
    }
    if(lua_type(l, idx) == LUA_TSTRING){
        return atol(lua_tostring(l, idx));
    }
    return dv;
}

const cxStr *cxObject::LuaToStr(lua_State *l,cxInt idx)
{
    if(lua_gettop(l) < idx){
        return nullptr;
    }
    if(lua_type(l, idx) == LUA_TNUMBER){
        if(lua_isinteger(l, idx)){
            return cxStr::UTF8("%d",(cxInt)lua_tointeger(l, idx));
        }
        if(lua_isnumber(l, idx)){
            return cxStr::UTF8("%f",lua_tonumber(l, idx));
        }
    }else if(lua_type(l, idx) == LUA_TSTRING){
        cchars txt = lua_tostring(l, idx);
        if(!cxStr::IsOK(txt)){
            return nullptr;
        }
        return cxStr::Create()->Init(txt);
    }
    return nullptr;
}

cchars cxObject::LuaToChars(lua_State *l,cxInt idx)
{
    if(lua_gettop(l) < idx){
        return nullptr;
    }
    if(lua_type(l, idx) != LUA_TSTRING){
        return nullptr;
    }
    return lua_tostring(l, idx);
}

cxFloat cxObject::LuaToFloat(lua_State *l,cxInt idx,cxFloat dv)
{
    if(lua_gettop(l) < idx){
        return dv;
    }
    if(lua_type(l, idx) == LUA_TNUMBER){
        return (cxFloat)lua_tonumber(l, idx);
    }
    if(lua_type(l, idx) == LUA_TSTRING){
        return (cxFloat)atof(lua_tostring(l, idx));
    }
    return dv;
}

cxBool cxObject::LuaToBool(lua_State *l,cxInt idx,cxBool dv)
{
    if(lua_gettop(l) < idx){
        return dv;
    }
    if(lua_type(l, idx) == LUA_TBOOLEAN){
        return (cxBool)lua_toboolean(l, idx);
    }
    if(lua_type(l, idx) == LUA_TNUMBER){
        return lua_tonumber(l, idx) != 0;
    }
    if(lua_type(l, idx) == LUA_TSTRING){
        return strcmp(lua_tostring(l, idx), "true");
    }
    return dv;
}

cxInt cxObject::LuaLT(lua_State *l)
{
    return 0;
}

cxInt cxObject::LuaGC(lua_State *l)
{
    return 0;
}

cxInt cxObject::LuaCall(lua_State *l)
{
    return 0;
}

cxInt cxObject::LuaIndex(lua_State *l)
{
    cchars key = lua_tostring(l, 1);
    if(cxStr::IsEqu(key, "tag")){
        lua_pushinteger(l, Tag());
        return 1;
    }
    return 0;
}

cxInt cxObject::LuaNewIndex(lua_State *l)
{
    cchars key = lua_tostring(l, 1);
    if(cxStr::IsEqu(key, "tag")){
        SetTag(LuaToLong(l, 2, 0));
        return 0;
    }
    return 0;
}

void cxObject::PrintStack(lua_State *l)
{
    cxInt top = lua_gettop(l);
    for(cxInt i=1;i<=top;i++){
        CX_LOGGER("%d - %s",i,lua_typename(l, lua_type(l, i)));
    }
}

void cxObject::NewType(lua_State *l,cchars type)
{
    luaL_newmetatable(l,type);
    
    lua_pushstring(l,"__index" );
    lua_pushcfunction(l, cxObject::__LuaIndex);
    lua_rawset(l, -3 );
    
    lua_pushstring(l, "__newindex" );
    lua_pushcfunction(l, cxObject::__LuaNewIndex);
    lua_rawset(l, -3 );
    
    lua_pushstring(l, "__call" );
    lua_pushcfunction(l, cxObject::__LuaCall);
    lua_rawset(l, -3 );
    
    lua_pushstring(l, "__lt" );
    lua_pushcfunction(l, cxObject::__LuaLT);
    lua_rawset(l, -3 );
    
    lua_pushstring(l, "__gc" );
    lua_pushcfunction(l, cxObject::__LuaGC);
    lua_rawset(l, -3 );
}

void cxObject::NewValue(lua_State *l,cchars type,cxObject *v)
{
    char buf[64]={0};
    snprintf(buf, 64, "__lua__%s__",type);
    //create object ptr save
    cxObject **obj = (cxObject **)lua_newuserdata(l, sizeof(cxObject **));
    *obj = v;(*obj)->Retain();
    if(luaL_getmetatable(l,buf) != LUA_TTABLE){
        //pop nil
        lua_pop(l, 1);
        //push meta table
        cxObject::NewType(l,buf);
    }
    lua_setmetatable(l, -2);
}

void cxObject::NewGlobal(lua_State *l,cchars type,cchars var,cxObject *v)
{
    NewValue(l,type,v);
    lua_setglobal(l, var);
}

const cxInt cxObject::BindesSize() const
{
    return (cxInt)bindes.size();
}

const cxInt cxObject::BindedSize() const
{
    return (cxInt)binded.size();
}

void cxObject::Bind(cxObject *pobj,cxLong tag)
{
    CX_ASSERT(pobj != nullptr, "pobj args error");
    CX_ASSERT(pobj != this, "self can't bind self");
    if(bindes.find(pobj) == bindes.end()){
        bindes[pobj] = tag;
    }
    if(pobj->binded.find(this) == pobj->binded.end()){
        pobj->binded[this] = tag;
    }
}

const cxBool cxObject::HasBindes(cxObject *pobj) const
{
    CX_ASSERT(pobj !=  nullptr, "pobj args error");
    return bindes.find(pobj) != bindes.end();
}

const cxBool cxObject::HasBinded(cxObject *pobj) const
{
    CX_ASSERT(pobj !=  nullptr, "pobj args error");
    return binded.find(pobj) != binded.end();
}

cxInt cxObject::EachBindes(std::function<cxBool(cxObject *pobj)> func)
{
    cxInt c = 0;
    if(bindes.empty()){
        return c;
    }
    BindMap::iterator it = bindes.begin();
    while(it != bindes.end()){
        cxObject *obj = (cxObject *)it->first;
        if(!func(obj))break;
        c++;
        it++;
    }
    return c;
}

const cxArray *cxObject::GetBindes()
{
    cxArray *objects = cxArray::Create();
    if(bindes.empty()){
        return objects;
    }
    BindMap::iterator it = bindes.begin();
    while(it != bindes.end()){
        cxObject *pobj = (cxObject *)it->first;
        objects->Append(pobj);
        it++;
    }
    return objects;
}

cxObject *cxObject::FirstBindes()
{
    if(bindes.empty()){
        return nullptr;
    }
    BindMap::iterator it = bindes.begin();
    if(it != bindes.end()){
        return (cxObject *)it->first;
    }
    return nullptr;
}

cxObject *cxObject::GetBindes(cxLong tag)
{
    if(bindes.empty()){
        return nullptr;
    }
    BindMap::iterator it = bindes.begin();
    while(it != bindes.end()){
        if(it->second == tag){
            return (cxObject *)it->first;
        }
        it++;
    }
    return nullptr;
}

cxInt cxObject::EachBinded(std::function<cxBool(cxObject *pobj)> func)
{
    cxInt c = 0;
    if(binded.empty()){
        return c;
    }
    BindMap::iterator it = binded.begin();
    while(it != binded.end()){
        cxObject *obj = (cxObject *)it->first;
        if(!func(obj))break;
        c++;
        it++;
    }
    return c;
}

const cxArray *cxObject::GetBinded()
{
    cxArray *objects = cxArray::Create();
    if(binded.empty()){
        return objects;
    }
    BindMap::iterator it = binded.begin();
    while(it != binded.end()){
        cxObject *pobj = (cxObject *)it->first;
        objects->Append(pobj);
        it++;
    }
    return objects;
}

cxObject *cxObject::FirstBinded()
{
    if(binded.empty()){
        return nullptr;
    }
    BindMap::iterator it = binded.begin();
    if(it != binded.end()){
        return (cxObject *)it->first;
    }
    return nullptr;
}

cxObject *cxObject::GetBinded(cxLong tag)
{
    if(binded.empty()){
        return nullptr;
    }
    BindMap::iterator it = binded.begin();
    while(it != binded.end()){
        if(it->second == tag){
            return (cxObject *)it->first;
        }
        it++;
    }
    return nullptr;
}

void cxObject::UnBind(cxObject *pobj)
{
    if(pobj == nullptr){
        return;
    }
    bindes.erase(pobj);
    pobj->binded.erase(this);
}

void cxObject::UnBind()
{
    BindMap::iterator it = bindes.begin();
    while(it != bindes.end()){
        cxObject *pobj = (cxObject *)it->first;
        pobj->binded.erase(this);
        it++;
    }
    bindes.clear();
    it = binded.begin();
    while(it != binded.end()){
        cxObject *pobj = (cxObject *)it->first;
        pobj->bindes.erase(this);
        it++;
    }
    binded.clear();
}

cxJson *cxObject::Serialize()
{
    return cxJson::CreateObject();
}

cxLong cxObject::Tag() const
{
    return tag;
}

cxULong cxObject::Hash() const
{
    return (cxULong)this;
}

void cxObject::SetTag(cxLong value)
{
    tag = value;
}

void cxObject::Retain()
{
    CX_ASSERT(refcount > 0, "error refcount should > 0");
    refcount.fetch_add(1);
}

cxInt cxObject::Refcount() const
{
    return refcount;
}

cxBool cxObject::IsType(cchars type)
{
    return cxStr::IsEqu(ClassName(), type);
}

cxObject *cxObject::AutoRelease()
{
    return cxAutoPool::Append(this);
}

cxObject *cxObject::initFromJson(const cxJson *json)
{
    cxJson::Iter it = json->Begin();
    while(it != json->End()){
        cxJson *value = cxJson::Alloc();
        if(it.Value(value)){
            SetProperty(it.Key(), value);
        }
        value->Release();
        it++;
    }
    return this;
}

void cxObject::SetProperty(cchars key,const cxJson *json)
{
    if(cxStr::IsEqu(key, "tag")){
        SetTag(json->ToInt());
    }
}

const cxJson *cxObject::GetProperty(cchars key)
{
    if(cxStr::IsEqu(key, "tag")){
        return cxJson::Create()->From((cxInt)Tag());
    }
    return nullptr;
}

void cxObject::OnFree()
{
    onFree.Fire(this);
}

void cxObject::OnInit()
{
    onInit.Fire(this);
}

void cxObject::Release()
{
    CX_ASSERT(refcount > 0, "error,retain count must > 0");
    if(refcount.fetch_sub(1) == 1){
        OnFree();
        delete this;
    }
}

CX_CPP_END











