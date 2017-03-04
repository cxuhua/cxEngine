//
//  cxSprite.h
//  cxEngineCore
//
//  Created by xuhua on 6/17/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#ifndef cxEngineCore_cxSprite_h
#define cxEngineCore_cxSprite_h

#include <math/cxBoxRender.h>
#include "cxView.h"
#include "cxTexture.h"

CX_CPP_BEGIN

class cxFrameMap;
class cxSprite : public cxView
{
public:
    CX_DECLARE(cxSprite);
protected:
    explicit cxSprite();
    virtual ~cxSprite();
private:
    cxBoxRender box;
    cxTexCoord *coord;
    cxBox4F pixel;
    cxBool flipx;
    cxBool flipy;
    cxTexture *texture;
protected:
    void OnDirty();
    void OnRender(cxRender *render,const cxMatrixF &model);
public:
    virtual void SetTexture(const cxTexture *ptex);
    virtual void SetCoords(const cxArray *acoords,const cxFrameMap *map);
    virtual cxTexCoord *TexCoord() const;
public:
    
    static cxSprite *Create(cchars fkey,...);

    cxSprite *UseTextureSize();
    
    cxBoxRender &BoxRender();
    
    cxSprite *SetTexKey(const cxStr *v);
    cxSprite *SetTexKey(cchars fmt,...);
    cxSprite *SetTexCoord(const cxTexCoord *v);
    
    const cxBool FlipX() const;
    const cxBool FlipY() const;
    
    const cxBox4F Pixel() const;
    
    cxSprite *SetFlipX(cxBool v);
    cxSprite *SetFlipY(cxBool v);
    cxSprite *SetFlip(cxBool x,cxBool y);
    
    cxSprite *SetPixel(const cxBox4F &v);
    
    cxTexture *Texture() const;
    cxSprite *SetTexture(cchars key);
    cxSprite *SetTexture(const cxStr *tkey);
    
    const cxBoxCoord2F &BoxCoord() const;
};

CX_CPP_END

#endif


