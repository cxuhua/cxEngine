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
    indices.Init(max*6);
    renders.Append(max);
    draws.Append(max);
}

cxRender::~cxRender()
{
    
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

void cxRender::MergeDraw(const cxDraw &draw)
{
    cxInt start = renders.Size();
    for(cxInt i=0;i<draw.indices.Size();i++){
        indices.Append(draw.indices.At(i) + start);
    }
    renders.Append(draw.renders);
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
        if(type == cxRenderState::TrianglesVBO){
            MergeDraw(draw);
            continue;
        }
        if(type == cxRenderState::Triangles){
            MergeDraw(draw);
            continue;
        }
        if(type == cxRenderState::TriangleFan){
            renders.Append(draw.renders);
            DrawAllRenders(&draw);
            continue;
        }
        if(type == cxRenderState::TriangleStrip){
            renders.Append(draw.renders);
            DrawAllRenders(&draw);
            continue;
        }
    }
    DrawAllRenders(prev);
}

CX_CPP_END

