//
//  cxSprite.cpp
//  cxEngineCore
//
//  Created by xuhua on 6/17/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#include "cxOpenGL.h"
#include "cxSprite.h"

CX_CPP_BEGIN

CX_IMPLEMENT(cxSprite);

cxSprite::cxSprite()
{
    coord = cxTexCoord::Alloc();
    flipx = false;
    flipy = false;
    texture = nullptr;
    SetShader(cxShader::Default);
    SetBlend(BlendFunc::ALPHA);
}

cxSprite::~cxSprite()
{
    coord->Release();
    cxObject::release(&texture);
}

void cxSprite::OnDirty()
{
    if(IsDirtyMode(DirtyModeSize|DirtyModeTexture)){
        cxBoxPoint3F bp = coord->Trimmed(BoxPoint(), Size(), flipx, flipy);
        box.SetVertices(bp);
        box.SetCoords(BoxCoord());
    }
    if(IsDirtyMode(DirtyModeColor)) {
        box.SetColor(BoxColor());
    }
}

cxSprite *cxSprite::Create(cchars fkey,...)
{
    va_list ap;
    va_start(ap, fkey);
    char file[256]={0};
    vsnprintf(file, 256, fkey, ap);
    va_end(ap);
    cxTexture *ptex = cxTexture::Create()->From(file);
    CX_ASSERT(ptex != nullptr, "textue %s miss",file);
    cxSprite *pv = cxSprite::Create();
    pv->SetTexture(ptex);
    return pv;
}

cxSprite *cxSprite::SetFlip(cxBool x,cxBool y)
{
    SetFlipX(x);
    SetFlipY(y);
    return this;
}

const cxBool cxSprite::FlipX() const
{
    return flipx;
}

const cxBool cxSprite::FlipY() const
{
    return flipy;
}

const cxBox4F cxSprite::Pixel() const
{
    return pixel;
}

cxSprite *cxSprite::SetFlipX(cxBool v)
{
    if(flipx != v){
        flipx = v;
        SetDirty(DirtyModeTexture);
    }
    return this;
}

cxSprite *cxSprite::SetFlipY(cxBool v)
{
    if(flipy != v){
        flipy = v;
        SetDirty(DirtyModeTexture);
    }
    return this;
}

cxSprite *cxSprite::SetPixel(const cxBox4F &v)
{
    if(pixel != v){
        pixel = v;
        SetDirty(DirtyModeTexture);
    }
    return this;
}

cxSprite *cxSprite::SetTexCoord(const cxTexCoord *v)
{
    if(coord != v){
        cxObject::swap(&coord, v);
        SetDirty(DirtyModeTexture);
    }
    return this;
}

cxBoxRender &cxSprite::BoxRender()
{
    return box;
}

cxSprite *cxSprite::SetTexKey(const cxStr *v)
{
    return SetTexKey(v->ToString());
}

cxSprite *cxSprite::SetTexKey(cchars fmt,...)
{
    CX_ASSERT(texture != nullptr, "texture not set");
    CX_ASSERT(cxStr::IsOK(fmt), "fmt args error");
    va_list ap;
    va_start(ap, fmt);
    char key[256]={0};
    vsnprintf(key, 256, fmt, ap);
    va_end(ap);
    cxTexCoord *coord = Texture()->At(key);
    if(coord == nullptr){
        CX_WARN("texture key %s miss",key);
        return this;
    }
    SetTexCoord(coord);
    return this;
}

cxInt cxSprite::TexCoordSize() const
{
    return (coord == nullptr)?0:1;
}

cxTexCoord *cxSprite::TexCoord(cxInt idx) const
{
    return coord;
}

const cxBoxCoord2F &cxSprite::BoxCoord() const
{
    return coord->BoxCoord(pixel, flipx, flipy);
}

cxTexture *cxSprite::Texture() const
{
    return texture;
}

cxSprite *cxSprite::SetTexture(const cxTexture *ptex)
{
    CX_ASSERT(ptex != nullptr, "ptex args error");
    if(texture != ptex){
        cxObject::swap(&texture, ptex);
        SetDirty(DirtyModeTexture);
    }
    return this;
}

cxSprite *cxSprite::SetTexture(const cxStr *tkey)
{
    CX_ASSERT(cxStr::IsOK(tkey), "tkey args error");
    return SetTexture(tkey->ToString());
}

cxSprite *cxSprite::SetTexture(cchars key)
{
    CX_ASSERT(cxStr::IsOK(key), "key args error");
    cxTexture *ptex = cxObject::gcGet<cxTexture>(key);
    if(ptex == nullptr){
        CX_WARN("texture %s not exists",key);
        return this;
    }
    SetTexture(ptex);
    return this;
}

cxSprite *cxSprite::UseTextureSize()
{
    if(coord == nullptr){
        return this;
    }
    SetSize(coord->Size());
    return this;
}

void cxSprite::OnRender(cxRender *render,const cxMatrixF &model)
{
    if(Texture() == nullptr){
        return;
    }
    state.Set(cxRenderState::TrianglesVBO);
    state.Set(Texture());
    state.Set(Blend());
    render->Render(box, model, State(), Flags());
}

CX_CPP_END

