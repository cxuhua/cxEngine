//
//  cxOpenGL.cpp
//  cxEngineCore
//
//  Created by xuhua on 6/11/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//


#include "cxOpenGL.h"
#include "cxView.h"
#include "cxShader.h"
#include <math/cxRect4F.h>
#include <math/cxBox4F.h>
#include <math/cxMatrixF.h>
#include <math/cxBoxPoint2F.h>
#include <math/cxBoxRender.h>
#include <core/cxUtil.h>

CX_CPP_BEGIN

#include "system.shader"

#define CX_GL_SUPPORT(t)                          \
support_##t = strstr(extensions,#t) > 0;          \
if(support_##t){CX_LOGGER("%s = YES",#t);}        \
else {CX_LOGGER("%s = NO",#t);}

cxOpenGL *cxOpenGL::instance = nullptr;

CX_IMPLEMENT(cxOpenGL);

void cxOpenGL::Init()
{
    CX_LOGGER("GL_VENDOR:   %s", glGetString(GL_VENDOR));
    CX_LOGGER("GL_RENDERER: %s", glGetString(GL_RENDERER));
    CX_LOGGER("GL_VERSION:  %s", glGetString(GL_VERSION));
    
    cchars extensions = (cchars)glGetString(GL_EXTENSIONS);
    CX_ASSERT(extensions != NULL, "gl extensions get error");
    CX_GL_SUPPORT(GL_OES_rgb8_rgba8);
    CX_GL_SUPPORT(GL_IMG_texture_npot);
    CX_GL_SUPPORT(GL_IMG_texture_compression_pvrtc);
    CX_GL_SUPPORT(GL_OES_compressed_ETC1_RGB8_texture);
    CX_GL_SUPPORT(GL_OES_vertex_array_object);
    CX_GL_SUPPORT(GL_EXT_discard_framebuffer);
    CX_GL_SUPPORT(GL_OES_mapbuffer);
    CX_GL_SUPPORT(GL_OES_packed_depth_stencil);

#if (CX_TARGET_PLATFORM == CX_PLATFORM_ANDROID)
    //support GL_OES_vertex_array_object
    glGenVertexArrays = (PFNGLGENVERTEXARRAYSOESPROC)eglGetProcAddress("glGenVertexArraysOES");
    glBindVertexArray = (PFNGLBINDVERTEXARRAYOESPROC)eglGetProcAddress("glBindVertexArrayOES");
    glDeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSOESPROC)eglGetProcAddress("glDeleteVertexArraysOES" );
    glIsVertexArray = (PFNGLISVERTEXARRAYOESPROC)eglGetProcAddress ("glIsVertexArrayOES");
    support_GL_OES_vertex_array_object = false;
    //support GL_OES_mapbuffer
    glMapBuffer = (PFNGLMAPBUFFEROESPROC)eglGetProcAddress("glMapBufferOES");
    glUnmapBuffer = (PFNGLUNMAPBUFFEROESPROC)eglGetProcAddress("glUnmapBufferOES");
    glGetBufferPointerv = (PFNGLGETBUFFERPOINTERVOESPROC)eglGetProcAddress("glGetBufferPointervOES");
    support_GL_OES_mapbuffer = false;
#endif
    
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSize);
    CX_LOGGER("GL_MAX_TEXTURE_SIZE: %d",maxTextureSize);
    
    glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &maxCombinedTextureUnits);
    CX_LOGGER("GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS: %d",maxCombinedTextureUnits);
    
    glGetIntegerv(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS, &maxVertexTextureUnits);
    CX_LOGGER("GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS: %d",maxVertexTextureUnits);
    //init system shader
    cxShader::Create()->Init(DefaultVSH,DefaultFSH)->gcSet<cxShader>(DefaultShader);
    cxShader::Create()->Init(GrayVSH,GrayFSH)->gcSet<cxShader>(GrayShader);
    cxColorShader::Create()->Init(ColorVSH,ColorFSH)->gcSet<cxShader>(ColorShader);
    //init system color
    SetClearColor(cxColor4F::BLACK);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_SCISSOR_TEST);
    glDisable(GL_STENCIL_TEST);
}

