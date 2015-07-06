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

#define MAX_TRIANGLES   65536/6

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
    cxUInt16 *indices;
    cxBoxRenderArray renders;
    cxDrawArray draws;
    cxFloat fpsTime;
    void debug();
public:
    void Draw();
    void Init();
    //cxSprite
    void Render(cxBoxRender &r,const cxMatrixF &m,const cxRenderState &s,cxUInt flags);
    //cxAtlas
    void Render(cxBoxRenderArray &rs,const cxMatrixF &m,const cxRenderState &s,cxUInt flags);
    //clip subviews
    void Clip(cxStateType type,const cxBox4F &box);
    void DrawRenders(cxDraw *draw);
};

CX_CPP_END

#endif


