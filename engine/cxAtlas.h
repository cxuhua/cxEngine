//
//  cxAtlas.h
//  cxEngineCore
//
//  Created by xuhua on 6/27/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#ifndef cxEngineCore_cxAtlas_h
#define cxEngineCore_cxAtlas_h

#include "cxSprite.h"

CX_CPP_BEGIN

class cxAtlas : public cxSprite
{
public:
    CX_DECLARE(cxAtlas);
protected:
    explicit cxAtlas();
    virtual ~cxAtlas();
private:
    cxBool isscale9;        //use scale9 image render
    cxBox4F scalebox;
    void updateScale9();
    cxBoxRenderArray renders;
protected:
    void OnDirty();
    void OnRender(cxRender *render,const cxMatrixF &model);
public:
    cxAtlas *SetCapacity(cxInt cap);
    cxInt Capacity() const;
    
    cxInt Number() const;
    
    cxBoxRender &Inc();
    cxInt Inc(cxInt inc);
    cxBoxRender &At(cxInt idx);
    
    void SetCoords(const cxArray *coords);
    
    cxAtlas *SetScale9(const cxBox4F &sb);
    
    cxAtlas *Clear();
};

CX_CPP_END

#endif


