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
    
}

cxTriangles::~cxTriangles()
{
    
}

void cxTriangles::OnDirty()
{
    
}

cxRenderFArray &cxTriangles::Points()
{
    return rs;
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
    state.Set(cxRenderState::Triangles);
    state.Set(Texture());
    state.Set(Blend());
    render->Render(rs, model, State(), Flags());
}

CX_CPP_END

