//
//  cxRender.cpp
//  cxEngineCore
//
//  Created by xuhua on 6/24/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#include <core/cxUtil.h>
#include "cxEngine.h"
#include "cxShader.h"
#include "cxRender.h"

CX_CPP_BEGIN

CX_IMPLEMENT(cxRender);

cxRender::cxRender()
{
    Init();
    max = MAX_TRIANGLES;
    renders.Append(max);
    draws.Append(max);
}

cxRender::~cxRender()
{
    
}

void cxRender::InitBoxInices()
{
    cxInt num = indices.Size()/6;
    for(cxInt i=0; i<num;i++){
        indices.At(i*6+0)=i*4+0;
        indices.At(i*6+1)=i*4+1;
        indices.At(i*6+2)=i*4+2;
        indices.At(i*6+3)=i*4+3;
        indices.At(i*6+4)=i*4+2;
        indices.At(i*6+5)=i*4+1;
    }
}

cxRenderFArray &cxRender::Renders()
{
    return renders;
}

cxDraw &cxRender::Render(cxBoxRender &r,const cxMatrixF &m,const cxRenderState &s,cxUInt flags)
{
    cxInt num = draws.Size();
    cxDraw &draw = draws.At(num);
    if(draw.Render(r, m, s, flags)){
        draws.Inc(1);
    }
    return draw;
}

cxDraw &cxRender::Render(cxBoxRenderArray &rs,const cxMatrixF &m,const cxRenderState &s,cxUInt flags)
{
    cxInt num = draws.Size();
    cxDraw &draw = draws.At(num);
    if(draw.Render(rs, m, s, flags)){
        draws.Inc(1);
    }
    return draw;
}

cxDraw &cxRender::Render(cxRenderFArray &vs,const cxMatrixF &m,const cxRenderState &s,cxUInt flags)
{
    cxInt num = draws.Size();
    cxDraw &draw = draws.At(num);
    if(draw.Render(vs, m, s, flags)){
        draws.Inc(1);
    }
    return draw;
}

cxDraw &cxRender::Render(cxRenderFArray &vs,cxIndicesArray &is,const cxMatrixF &m,const cxRenderState &s,cxUInt flags)
{
    cxInt num = draws.Size();
    cxDraw &draw = draws.At(num);
    if(draw.Render(vs, is, m, s, flags)){
        draws.Inc(1);
    }
    return draw;
}


void cxRender::DrawAllRenders(cxDraw *draw)
{
    if(draw == nullptr){
        return;
    }
    prev = nullptr;
    draw->Using();
    vdc ++;
    switch (draw->Type()) {
        case cxRenderState::BoxRender:{
            vsc += renders.Size();
            InitBoxInices();
            DrawVertexRender(renders, indices);
            renders.Clear();
            indices.Clear();
            break;
        }
        case cxRenderState::TrianglesVBO:{
            vsc += renders.Size();
            DrawVertexRender(renders, indices);
            renders.Clear();
            indices.Clear();
            break;
        }
        case cxRenderState::Triangles:{
            vsc += renders.Size();
            DrawTriangles(GL_TRIANGLES, renders);
            renders.Clear();
            break;
        }
        case cxRenderState::TriangleFan:{
            vsc += renders.Size();
            DrawTriangles(GL_TRIANGLE_FAN, renders);
            renders.Clear();
            break;
        }
        case cxRenderState::TriangleStrip:{
            vsc += renders.Size();
            DrawTriangles(GL_TRIANGLE_STRIP, renders);
            renders.Clear();
            break;
        }
        default:{
            break;
        }
    }
}

void cxRender::Clip(cxStateType type,const cxBox4F &box)
{
    cxInt num = draws.Size();
    cxDraw &draw = draws.At(num);
    if(draw.Clip(type, box)){
        draws.Inc(1);
    }
}

void cxRender::Init()
{
    indices.Clear();
    draws.Clear();
    renders.Clear();
    vsc = 0;
    vdc = 0;
    prev= nullptr;
    cid = 0;
}

void cxRender::Draw()
{
    for(cxInt i=0; i < draws.Size();i++){
        cxDraw &draw = draws.At(i);
        cxStateType type = draw.Type();
        if(type == cxRenderState::ClipOn){
            DrawAllRenders(prev);
            cxOpenGL::Instance()->Scissor(draw.clipbox);
            continue;
        }
        if(type == cxRenderState::ClipOff){
            DrawAllRenders(prev);
            cxOpenGL::Instance()->Scissor();
            continue;
        }
        cxUInt64 id = draw.ID();
        if(cid != id){
            cid = id;
            DrawAllRenders(prev);
        }
        prev = &draw;
        if(type == cxRenderState::BoxRender){
            renders.Append(draw.renders);
            indices.Append(draw.indices);
            continue;
        }
        if(type == cxRenderState::TrianglesVBO){
            renders.Append(draw.renders);
            indices.Append(draw.indices);
            continue;
        }
        if(type == cxRenderState::Triangles){
            renders.Append(draw.renders);
            continue;
        }
        if(type == cxRenderState::TriangleFan){
            renders.Append(draw.renders);
            DrawAllRenders(prev);
            continue;
        }
        if(type == cxRenderState::TriangleStrip){
            renders.Append(draw.renders);
            DrawAllRenders(prev);
            continue;
        }
    }
    DrawAllRenders(prev);
}

CX_CPP_END

