//
//  cxTriangles.cpp
//  cxEngineCore
//
//  Created by xuhua on 7/16/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#include "cxFrames.h"
#include "cxTriangles.h"

CX_CPP_BEGIN

CX_IMPLEMENT(cxTriangles);

cxTriangles::cxTriangles()
{
    cframes = nullptr;
    cidx = -1;
    coords = cxArray::Alloc();
    type = cxRenderState::TrianglesVBO;
}

cxTriangles::~cxTriangles()
{
    coords->Release();
    cxObject::release(&cframes);
}

cxTriangles *cxTriangles::Clear()
{
    cidx = -1;
    rs.Clear();
    is.Clear();
    coords->Clear();
    return this;
}

const cxFrames *cxTriangles::Frames()
{
    return cframes;
}

cxTriangles *cxTriangles::SetFrames(cxInt idx)
{
    CX_ASSERT(cframes != nullptr, "frames not set");
    cidx = idx;
    const cxArray *layers = cframes->Layers(idx);
    CX_ASSERT(layers != nullptr, "frames null");
    SetTexture(cframes->Texture());
    SetCoords(layers, cframes->Map());
    return this;
}

cxTriangles *cxTriangles::SetFrames(const cxFrames *frames,cxInt idx)
{
    if(frames == nullptr){
        Clear();
        return this;
    }
    if(frames == cframes && cidx == idx){
        return this;
    }
    cxObject::swap(&cframes, frames);
    SetFrames(idx);
    return this;
}

void cxTriangles::SetCoords(const cxArray *acoords,const cxFrameMap *map)
{
    CX_ASSERT(acoords != nullptr && map != nullptr, "coords or map args error");
    CX_ASSERT(!Size().IsZero(), "size not set");
    rs.Clear();
    is.Clear();
    coords->Clear();
    cxBox4F box = Box();
    cxBool fx = FlipX();
    cxBool fy = FlipY();
    cxColor4F color = Color();
    for(cxInt i = 0;i < map->num;i++){
        cxInt idx = map->values[i];
        cxPoint2F off = map->off[i];
        cxTexCoord *coord = acoords->At(idx)->To<cxTexCoord>();
        if(coord->IsEmpty()){
            continue;
        }
        if(!coord->TrimmedTriangles(color , box, off, fx, fy)){
            continue;
        }
        if(!OnCoord(idx, coord)){
            continue;
        }
        rs.Append(coord->rts);
        is.Append(coord->ats,is.Size());
        coords->Append(coord);
    }
}


void cxTriangles::SetType(cxStateType v)
{
    type = v;
}

void cxTriangles::OnDirty()
{
    if(IsDirtyMode(DirtyModeColor)){
        rs.MulColor(Color());
    }
}

cxRenderFArray &cxTriangles::Renders()
{
    return rs;
}

cxIndicesArray &cxTriangles::Indices()
{
    return is;
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

