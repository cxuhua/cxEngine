//
// cxScript.h
// cxEngineCore
//
// Created by 徐华 on 2018/9/14.
//Copyright © 2018 xuhua. All rights reserved.
//

#ifndef cxScript_h
#define cxScript_h

#include <core/cxObject.h>
#include <lua/src/lua.hpp>

CX_CPP_BEGIN

class cxScript : public cxObject
{
public:
    CX_DECLARE(cxScript);
protected:
    explicit cxScript();
    virtual ~cxScript();
private:
    static cxInt __LuaIndex(lua_State *l);
    static cxInt __LuaNewIndex(lua_State *l);
    static cxInt __LuaCall(lua_State *l);
    static cxInt __LuaGC(lua_State *l);
protected:
    virtual cxInt LuaCall(lua_State *l);
    virtual cxInt LuaIndex(lua_State *l);
    virtual cxInt LuaNewIndex(lua_State *l);
public:
    static void PrintStack(lua_State *l);
    static void NewType(lua_State *l,cchars type);
    static void NewValue(lua_State *l,cchars type,cxScript *v);
    static void NewGlobal(lua_State *l,cchars type,cchars var,cxScript *v);
};

CX_CPP_END

#endif
