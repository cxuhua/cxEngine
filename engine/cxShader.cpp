//
//  cxShader.cpp
//  cxEngineCore
//
//  Created by xuhua on 6/12/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#include <core/cxUtil.h>
#include "cxEngine.h"
#include "cxOpenGL.h"
#include "cxShader.h"
#include "cxView.h"

CX_CPP_BEGIN

//
CX_IMPLEMENT(cxShader);

void cxShader::Free()
{
    cxOpenGL::Instance()->DelShader(vsh);
    cxOpenGL::Instance()->DelShader(fsh);
    cxOpenGL::Instance()->DelProgram(program);
}

cxShader *cxShader::Vertex(cchars file)
{
    cxObject::swap(&vs, cxUtil::Assets(file));
    return this;
}

cxShader *cxShader::Fragment(cchars file)
{
    cxObject::swap(&fs, cxUtil::Assets(file));
    return this;
}

void cxShader::SetModelProject(const cxMatrixF &value) const
{
    SetUniform(ump, value);
}

void cxShader::SetModelView(const cxMatrixF &value) const
{
    SetUniform(ump, value);
}

void cxShader::Using() const
{
    cxOpenGL *gl = cxOpenGL::Instance();
    gl->UseProgram(program);
    SetModelProject(gl->ModelProject());
}

cxShader::cxShader()
{
    vs = nullptr;
    fs = nullptr;
    program = 0;
    vsh = 0;
    fsh = 0;
    ump = 0;
    umv = 0;
}

cxShader::~cxShader()
{
    cxObject::release(&vs);
    cxObject::release(&fs);
    Free();
}

cxBool cxShader::InitUniform()
{
    return true;
}

cxBool cxShader::InitAttrib()
{
    BindAttrib(cxVertexAttribPosition, CX_ATTR_POSITION);
    BindAttrib(cxVertexAttribColor, CX_ATTR_COLOR);
    BindAttrib(cxVertexAttribTexcoord, CX_ATTR_TEXCOORD);
    return true;
}

//
CX_IMPLEMENT(cxColorShader);

cxColorShader::cxColorShader()
{
    
}

cxColorShader::~cxColorShader()
{
    
}

void cxColorShader::Using() const
{
    cxOpenGL *gl = cxOpenGL::Instance();
    gl->UseProgram(program);
    SetModelProject(gl->ModelProject());
    SetModelView(gl->ModelView());
}

cxBool cxColorShader::InitAttrib()
{
    BindAttrib(cxVertexAttribPosition, CX_ATTR_POSITION);
    BindAttrib(cxVertexAttribColor, CX_ATTR_COLOR);
    return true;
}
//
CX_IMPLEMENT(cxEffectShader);

cxEffectShader::cxEffectShader()
{
    
}

cxEffectShader::~cxEffectShader()
{
    
}

cxBool cxEffectShader::InitUniform()
{
    time = GetUniform("time");
    return true;
}

void cxEffectShader::Using() const
{
    cxShader::Using();
    cxEngine *engine = cxEngine::Instance();
    SetUniform(time, engine->Time());
}

CX_CPP_END

