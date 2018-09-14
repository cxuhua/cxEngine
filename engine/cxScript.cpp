//
//  cxScript.cpp
//  cxEngineCore
//
//  Created by 徐华 on 2018/9/14.
//  Copyright © 2018 xuhua. All rights reserved.
//

#include "cxScript.h"

CX_CPP_BEGIN

CX_IMPLEMENT(cxScript);

cxScript::cxScript()
{
    
}

cxScript::~cxScript()
{
    
}

cxInt cxScript::__LuaIndex(lua_State *l)
{
    cxScript **ext = (cxScript **)lua_touserdata(l, 1);
    //remove this args
    lua_remove(l, 1);
    return (*ext)->LuaIndex(l);
}
cxInt cxScript::__LuaNewIndex(lua_State *l)
{
    cxScript **ext = (cxScript **)lua_touserdata(l, 1);
    //remove this args
    lua_remove(l, 1);
    return (*ext)->LuaNewIndex(l);
}
cxInt cxScript::__LuaCall(lua_State *l)
{
    cxScript **ext = (cxScript **)lua_touserdata(l, 1);
    //remove this args
    lua_remove(l, 1);
    return (*ext)->LuaCall(l);
}
cxInt cxScript::__LuaGC(lua_State *l)
{
    cxObject **obj = (cxObject **)lua_touserdata(l, 1);
    (*obj)->Release();
    return 0;
}

cxInt cxScript::LuaCall(lua_State *l)
{
    CX_LOGGER("LuaCall");
    return 0;
}

cxInt cxScript::LuaIndex(lua_State *l)
{
    CX_LOGGER("LuaIndex");
    return 0;
}

cxInt cxScript::LuaNewIndex(lua_State *l)
{
    CX_LOGGER("LuaNewIndex");
    return 0;
}

void cxScript::PrintStack(lua_State *l)
{
    cxInt top = lua_gettop(l);
    for(cxInt i=1;i<=top;i++){
        CX_LOGGER("%d - %s",i,lua_typename(l, lua_type(l, i)));
    }
}

void cxScript::NewType(lua_State *l,cchars type)
{
    luaL_newmetatable(l,type);
    
    lua_pushstring(l,"__index" );
    lua_pushcfunction(l, cxScript::__LuaIndex);
    lua_rawset(l, -3 );
    
    lua_pushstring(l, "__newindex" );
    lua_pushcfunction(l, cxScript::__LuaNewIndex);
    lua_rawset(l, -3 );
    
    lua_pushstring(l, "__call" );
    lua_pushcfunction(l, cxScript::__LuaCall);
    lua_rawset(l, -3 );
    
    lua_pushstring(l, "__gc" );
    lua_pushcfunction(l, cxScript::__LuaGC);
    lua_rawset(l, -3 );
}

void cxScript::NewValue(lua_State *l,cchars type,cxScript *v)
{
    char buf[64]={0};
    snprintf(buf, 64, "__lua__%s__",type);
    //create object ptr save
    cxScript **obj = (cxScript **)lua_newuserdata(l, sizeof(cxScript **));
    *obj = v;(*obj)->Retain();
    //类型不存在就新创建一个
    if(luaL_getmetatable(l,buf) != LUA_TTABLE){
        //pop nil
        lua_pop(l, 1);
        //push meta table
        cxScript::NewType(l,buf);
    }
    lua_setmetatable(l, -2);
}

void cxScript::NewGlobal(lua_State *l,cchars type,cchars var,cxScript *v)
{
    NewValue(l,type,v);
    lua_setglobal(l, var);
}


CX_CPP_END

