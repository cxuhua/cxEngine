//
//  cxShader.cpp
//  cxEngineCore
//
//  Created by xuhua on 6/12/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#include <core/cxUtil.h>
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
    program = 0;
}

cxShader::~cxShader()
{
    Free();
}

cxBool cxShader::InitUniform()
{
    return true;
}

const cxStr *cxShader::Vertex() const
{
    const cxStr *data = cxUtil::Assets("shader/default.vsh");
    CX_ASSERT(cxStr::IsOK(data), "shader/default.vsh miss");
    return data;
}
const cxStr *cxShader::Fragment() const
{
    const cxStr *data = cxUtil::Assets("shader/default.fsh");
    CX_ASSERT(cxStr::IsOK(data), "shader/default.fsh miss");
    return data;
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

const cxStr *cxColorShader::Vertex() const
{
    const cxStr *data = cxUtil::Assets("shader/color.vsh");
    CX_ASSERT(cxStr::IsOK(data), "shader/color.vsh miss");
    return data;
}

const cxStr *cxColorShader::Fragment() const
{
    const cxStr *data = cxUtil::Assets("shader/color.fsh");
    CX_ASSERT(cxStr::IsOK(data), "shader/color.fsh miss");
    return data;
}

CX_CPP_END

