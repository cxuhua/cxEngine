//
//  cxTriangles.cpp
//  cxEngineCore
//
//  Created by xuhua on 7/16/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//


#include <poly2tri/poly2tri.h>
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

static inline cxUInt16 getVerticeIdx(p2t::Point *ps,cxInt num,p2t::Point *p)
{
    for(cxInt i=0;i<num;i++){
        if(&ps[i] == p){
            return i;
        }
    }
    CX_ASSERT(false, "idx miss");
    return 0;
}

cxInt cxTriangles::FromPolygons(const cxRenderFArray &vs)
{
    cxInt num = vs.Size();
    if(num < 3){
        return 0;
    }
    Renders().Clear();
    Indices().Clear();
    type = cxRenderState::TrianglesVBO;
    //append triangles vertices
    p2t::Point *ps = new p2t::Point[num];
    std::vector<p2t::Point*> polylines;
    for(cxInt i=0;i<num;i++){
        const cxRenderF &vp = vs.At(i);
        ps[i] = p2t::Point(vp.vertices.x,vp.vertices.y);
        polylines.push_back(&ps[i]);
        Renders().Append(vp);
    }
    //compute triangles indices
    p2t::CDT cdt(polylines);
    cdt.Triangulate();
    std::vector<p2t::Triangle*> triangles = cdt.GetTriangles();
    for(cxInt i=0;i<triangles.size();i++){
        p2t::Triangle *tv = triangles.at(i);
        
        p2t::Point *p0 = tv->GetPoint(0);
        cxUInt16 i0 = getVerticeIdx(ps, num, p0);
        Indices().Append(i0);
        
        p2t::Point *p1 = tv->GetPoint(1);
        cxUInt16 i1 = getVerticeIdx(ps, num, p1);
        Indices().Append(i1);
        
        p2t::Point *p2 = tv->GetPoint(2);
        cxUInt16 i2 = getVerticeIdx(ps, num, p2);
        Indices().Append(i2);
    }
    delete []ps;
    return 0;
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

