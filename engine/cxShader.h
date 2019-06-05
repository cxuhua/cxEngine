//
//  cxShader.h
//  cxEngineCore
//
//  Created by xuhua on 6/12/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#ifndef cxEngineCore_cxShader_h
#define cxEngineCore_cxShader_h

#include <core/cxObject.h>
#include <math/cxSize2F.h>
#include "cxOpenGL.h"

CX_CPP_BEGIN

enum {
    cxVertexAttribNone,
    cxVertexAttribPosition,
    cxVertexAttribColor,
    cxVertexAttribTexcoord,
    cxVertexAttribMax,
};

#define CX_ATTR_COLOR       "aColor"
#define CX_ATTR_POSITION    "aPosition"
#define CX_ATTR_TEXCOORD    "aTexcoord"
#define CX_UNIFORM_UMP      "uMP"
#define CX_UNIFORM_COLOR    "uColor"

class cxStr;
struct cxMatrixF;
struct cxPoint2F;
struct cxPoint3F;
struct cxColor4F;
class cxShader : public cxObject
{
public:
    CX_DECLARE(cxShader);
public:
    const static cchars Color;
    const static cchars Default;
    const static cchars Gray;
protected:
    explicit cxShader();
    virtual ~cxShader();
    
    virtual cxBool InitUniform();
    virtual cxBool InitAttrib();
    
    glUint program;
    glUint vsh;
    glUint fsh;
    glUint ump;
    glUint ucolor;
private:
    glUint CompileVSHShader(cchars source);
    glUint CompileFSHShader(cchars source);
    glUint CompileShader(GLenum type,cchars source);
public:
    
    void BindAttrib(glUint idx,cchars name);
    
    void SetModelProject(const cxMatrixF &value) const;
    
    void SetUniform(glUint loc, cxInt value) const;
    void SetUniform(glUint loc, cxFloat value) const;
    void SetUniform(glUint loc, const cxSize2F &value) const;
    void SetUniform(glUint loc, const cxPoint2F &value) const;
    void SetUniform(glUint loc, const cxPoint3F &value) const;
    void SetUniform(glUint loc, const cxColor4F &value) const;
    void SetUniform(glUint loc, const cxMatrixF &value) const;
    
    const glUint GetUniform(cchars name)  const;
    
    const glUint ID() const;
    
    void Free();
    //use file init
    cxShader *Init(cchars vsf,cchars fsf);
    
    virtual void Using(cxView *pview) const;
    void SetUColor(const cxColor4F &v);
};
//
class cxColorShader : public cxShader
{
public:
    CX_DECLARE(cxColorShader);
protected:
    explicit cxColorShader();
    virtual ~cxColorShader();
    virtual cxBool InitAttrib();
    void Using(cxView *pview) const;
};
//
class cxEffectShader : public cxShader
{
public:
    CX_DECLARE(cxEffectShader);
private:
    glUint time;
protected:
    explicit cxEffectShader();
    virtual ~cxEffectShader();
    void Using(cxView *pview) const;
    cxBool InitUniform();
};

CX_CPP_END

#endif


