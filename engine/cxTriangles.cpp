//
//  cxTriangles.cpp
//  cxEngineCore
//
//  Created by xuhua on 7/16/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#include "cxTriangles.h"

CX_CPP_BEGIN

CX_IMPLEMENT(cxTriangles);

cxTriangles::cxTriangles()
{
    type = cxRenderState::TriangleFan;
}

cxTriangles::~cxTriangles()
{
    
}

void cxTriangles::SetType(cxStateType v)
{
    type = v;
}

void cxTriangles::OnDirty()
{
    
}

cxRenderFArray &cxTriangles::Renders()
{
    return rs;
}

cxIndicesArray &cxTriangles::Indices()
{
    return is;
}

cxTriangles *cxTriangles::Append(const cxRenderF &p)
{
    rs.Append(p);
    return this;
}

void cxTriangles::OnRender(cxRender *render,const cxMatrixF &model)
{
    if(rs.Size() < 3 || Texture() == nullptr){
        return;
    }
    state.Set(type);
    state.Set(Texture());
    state.Set(Blend());
    if(type == cxRenderState::TrianglesVBO){
        render->Render(Renders(), Indices(), model, State(), Flags());
    }else{
        render->Render(Renders(), model, State(), Flags());
    }
}

CX_CPP_END