void cxOpenGL::SetClearColor(const cxColor4F &clear)
{
    glClearColor(clear.r, clear.g, clear.b, clear.a);
}

void cxOpenGL::SetTextureParams(const cxTextureParams &params)
{
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, params.minFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, params.magFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, params.wrapS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, params.wrapT);
}

cxBool cxOpenGL::DeleteTexture(cxTextureId texId)
{
    glDeleteTextures(1, (GLuint *)&texId);
    return glGetError() == GL_NO_ERROR;
}

void cxOpenGL::SetViewport(const cxRect4F &rect)
{
    glViewport(rect.x,rect.y,rect.w,rect.h);
}

cxBool cxOpenGL::BindTexture(cxTextureId texId,cxInt idx)
{
    GLenum error = GL_NO_ERROR;
    if(currTexId[idx] != texId){
        glActiveTexture(GL_TEXTURE0 + idx);
        glBindTexture(GL_TEXTURE_2D, texId);
        error = glGetError();
        currTexId[idx] = texId;
    }
    return error == GL_NO_ERROR;
}

void cxOpenGL::UseProgram(glUint program)
{
    if(currProg != program){
        glUseProgram(program);
        currProg = program;
    }
}

void cxOpenGL::DelProgram(glUint program)
{
    glDeleteProgram(program);
    currProg = -1;
}

void cxOpenGL::DelShader(glUint shader)
{
    glDeleteShader(shader);
}

cxTextureId cxOpenGL::GenTexture()
{
    cxTextureId texId = 0;
    glGenTextures(1, (GLuint *)&texId);
    GLenum error = glGetError();
    if(error != GL_NO_ERROR){
        CX_ERROR("gl gen textures error:%d",error);
    }
    return texId;
}

typedef void (*cxOpenGLInfoFunc)(GLuint program, GLenum pname, GLint *params);

typedef void (*cxOpenGLLogFunc)(GLuint program, GLsizei bufsize, GLsizei *length, GLchar *infolog);

static char* cxShaderLogFunc(GLuint object, cxOpenGLInfoFunc infoFunc, cxOpenGLLogFunc logFunc)
{
    GLint logLength = 0, writeBytes = 0;
    infoFunc(object, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength < 1){
        return NULL;
    }
    char *logBytes = (char*)malloc(logLength + 1);
    logFunc(object, logLength, &writeBytes, logBytes);
    logBytes[logLength] = '\0';
    return logBytes;
}

static char *cxShaderGetLog(glUint shader)
{
    return cxShaderLogFunc(shader, (cxOpenGLInfoFunc)&glGetShaderiv, (cxOpenGLLogFunc)&glGetShaderInfoLog);
}

static char *cxProgramGetLog(glUint program)
{
    return cxShaderLogFunc(program, (cxOpenGLInfoFunc)&glGetProgramiv, (cxOpenGLLogFunc)&glGetProgramInfoLog);
}

void cxOpenGL::Scissor(const cxRect4F &box)
{
    glEnable(GL_SCISSOR_TEST);
    glScissor(box.x, box.y, box.w, box.h);
}

void cxOpenGL::Scissor()
{
    glDisable(GL_SCISSOR_TEST);
}

void cxOpenGL::Clear()
{
    glClear(GL_COLOR_BUFFER_BIT);
}

cxOpenGL *cxOpenGL::Instance()
{
    return cxCore::One<cxOpenGL>(&instance);
}

cxOpenGL::cxOpenGL()
{
    shaders = cxHash::Alloc();
    kmGLSetCurrentContext();
    for(cxInt i=0;i<MAX_TEXTURES;i++){
        currTexId[i] = -1;
    }
    currProg = -1;
}

cxOpenGL::~cxOpenGL()
{
    shaders->Release();
    kmGLClearCurrentContext();
}

