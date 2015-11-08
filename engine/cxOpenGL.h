//
//  cxOpenGL.h
//  cxEngineCore
//
//  Created by xuhua on 6/11/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#ifndef cxEngineCore_cxOpenGL_h
#define cxEngineCore_cxOpenGL_h

#include <vector>
#include <core/cxObject.h>
#include <core/cxStr.h>
#include <core/cxHash.h>
#include <math/cxColor4F.h>
#include <math/cxBoxColor4F.h>
#include <math/cxBoxPoint3F.h>
#include <math/cxBoxCoord2F.h>
#include <math/cxLineF.h>
#include <math/cxMatrixF.h>
#include <math/kazmath/GL/matrix.h>
#include <math/cxBoxRender.h>
#include <math/cxRenderF.h>
#include "OpenGL.h"
#include "cxTexture.h"

CX_CPP_BEGIN

#define ColorShader     "cxShader_Color"
#define DefaultShader   "cxShader_Default"
#define GrayShader      "cxShader_Gray"

#define MAX_TEXTURES    32

typedef uint32_t glUint;

class cxView;
class cxShader;
struct cxBox4F;
struct cxRect4F;
struct cxSize2F;
struct cxBoxRenderF;
class cxOpenGL : public cxObject
{
    friend cxCore;
private:
    CX_DECLARE(cxOpenGL);
protected:
    explicit cxOpenGL();
    virtual ~cxOpenGL();
public:
    cxInt maxTextureSize;
    cxInt maxCombinedTextureUnits;
    cxInt maxVertexTextureUnits;
    cxBool support_GL_OES_rgb8_rgba8;
    cxBool support_GL_IMG_texture_npot;
    cxBool support_GL_IMG_texture_compression_pvrtc;
    cxBool support_GL_OES_compressed_ETC1_RGB8_texture;
    cxBool support_GL_OES_vertex_array_object;
    cxBool support_GL_EXT_discard_framebuffer;
    cxBool support_GL_OES_mapbuffer;
    cxBool support_GL_OES_packed_depth_stencil;
    cxBool support_GLES3;
private:
    static cxOpenGL *instance;
    cxTextureId currTexId[MAX_TEXTURES];
    glUint currProg;
    cxHash *shaders;
    cxMatrixF modelproject;
    cxMatrixF modelview;
public:
    
    void SetClearColor(const cxColor4F &clear);
    
    void Push();
    void Pop();
    const cxMatrixF &ModelView();
    const cxMatrixF &ModelProject();
    void MatrixMode(kmGLEnum mode);
    void LoadIdentity(void);
    void LoadMatrix(const cxMatrixF &mat4);
    void MultMatrix(const cxMatrixF &mat4);
    
    void Set3DProject(const cxSize2F &winsiz);
    
    void SetViewport(const cxRect4F &rect);
    void UseProgram(glUint program);
    void DelProgram(glUint program);
    void DelShader(glUint shader);
    
    void Init();
    void Clear();
    
    void Scissor(const cxRect4F &box);
    void Scissor();

    cxTextureId GenTexture();
    cxBool DeleteTexture(cxTextureId texId);
    cxBool BindTexture(cxTextureId texId,cxInt idx = 0);
    void SetTextureParams(const cxTextureParams &params);
    
    static cxOpenGL *Instance();
};

/////////////////////////////////////////////////////////////////////////////////////////////

struct BlendFunc
{
    glUint src;
    glUint dst;
    static BlendFunc ADDITIVE;
    static BlendFunc ALPHA;
    static BlendFunc MULTIPLIED;
    static BlendFunc NONE;
    BlendFunc();
    const cxByte ID() const;
    BlendFunc(glUint s,glUint d);
    cxBool operator==(const BlendFunc &v) const;
};

typedef cxByte cxStateType;

struct cxRenderState
{
    cxStateType type;
    BlendFunc   blend;
    cxShader    *shader;
    cxTexture   *texture;
    cxView      *view;
    
    cxRenderState();
    
    void Set(BlendFunc b);
    void Set(cxShader *s);
    void Set(cxTexture *t);
    void Set(cxStateType f);
    void Set(cxView *v);
    
    cxUInt64 ID() const;
    
    static const cxStateType BoxRender      = 0;
    static const cxStateType ClipOn         = 1;
    static const cxStateType ClipOff        = 2;
    static const cxStateType TrianglesVBO   = 3;    //GL_TRIANGLES VBO
    static const cxStateType Triangles      = 4;    //GL_TRIANGLES
    static const cxStateType TriangleFan    = 5;    //GL_TRIANGLE_FAN
    static const cxStateType TriangleStrip  = 6;    //GL_TRIANGLE_STRIP
};

class TDrawable
{
protected:
    explicit TDrawable();
    virtual ~TDrawable();
private:
    static BlendFunc blend;
public:
    static void UseBlend(BlendFunc &b);
    static BlendFunc &Blend();
protected:
    cxOpenGL *gl;
    void DrawBoxRender(const cxBoxRenderArray &renders,const cxUInt16 *indices);
    void DrawBoxRender(const cxBoxRender &render,const cxUInt16 *indices);
    void DrawBoxLine(const cxBoxPoint3F &vs,const cxBoxColor4F &cs);
    void DrawLine(const cxLineF &line,const cxColor4F &color);
    void DrawBoxShape(const cxBoxPoint3F &vs,const cxBoxColor4F &cs);
    void DrawTriangles(const cxRenderFArray &renders);
};

class TDrawBuffer
{
private:
    cxBool isinit;
    glUint vaoid;
    glUint vboid[2];
protected:
    cxOpenGL *gl;
    explicit TDrawBuffer();
    virtual ~TDrawBuffer();
    void InitVertexBuffer(const cxRenderFArray &renders,const cxIndicesArray &indices);
    void DrawVertexRender(const cxRenderFArray &renders,const cxIndicesArray &indices);
    
    void DrawTriangles(cxUInt mode,const cxRenderFArray &renders);
};

CX_CPP_END

#endif


