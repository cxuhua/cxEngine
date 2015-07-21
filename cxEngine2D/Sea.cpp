//
//  Sea.cpp
//  cxEngineTest
//
//  Created by xuhua on 7/21/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#include <engine//cxEngine.h>
#include "Sea.h"

CX_CPP_BEGIN

CX_IMPLEMENT(Sea);

Sea::Sea()
{
    
}

Sea::~Sea()
{
    
}

void Sea::Using() const
{
    cxEngine *el = cxEngine::Instance();
    cxOpenGL *gl = cxOpenGL::Instance();
    gl->UseProgram(program);
    SetModelProject(gl->ModelProject());
    SetUniform(u_time, el->Time());
    SetUniform(u_1DivLevelHeight, 1);
    SetUniform(u_1DivLevelWidth, 1);
    SetUniform(WAVE_HEIGHT, 30);
    SetUniform(WAVE_MOVEMENT, 40);
    SetUniform(SHORE_DARK, cxPoint3F(0.5, 0.5, 0.5));
    SetUniform(SHORE_DARK, cxPoint3F(1, 1, 1));
    SetUniform(SEA_DARK, cxPoint3F(0.043f,0.675f,0.533f));
    SetUniform(SEA_LIGHT, cxPoint3F(1, 1, 1));
    SetUniform(u_lightPos, cxPoint3F(0, 0, 0));
}

cxBool Sea::InitUniform()
{
    ump = GetUniform("u_mvp");
    u_time = GetUniform("u_time");
    u_1DivLevelWidth = GetUniform("u_1DivLevelWidth");
    u_1DivLevelHeight = GetUniform("u_1DivLevelHeight");
    WAVE_HEIGHT = GetUniform("WAVE_HEIGHT");
    WAVE_MOVEMENT = GetUniform("WAVE_MOVEMENT");
    
    SHORE_DARK = GetUniform("SHORE_DARK");
    SHORE_LIGHT = GetUniform("SHORE_LIGHT");
    SEA_DARK = GetUniform("SEA_DARK");
    SEA_LIGHT = GetUniform("SEA_LIGHT");
    
    u_lightPos = GetUniform("u_lightPos");
    return true;
}

cxBool Sea::InitAttrib()
{
    BindAttrib(cxVertexAttribPosition, "a_pos");
    BindAttrib(cxVertexAttribColor, "a_color");
    BindAttrib(cxVertexAttribTexcoord, "a_uv0");
    return true;
}

CX_CPP_END