void cxOpenGL::Set3DProject(const cxSize2F &winsiz)
{
    cxFloat zeye = winsiz.h/1.1566f;
    kmGLMatrixMode(KM_GL_PROJECTION);
    kmGLLoadIdentity();
    kmMat4 project;
    kmMat4PerspectiveProjection(&project, 60.0f, winsiz.w/winsiz.h, 10.0f, zeye+winsiz.h/2.0f);
    kmGLMultMatrix(&project);
    kmMat4 lookAt;
    kmVec3 eye;
    kmVec3 center;
    kmVec3 up;
    kmVec3Fill(&eye, 0.0f, 0.0f, zeye);
    kmVec3Fill(&center, 0.0f, 0.0f, 0.0f);
    kmVec3Fill(&up, 0.0f, 1.0f, 0.0f);
    kmMat4Identity(&lookAt);
    kmMat4LookAt(&lookAt, &eye, &center, &up);
    kmGLMultMatrix(&lookAt);
    kmGLMatrixMode(KM_GL_MODELVIEW);
    kmGLLoadIdentity();
}

void cxOpenGL::MatrixMode(kmGLEnum mode)
{
    kmGLMatrixMode(mode);
}

void cxOpenGL::LoadIdentity(void)
{
    kmGLLoadIdentity();
}

void cxOpenGL::LoadMatrix(const cxMatrixF &mat4)
{
    kmGLLoadMatrix(&mat4.mat4);
}

const cxMatrixF &cxOpenGL::ModelView()
{
    kmGLGetMatrix(KM_GL_MODELVIEW,  &modelview.mat4);
    return modelview;
}

const cxMatrixF &cxOpenGL::ModelProject()
{
    kmGLGetMatrix(KM_GL_PROJECTION, &modelproject.mat4);
    return modelproject;
}

void cxOpenGL::MultMatrix(const cxMatrixF &mat4)
{
    kmGLMultMatrix(&mat4.mat4);
}

void cxOpenGL::Push()
{
    kmGLPushMatrix();
}

void cxOpenGL::Pop()
{
    kmGLPopMatrix();
}

void cxShader::BindAttrib(glUint idx,cchars name)
{
    glBindAttribLocation(program, idx, name);
}

void cxShader::SetUniform(glUint loc, const cxMatrixF &value) const
{
    glUniformMatrix4fv(loc, 1, GL_FALSE, (const GLfloat *)value.mat4.mat);
}

void cxShader::SetUniform(glUint loc,cxFloat value) const
{
    glUniform1f(loc, value);
}

void cxShader::SetUniform(glUint loc,cxInt value) const
{
    glUniform1f(loc, value);
}

void cxShader::SetUniform(glUint loc,const cxPoint2F &value) const
{
    glUniform2f(loc, value.x, value.y);
}

void cxShader::SetUniform(glUint loc,const cxPoint3F &value) const
{
    glUniform3f(loc, value.x, value.y, value.z);
}

void cxShader::SetUniform(glUint loc,const cxSize2F &value) const
{
    glUniform2f(loc, value.w, value.h);
}

void cxShader::SetUniform(glUint loc,const cxColor4F &value) const
{
    glUniform4f(loc, value.r, value.g, value.b, value.a);
}

const glUint cxShader::ID() const
{
    return program;
}

const glUint cxShader::GetUniform(cchars name) const
{
    return glGetUniformLocation(program, name);
}

glUint cxShader::CompileVSHShader(cchars source)
{
    CX_ASSERT(cxStr::IsOK(source), "source error");
    GLint status = 0;
    CX_ASSERT(source != NULL,"shader sources NULL");
    const GLchar *sources[] = {source};
    glUint shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(shader, sizeof(sources)/sizeof(*sources), sources, NULL);
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if(status == GL_FALSE){
        GLsizei length = 0;
        glGetShaderiv(shader, GL_SHADER_SOURCE_LENGTH, &length);
        GLchar* src = (GLchar *)malloc(sizeof(GLchar) * length);
        glGetShaderSource(shader, length, NULL, src);
        CX_ERROR("Failed to compile shader:%s\n", src);
        char *log = cxShaderGetLog(shader);
        CX_ERROR("compile shader error:%s",log);
        free(log);
        free(src);
        abort();
    }
    return shader;
}

