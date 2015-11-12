//
//  cxTriangles.h
//  cxEngineCore
//
//  Created by xuhua on 7/16/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#ifndef cxEngineCore_cxTriangles_h
#define cxEngineCore_cxTriangles_h

#include "cxSprite.h"

CX_CPP_BEGIN

class cxTriangles : public cxSprite
{
public:
    CX_DECLARE(cxTriangles);
protected:
    explicit cxTriangles();
    virtual ~cxTriangles();
protected:
    void OnDirty();
    void OnRender(cxRender *render,const cxMatrixF &model);
private:
    cxRenderFArray rs;
    cxIndicesArray is;
    cxStateType type;       //default TriangleFan
public:
    void SetType(cxStateType v);
    cxRenderFArray &Renders();
    cxIndicesArray &Indices();
    cxTriangles *Append(const cxRenderF &p);
    cxInt FromPolygons(const cxRenderFArray &vs);
};

CX_CPP_END

#endif


