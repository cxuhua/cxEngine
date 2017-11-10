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
#include <math/cxRenderF.h>
#include <core/cxJson.h>

CX_CPP_BEGIN

typedef cxInt32 cxTextureId;

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
    void initTriangles(const cxJson *avts,const cxJson *auvs,const cxJson *aats);
public:
    //atrs
    cxBool rotated;
    cxBool trimmed;
    cxRect4F frame;
    cxSize2F sourceSize;
    cxRect4F spriteSourceSize;
    cxPoint2F pivot;
    //renders
    cxRenderFArray rts;
    //box
    cxBoxPoint3F box;
    cxBoxCoord2F coord;
    //vertices
    cxPoint2FArray vts;
    //verticesUV
    cxPoint2FArray uvs;
    //triangles
    cxIndicesArray ats;
public:
    cxBool HasTriangles();
    void ParseTriangles(const cxJson *item);
    void SetTexture(cxTexture *v);
    cxTexture *Texture();
    const cxSize2F FrameSize() const;
    const cxBool IsEmpty() const;
    const cxSize2F &Size() const;
    cxBool TrimmedBox(const cxBox4F &vbox, const cxBox4F &pixel, cxPoint2F off, cxBool flipx,cxBool flipy);
    cxBool TrimmedTriangles(const cxColor4F &color,const cxBox4F &vbox, cxPoint2F off, cxBool flipx,cxBool flipy);
    cxBool TrimmedCoord(const cxBox4F &pixel,cxBool flipx,cxBool flipy);
    const cxBoxCoord2F &FlipCoord(const cxBoxCoord2F &ov,cxBool flipx,cxBool flipy);
};

struct cxTextureParams
{
    cxUInt32 minFilter;
    cxUInt32 magFilter;
    cxUInt32 wrapS;
    cxUInt32 wrapT;
    static const cxTextureParams Default;
    static const cxTextureParams Repeat;
};

struct cxTextAttr;
class cxTexture : public cxObject
{
public:
    CX_DECLARE(cxTexture);
public:
    static const cxInt RAW = 1;
    static const cxInt PNG = 2;
    static const cxInt PVR = 3;
    static const cxInt PKM = 4;
    static const cxInt TXT = 5;
    static const cxInt ATLAS_MAXRECTS   = 1;
    static const cxInt ATLAS_POLYGON    = 2;
protected:
    explicit cxTexture();
    virtual ~cxTexture();
private:
    cxInt type;
    cxInt atlasType;
    cxBool success;
    cxHash *coords;
    cxTextureId texId;
    cxSize2F size;
    cxTexture *parseFrames(const cxStr *data);
public:
    cxInt CoordCount() const;
    cxTexCoord *At(cchars key) const;
    cxTexCoord *At(const cxStr *key) const;
    
    const cxTextureId ID() const;
    
    cxBool IsSuccess() const;
    const cxSize2F &Size() const;
    
    cxTexture *GenTexture();
    cxTexture *Bind();
    cxTexture *SetParams(const cxTextureParams &params);
    cxInt Type() const;
public:
    
    cxTexture *FromRGB(cchars data,cxInt width,cxInt height);
    cxTexture *UpdateRGB(cchars data);
    cxTexture *FromRGBA(cchars data,cxInt width,cxInt height);
    cxTexture *UpdateRGBA(cchars data);
    
    cxTexture *FromLQT(const cxStr *data);
    cxTexture *FromPNG(const cxStr *data);
    cxTexture *FromPVR(const cxStr *data);
    cxTexture *FromPKM(const cxStr *data);
    
    cxTexture *FromLQT(cchars file);
    cxTexture *FromPNG(cchars file);
    cxTexture *FromPVR(cchars file);
    cxTexture *FromPKM(cchars file);
    
    cxTexture *FromTXT(const cxStr *txt,const cxTextAttr &attr,cxUInt64 *key=nullptr);
    cxTexture *From(cchars file);
    cxTexture *From(cxTextureId name,const cxSize2F &siz);
    cxTexture *From(cxInt type,const cxStr *data);
    cxTexture *From(cchars file,const cxStr *data);
};

CX_CPP_END

#endif


