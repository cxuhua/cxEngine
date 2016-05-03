//
//  cxScript.h
//  cxEngineCore
//
//  Created by xuhua on 5/2/16.
//  Copyright © 2016 xuhua. All rights reserved.
//

#ifndef cxEngineCore_cxScript_h
#define cxEngineCore_cxScript_h

#include <core/cxObject.h>
#include "cxTimer.h"

CX_CPP_BEGIN

#define SCRIPT_SELF(_t_)                                        \
cxObject **_obj_= (cxObject **)luaL_checkudata(L, 1, #_t_);     \
    _t_ *self = (*_obj_)->To<_t_>()

#define SCRIPT_DEF(_t_)                                         \
int Script_##_t_##_Init(lua_State *L);                          \
int Script_##_t_##_New(lua_State *L);                           \
int Script_##_t_##_GC(lua_State *L);

#define SCRIPT_BEG_FUNC(_t_)                                    \
static const struct luaL_Reg Script_##_t_##_Funcs[] = {         \
    {"new" , Script_##_t_##_New},                               \

#define SCRIPT_END_FUNC(_t_)    {NULL, NULL}};

#define SCRIPT_BEG_METHOD(_t_)                                  \
static const struct luaL_Reg Script_##_t_##_Methods[] = {       \
    {"__gc" , Script_##_t_##_GC},                               \

#define SCRIPT_END_METHOD(_t_)    {NULL, NULL}};

#define SCRIPT_IMP(_t_)                                         \
int Script_##_t_##_Init(lua_State *L){                          \
    luaL_newlib(L, Script_##_t_##_Funcs);                       \
    luaL_newmetatable(L, #_t_);                                 \
    lua_pushvalue(L, -1);                                       \
    lua_setfield(L, -2, "__index");                             \
    luaL_setfuncs(L, Script_##_t_##_Methods, 0);                \
    lua_pop(L, 1);                                              \
    return 1;                                                   \
}                                                               \
int Script_##_t_##_New(lua_State *L){                           \
    _t_ **obj = (_t_ **)lua_newuserdata(L, sizeof(_t_ *));      \
    *obj = _t_::Alloc();                                        \
    luaL_setmetatable(L, #_t_);                                 \
    return 1;                                                   \
}                                                               \
int Script_##_t_##_GC(lua_State *L){                            \
    SCRIPT_SELF(_t_);                                           \
    self->Release();                                            \
    return 0;                                                   \
}

#define SCRIPT_INIT(_t_)                                        \
    luaL_requiref(L, #_t_, Script_##_t_##_Init, 1);             \
    lua_pop(L, 1)

class cxScript
{
public:
};

CX_CPP_END

#endif


