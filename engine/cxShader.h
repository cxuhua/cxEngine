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
#define CX_UNIFORM_UMV      "uMV"

class cxView;
class cxStr;
struct cxMatrixF;
struct cxPoint2F;
struct cxPoint3F;
struct cxColor4F;
class cxShader : public cxObject
{
public:
    CX_DECLARE(cxShader);
protected:
    explicit cxShader();
    virtual ~cxShader();
    
    virtual const cxStr *Vertex() const;
    virtual const cxStr *Fragment() const;
    virtual cxBool InitUniform();
    virtual cxBool InitAttrib();
    
    glUint program;
    glUint vsh;
    glUint fsh;
    glUint ump;
    glUint umv;
private:
    glUint CompileVSHShader(const cxStr *source);
    glUint CompileFSHShader(const cxStr *source);
public:
    
    void BindAttrib(glUint idx,cchars name);
    
    void SetModelProject(const cxMatrixF &value) const;
    void SetModelView(const cxMatrixF &value) const;
    
    void SetUniform(glUint location,cxFloat value) const;
    void SetUniform(glUint location,const cxSize2F &value) const;
    void SetUniform(glUint location,const cxPoint2F &value) const;
    void SetUniform(glUint location,const cxPoint3F &value) const;
    void SetUniform(glUint location,const cxColor4F &value) const;
    void SetUniform(glUint location, const cxMatrixF &value) const;
    
    const glUint GetUniform(cchars name)  const;
    
    const glUint ID() const;
    
    void Free();
    cxShader *Init();
    virtual void Using() const;
};
//
class cxColorShader : public cxShader
{
public:
    CX_DECLARE(cxColorShader);
protected:
    explicit cxColorShader();
    virtual ~cxColorShader();
    virtual const cxStr *Vertex() const;
    virtual const cxStr *Fragment() const;
    virtual cxBool InitAttrib();
    void Using() const;
};

CX_CPP_END

#endif