glUint cxShader::CompileFSHShader(cchars source)
{
    CX_ASSERT(cxStr::IsOK(source), "source error");
    GLint status = 0;
    CX_ASSERT(source != NULL,"shader sources NULL");
    const GLchar *sources[] = {source};
    glUint shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(shader, sizeof(sources)/sizeof(*sources), sources, NULL);
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if(status == GL_FALSE){
        GLsizei length = 0;
        glGetShaderiv(shader, GL_SHADER_SOURCE_LENGTH, &length);
        GLchar* src = (GLchar *)malloc(sizeof(GLchar) * length);
        glGetShaderSource(shader, length, NULL, src);
        CX_ERROR("Failed to compile shader:%s\n", src);
        char *log = cxShaderGetLog(shader);
        CX_ERROR("compile shader error:%s",log);
        free(log);
        free(src);
        abort();
    }
    return shader;
}

cxShader *cxShader::Init(cchars avs,cchars afs)
{
    GLint status;
    vsh = CompileVSHShader(avs);
    fsh = CompileFSHShader(afs);
    program = glCreateProgram();
    glAttachShader(program, vsh);
    glAttachShader(program, fsh);
    if(!InitAttrib()){
        CX_ERROR("init shader attrib error");
        return this;
    }
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if(status == GL_FALSE){
        char *log = cxProgramGetLog(program);
        CX_ERROR("link program error:%s",log);
        free(log);
        return this;
    }
    glUseProgram(program);
    ump = GetUniform(CX_UNIFORM_UMP);
    umv = GetUniform(CX_UNIFORM_UMV);
    if(!InitUniform()){
        CX_ERROR("init uniform attrib error");
        return this;
    }
    return this;
}

TDrawBuffer::TDrawBuffer()
{
    gl = cxOpenGL::Instance();
    isinit = false;
}

TDrawBuffer::~TDrawBuffer()
{
    glDeleteBuffers(2, &vboid[0]);
    glDeleteVertexArrays(1, &vaoid);
}

void TDrawBuffer::DrawTriangles(cxUInt mode,const cxRenderFArray &renders)
{
    cxInt num = renders.Size();
    if(num == 0){
        return;
    }
    cxULong start = (cxULong)renders.Buffer();
    glEnableVertexAttribArray(cxVertexAttribPosition);
    GLvoid *poff = (GLvoid *)(start + offsetof(cxRenderF, vertices));
    glVertexAttribPointer(cxVertexAttribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(cxRenderF), poff);
    
    glEnableVertexAttribArray(cxVertexAttribColor);
    GLvoid *coff = (GLvoid *)(start + offsetof(cxRenderF, colors));
    glVertexAttribPointer(cxVertexAttribColor, 4, GL_FLOAT, GL_FALSE, sizeof(cxRenderF), coff);
    
    glEnableVertexAttribArray(cxVertexAttribTexcoord);
    GLvoid *toff = (GLvoid *)(start + offsetof(cxRenderF, coords));
    glVertexAttribPointer(cxVertexAttribTexcoord, 2, GL_FLOAT, GL_FALSE, sizeof(cxRenderF), toff);
    glDrawArrays(mode, 0, num);
}

void TDrawBuffer::InitTrianglesVBO(const cxRenderFArray &renders,const cxIndicesArray &indices)
{
    glGenBuffers(2, &vboid[0]);
    if(gl->support_GL_OES_vertex_array_object){
        glGenVertexArrays(1, &vaoid);
        glBindVertexArray(vaoid);
        glBindBuffer(GL_ARRAY_BUFFER, vboid[0]);
        glBufferData(GL_ARRAY_BUFFER, renders.BufferBytes(), renders.Buffer(), GL_DYNAMIC_DRAW);
        //vertices
        glEnableVertexAttribArray(cxVertexAttribPosition);
        glVertexAttribPointer(cxVertexAttribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(cxRenderF), (GLvoid*)offsetof(cxRenderF, vertices));
        //colors
        glEnableVertexAttribArray(cxVertexAttribColor);
        glVertexAttribPointer(cxVertexAttribColor, 4, GL_FLOAT, GL_FALSE, sizeof(cxRenderF), (GLvoid*)offsetof(cxRenderF, colors));
        //tex coords
        glEnableVertexAttribArray(cxVertexAttribTexcoord);
        glVertexAttribPointer(cxVertexAttribTexcoord, 2, GL_FLOAT, GL_FALSE, sizeof(cxRenderF), (GLvoid*)offsetof(cxRenderF, coords));
        //
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboid[1]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.BufferBytes(), indices.Buffer(), GL_DYNAMIC_DRAW);
        
        glBindVertexArray(0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }else{
        glBindBuffer(GL_ARRAY_BUFFER, vboid[0]);
        glBufferData(GL_ARRAY_BUFFER, renders.BufferBytes(), renders.Buffer(), GL_DYNAMIC_DRAW);
        
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboid[1]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.BufferBytes(), indices.Buffer(), GL_DYNAMIC_DRAW);
        
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
}

