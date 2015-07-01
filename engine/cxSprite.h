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
    virtual void OnDirty();
    virtual void OnRender(cxRender *render,const cxMatrixF &model);
public:
    
    static cxSprite *Create(cchars fkey);
    
    cxSprite *SetTexKey(cchars fmt,...);
    cxSprite *SetTexCoord(cxTexCoord *v);
    cxSprite *SetFlipX(cxBool v);
    cxSprite *SetFlipY(cxBool v);
    cxSprite *SetFlip(cxBool x,cxBool y);
    cxSprite *SetPixel(const cxBox4F &v);
    cxTexture *Texture() const;
    cxSprite *SetTexture(cchars key);
    cxSprite *SetTexture(cxTexture *ptex);
    cxTexCoord *TexCoord() const;
};

CX_CPP_END

#endif


