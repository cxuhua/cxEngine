//
//  cxRender.h
//  cxEngineCore
//
//  Created by xuhua on 6/24/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#ifndef cxEngineCore_cxRender_h
#define cxEngineCore_cxRender_h

#include <core/cxArray.h>
#include <math/cxBoxRender.h>
#include "cxOpenGL.h"
#include "cxDraw.h"

CX_CPP_BEGIN

#define MAX_TRIANGLES   10240

class cxRender : public cxObject,public TDrawBuffer
{
public:
    CX_DECLARE(cxRender);
protected:
    explicit cxRender();
    virtual ~cxRender();
private:
    cxDraw *prev;
    cxUInt64 cid;
    cxInt vsc;
    cxInt vdc;
    cxInt max;
    cxIndicesArray indices;
    cxRenderFArray renders;
    cxDrawArray draws;
public:
    void MergeDraw(const cxDraw &draw);
    cxRenderFArray &Renders();
    void Draw();
    void Init();
    //cxSprite
    cxDraw &Render(cxBoxRender &r,const cxMatrixF &m,const cxRenderState &s,cxUInt flags);
    //cxAtlas
    cxDraw &Render(cxBoxRenderArray &rs,const cxMatrixF &m,const cxRenderState &s,cxUInt flags);
    cxDraw &Render(cxRenderFArray &vs,const cxMatrixF &m,const cxRenderState &s,cxUInt flags);
    cxDraw &Render(cxRenderFArray &vs,cxIndicesArray &is,const cxMatrixF &m,const cxRenderState &s,cxUInt flags);
    //clip subviews
    void Clip(cxStateType type,const cxBox4F &box);
    //
    void DrawAllRenders(cxDraw *draw);
};

CX_CPP_END

#endif


