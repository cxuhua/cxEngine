//
//  cxTriangles.h
//  cxEngineCore
//
//  Created by xuhua on 7/16/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#ifndef cxEngineCore_cxTriangles_h
#define cxEngineCore_cxTriangles_h

#include "cxFrames.h"
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
public:
    void SetCoords(const cxArray *acoords,const cxFrameMap *map);
private:
    cxRenderFArray rs;
    cxIndicesArray is;
    cxStateType type;       //default TriangleFan
    
    cxFrames *cframes;
    cxInt cidx;
    cxArray *coords;
public:
    void SetType(cxStateType v);
    cxRenderFArray &Renders();
    cxIndicesArray &Indices();

    cxTriangles *SetFrames(cxInt idx);
    cxTriangles *SetFrames(const cxFrames *frames,cxInt idx=0);
    const cxFrames *Frames();
    
    cxTriangles *Clear();
};

CX_CPP_END

#endif


