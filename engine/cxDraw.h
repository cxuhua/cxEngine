//
//  cxDraw.h
//  cxEngineCore
//
//  Created by xuhua on 6/25/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#ifndef cxEngineCore_cxDraw_h
#define cxEngineCore_cxDraw_h

#include <core/cxObject.h>
#include <math/cxBoxRender.h>
#include "cxOpenGL.h"
#include "cxShader.h"

CX_CPP_BEGIN

struct cxDraw
{
    cxRenderState state;
    cxBoxRenderArray render;    //Render
    cxRect4F clipbox;           //ClipOn ClipOff
    ~cxDraw();
    cxDraw();
    cxBool Render(cxBoxRender &r,const cxMatrixF &m,const cxRenderState &s,cxUInt flags);
    cxBool Render(cxBoxRenderArray &r,const cxMatrixF &m,const cxRenderState &s,cxUInt flags);
    cxBool Clip(cxStateType type,const cxBox4F &box);
    cxUInt64 ID() const;
    cxStateType Type() const;
    void Using();
};

class cxDrawArray : private std::vector<cxDraw>
{
public:
    explicit cxDrawArray();
    virtual ~cxDrawArray();
private:
    cxInt number;
public:
    cxInt Capacity() const;
    cxInt Size() const;
    cxInt Inc(cxInt inc);
    cxDraw &Inc();
    void Append(cxInt num);
    void Append(const cxDraw &v);
    cxDraw &At(cxInt idx);
    void Clear();
};

CX_CPP_END

#endif


