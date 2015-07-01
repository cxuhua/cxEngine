//
//  cxTexture.h
//  cxEngineCore
//
//  Created by xuhua on 6/12/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#ifndef cxEngineCore_cxTexture_h
#define cxEngineCore_cxTexture_h

#include <math/cxSize2F.h>
#include <math/cxRect4F.h>
#include <core/cxHash.h>
#include <math/cxBoxCoord2F.h>
#include <math/cxBox4F.h>
#include <math/cxBoxPoint3F.h>
#include <core/cxJson.h>

CX_CPP_BEGIN

typedef int32_t cxTextureId;

class cxTexture;
class cxTexCoord : public cxObject
{
public:
    CX_DECLARE(cxTexCoord);
protected:
    explicit cxTexCoord();
    virtual ~cxTexCoord();
private:
    cxTexture *texture;
    cxBoxCoord2F coord;
public:
    cxBool rotated;
    cxBool trimmed;
    cxRect4F frame;
    cxSize2F sourceSize;
    cxRect4F spriteSourceSize;
public:
    void SetTexture(cxTexture *v);
    cxTexture *Texture();
    
    const cxSize2F &Size() const;
    cxBox4F Trimmed(const cxBoxPoint3F &box,const cxSize2F &size,cxBool flipx,cxBool flipy) const;
    cxBox4F &Trimmed(cxBox4F &vbox,const cxSize2F &size,cxBool flipx,cxBool flipy) const;
    const cxBoxCoord2F *BoxCoord(const cxBox4F &pixel,cxBool flipx,cxBool flipy);
};

struct cxTextureParams
{
    uint32_t minFilter;
    uint32_t magFilter;
    uint32_t wrapS;
    uint32_t wrapT;
    static const cxTextureParams Default;
    static const cxTextureParams Repeat;
};

struct cxTextAttr;
class cxTexture : public cxObject
{
public:
    CX_DECLARE(cxTexture);
public:
    static const cxInt PNG = 1;
    static const cxInt PVR = 2;
    static const cxInt PKM = 3;
protected:
    explicit cxTexture();
    virtual ~cxTexture();
private:
    cxBool success;
    cxHash *atlas;
    cxTextureId texId;
    cxSize2F size;
public:
    cxTexCoord *At(cchars key) const;
    cxTexCoord *At(const cxStr *key) const;
    
    const cxTextureId ID() const;
    
    cxBool IsSuccess() const;
    const cxSize2F &Size() const;
    
    cxTexture *GenTexture();
    const cxTexture *Bind() const;
    const cxTexture *SetParams(const cxTextureParams &params) const;
public:
    cxTexture *Atlas(const cxStr *data);
    cxTexture *FromPNG(const cxStr *data);
    cxTexture *FromJPG(const cxStr *data);
    cxTexture *FromPVR(const cxStr *data);
    cxTexture *FromPKM(const cxStr *data);
    cxTexture *Atlas(cchars file);
    cxTexture *FromPNG(cchars file);
    cxTexture *FromJPG(cchars file);
    cxTexture *FromPVR(cchars file);
    cxTexture *FromPKM(cchars file);
    cxTexture *FromTXT(const cxStr *txt,const cxTextAttr &attr);
    cxTexture *From(cchars file);
    cxTexture *From(cxInt type,const cxStr *data);
};

CX_CPP_END

#endif