void TDrawBuffer::DrawTrianglesVBO(const cxRenderFArray &renders,const cxIndicesArray &indices)
{
    if(!isinit){
        InitTrianglesVBO(renders, indices);
        isinit = true;
    }
    if(renders.Size() == 0){
        return;
    }
    //
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboid[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.DataBytes(), indices.Buffer(), GL_DYNAMIC_DRAW);
    //
    glBindBuffer(GL_ARRAY_BUFFER, vboid[0]);
    glBufferData(GL_ARRAY_BUFFER, renders.DataBytes(), renders.Buffer(), GL_DYNAMIC_DRAW);
    //
    if(gl->support_GL_OES_vertex_array_object){
        glBindVertexArray(vaoid);
        glDrawElements(GL_TRIANGLES, indices.Size(), GL_UNSIGNED_SHORT, NULL);
        glBindVertexArray(0);
    }else{
        //vertices
        glEnableVertexAttribArray(cxVertexAttribPosition);
        glVertexAttribPointer(cxVertexAttribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(cxRenderF), (GLvoid*)offsetof(cxRenderF, vertices));
        //colors
        glEnableVertexAttribArray(cxVertexAttribColor);
        glVertexAttribPointer(cxVertexAttribColor, 4, GL_FLOAT, GL_FALSE, sizeof(cxRenderF), (GLvoid*)offsetof(cxRenderF, colors));
        //tex coords
        glEnableVertexAttribArray(cxVertexAttribTexcoord);
        glVertexAttribPointer(cxVertexAttribTexcoord, 2, GL_FLOAT, GL_FALSE, sizeof(cxRenderF), (GLvoid*)offsetof(cxRenderF, coords));
        //
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboid[1]);
        glDrawElements(GL_TRIANGLES, indices.Size(), GL_UNSIGNED_SHORT, NULL);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

cxRenderState::cxRenderState()
{
    tags = 0;
    blend = BlendFunc::NONE;
    shader = nullptr;
    texture = nullptr;
    type = 0;
}

cxUInt64 cxRenderState::ID() const
{
    cxUInt64 idd = 0;
    cxUInt16 *ptr = (cxUInt16 *)&idd;
    ptr[0] = (cxUInt16)shader->ID();
    ptr[1] = (cxUInt16)texture->ID();
    ptr[2] = (cxUInt16)blend.ID();
    ptr[3] = (cxUInt16)(((type&0xFF)<<8)|(tags&0xFF));
    return idd;
}

void cxRenderState::SetTags(cxByte v)
{
    tags = v;
}

void cxRenderState::Set(BlendFunc b)
{
    blend = b;
}

void cxRenderState::Set(cxStateType f)
{
    type = f;
}

void cxRenderState::Set(cxView *v)
{
    view = v;
}

void cxRenderState::Set(cxShader *s)
{
    shader = s;
}

void cxRenderState::Set(cxTexture *t)
{
    texture = t;
}
BlendFunc BlendFunc::NONE                   = BlendFunc(0, 0);
BlendFunc BlendFunc::ADDITIVE               = BlendFunc(GL_ONE, GL_ONE);
BlendFunc BlendFunc::ALPHA                  = BlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
BlendFunc BlendFunc::PREMULTIPLIED_ALPHA    = BlendFunc(GL_ONE,GL_ONE_MINUS_SRC_ALPHA);
BlendFunc BlendFunc::MULTIPLY               = BlendFunc(GL_SRC_ALPHA, GL_ZERO);
BlendFunc BlendFunc::SCREEN                 = BlendFunc(GL_ONE_MINUS_DST_COLOR,GL_ONE);
BlendFunc BlendFunc::MULTIPLICATIVE         = BlendFunc(GL_DST_COLOR,GL_ONE_MINUS_SRC_ALPHA);
BlendFunc BlendFunc::DODGE                  = BlendFunc(GL_ONE_MINUS_SRC_ALPHA,GL_ONE);

BlendFunc TDrawable::blend = BlendFunc::NONE;

const cxByte BlendFunc::ID() const
{
    if(*this == BlendFunc::ADDITIVE){
        return 1;
    }
    if(*this == BlendFunc::MULTIPLY){
        return 2;
    }
    if(*this == BlendFunc::ALPHA){
        return 3;
    }
    return 0;
}

BlendFunc BlendFunc::To(cchars name)
{
    if(cxStr::IsEqu(name, "NONE")){
        return BlendFunc::NONE;
    }
    if(cxStr::IsEqu(name, "ADDITIVE")){
        return BlendFunc::ADDITIVE;
    }
    if(cxStr::IsEqu(name, "ALPHA")){
        return BlendFunc::ALPHA;
    }
    if(cxStr::IsEqu(name, "PREMULTIPLIED_ALPHA")){
        return BlendFunc::PREMULTIPLIED_ALPHA;
    }
    if(cxStr::IsEqu(name, "MULTIPLY")){
        return BlendFunc::MULTIPLY;
    }
    if(cxStr::IsEqu(name, "SCREEN")){
        return BlendFunc::SCREEN;
    }
    if(cxStr::IsEqu(name, "MULTIPLICATIVE")){
        return BlendFunc::MULTIPLICATIVE;
    }
    if(cxStr::IsEqu(name, "DODGE")){
        return BlendFunc::DODGE;
    }
    return BlendFunc::ALPHA;
}

BlendFunc::BlendFunc(glUint s,glUint d)
{
    src = s;
    dst = d;
}

BlendFunc::BlendFunc()
{
    src = 0;
    dst = 0;
}

cxBool BlendFunc::operator==(const BlendFunc &v) const
{
    return src == v.src && dst == v.dst;
}

TDrawable::TDrawable()
{
    gl = cxOpenGL::Instance();
}

TDrawable::~TDrawable()
{
    
}

BlendFunc &TDrawable::Blend()
{
    return blend;
}

void TDrawable::UseBlend(BlendFunc &b)
{
    if(b == BlendFunc::NONE){
        glDisable(GL_BLEND);
        return;
    }
    if(blend == b){
        return;
    }
    blend = b;
    glEnable(GL_BLEND);
    glBlendFunc(blend.src, blend.dst);
}

void TDrawable::DrawBoxShape(const cxBoxPoint3F &vs,const cxBoxColor4F &cs)
{
    cxObject::gcGet<cxShader>(ColorShader)->Using(nullptr);
    
    glEnableVertexAttribArray(cxVertexAttribPosition);
    glVertexAttribPointer(cxVertexAttribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(cxPoint3F), &vs);
    
    glEnableVertexAttribArray(cxVertexAttribColor);
    glVertexAttribPointer(cxVertexAttribColor, 4, GL_FLOAT, GL_FALSE, sizeof(cxColor4F), &cs);
    
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void TDrawable::DrawLine(const cxLineF &line,const cxColor4F &color)
{
    cxObject::gcGet<cxShader>(ColorShader)->Using(nullptr);
    
    cxPoint3F ps[2];
    ps[0] = cxPoint3F(line.a.x, line.a.y, 0);
    ps[1] = cxPoint3F(line.b.x, line.b.y, 0);
    glEnableVertexAttribArray(cxVertexAttribPosition);
    glVertexAttribPointer(cxVertexAttribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(cxPoint3F), ps);
    
    cxColor4F cs[2];
    cs[0] = color;
    cs[1] = color;
    glEnableVertexAttribArray(cxVertexAttribColor);
    glVertexAttribPointer(cxVertexAttribColor, 4, GL_FLOAT, GL_FALSE, sizeof(cxColor4F), cs);
    
    glDrawArrays(GL_LINES, 0, 2);
}

void TDrawable::DrawBoxLine(const cxBoxPoint3F &vs,const cxBoxColor4F &cs)
{
    cxObject::gcGet<cxShader>(ColorShader)->Using(nullptr);
    
    cxBoxPoint3F bp = vs.ToLoop();
    glEnableVertexAttribArray(cxVertexAttribPosition);
    glVertexAttribPointer(cxVertexAttribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(cxPoint3F), &bp);
    
    cxBoxColor4F bc = cs.ToLoop();
    glEnableVertexAttribArray(cxVertexAttribColor);
    glVertexAttribPointer(cxVertexAttribColor, 4, GL_FLOAT, GL_FALSE, sizeof(cxColor4F), &bc);
    
    glDrawArrays(GL_LINE_LOOP, 0, 4);
}

void TDrawable::DrawBoxRender(const cxBoxRender &render,const cxUInt16 *indices)
{
    cxULong start = (cxULong)&render;
    glEnableVertexAttribArray(cxVertexAttribPosition);
    GLvoid *poff = (GLvoid *)(start + offsetof(cxRenderF, vertices));
    glVertexAttribPointer(cxVertexAttribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(cxRenderF), poff);
    
    glEnableVertexAttribArray(cxVertexAttribColor);
    GLvoid *coff = (GLvoid *)(start + offsetof(cxRenderF, colors));
    glVertexAttribPointer(cxVertexAttribColor, 4, GL_FLOAT, GL_FALSE, sizeof(cxRenderF), coff);

    glEnableVertexAttribArray(cxVertexAttribTexcoord);
    GLvoid *toff = (GLvoid *)(start + offsetof(cxRenderF, coords));
    glVertexAttribPointer(cxVertexAttribTexcoord, 2, GL_FLOAT, GL_FALSE, sizeof(cxRenderF), toff);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, indices);
}

void TDrawable::DrawBoxRender(const cxBoxRenderArray &renders,const cxUInt16 *indices)
{
    cxInt num = renders.Size();
    if(num == 0){
        return;
    }
    cxULong start = (cxULong)renders.Buffer();
    glEnableVertexAttribArray(cxVertexAttribPosition);
    GLvoid *poff = (GLvoid *)(start + offsetof(cxRenderF, vertices));
    glVertexAttribPointer(cxVertexAttribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(cxRenderF), poff);
    
    glEnableVertexAttribArray(cxVertexAttribColor);
    GLvoid *coff = (GLvoid *)(start + offsetof(cxRenderF, colors));
    glVertexAttribPointer(cxVertexAttribColor, 4, GL_FLOAT, GL_FALSE, sizeof(cxRenderF), coff);
    
    glEnableVertexAttribArray(cxVertexAttribTexcoord);
    GLvoid *toff = (GLvoid *)(start + offsetof(cxRenderF, coords));
    glVertexAttribPointer(cxVertexAttribTexcoord, 2, GL_FLOAT, GL_FALSE, sizeof(cxRenderF), toff);
    
    glDrawElements(GL_TRIANGLES, 6 * num, GL_UNSIGNED_SHORT, indices);
}

void TDrawable::DrawTriangles(const cxRenderFArray &renders)
{
    cxInt num = renders.Size();
    if(num == 0){
        return;
    }
    cxULong start = (cxULong)renders.Buffer();
    glEnableVertexAttribArray(cxVertexAttribPosition);
    GLvoid *poff = (GLvoid *)(start + offsetof(cxRenderF, vertices));
    glVertexAttribPointer(cxVertexAttribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(cxRenderF), poff);
    
    glEnableVertexAttribArray(cxVertexAttribColor);
    GLvoid *coff = (GLvoid *)(start + offsetof(cxRenderF, colors));
    glVertexAttribPointer(cxVertexAttribColor, 4, GL_FLOAT, GL_FALSE, sizeof(cxRenderF), coff);
    
    glEnableVertexAttribArray(cxVertexAttribTexcoord);
    GLvoid *toff = (GLvoid *)(start + offsetof(cxRenderF, coords));
    glVertexAttribPointer(cxVertexAttribTexcoord, 2, GL_FLOAT, GL_FALSE, sizeof(cxRenderF), toff);
    glDrawArrays(GL_TRIANGLE_FAN, 0, num);
}

CX_CPP_END

