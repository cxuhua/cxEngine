//
//  cxRender.cpp
//  cxEngineCore
//
//  Created by xuhua on 6/24/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//


#include "cxEngine.h"
#include "cxShader.h"
#include "cxRender.h"

CX_CPP_BEGIN

CX_IMPLEMENT(cxRender);

cxRender::cxRender()
{
    Init();
    max = MAX_TRIANGLES;
    indices = new cxUInt16[max*6];
    for(cxInt i=0; i<max;i++){
        indices[i*6+0]=i*4+0;
        indices[i*6+1]=i*4+1;
        indices[i*6+2]=i*4+2;
        indices[i*6+3]=i*4+3;
        indices[i*6+4]=i*4+2;
        indices[i*6+5]=i*4+1;
    }
    triangles.Append(max);
    renders.Append(max);
    draws.Append(max);
}

cxRender::~cxRender()
{
    delete []indices;
}

void cxRender::Render(cxBoxRender &r,const cxMatrixF &m,const cxRenderState &s,cxUInt flags)
{
    cxInt num = draws.Size();
    cxDraw &draw = draws.At(num);
    if(draw.Render(r, m, s, flags)){
        draws.Inc(1);
    }
}

void cxRender::Render(cxBoxRenderArray &rs,const cxMatrixF &m,const cxRenderState &s,cxUInt flags)
{
    cxInt num = draws.Size();
    cxDraw &draw = draws.At(num);
    if(draw.Render(rs, m, s, flags)){
        draws.Inc(1);
    }
}

void cxRender::Render(cxRenderFArray &rs,const cxMatrixF &m,const cxRenderState &s,cxUInt flags)
{
    cxInt num = draws.Size();
    cxDraw &draw = draws.At(num);
    if(draw.Render(rs, m, s, flags)){
        draws.Inc(1);
    }
}

void cxRender::DrawAllRenders(cxDraw *draw)
{
    if(draw == nullptr){
        return;
    }
    prev = nullptr;
    draw->Using();
    vdc ++;
    if(draw->Type() == cxRenderState::BoxRender){
        vsc += renders.Size();
        DrawBoxRender(renders, indices);
        renders.Clear();
        return;
    }
    if(draw->Type() == cxRenderState::Triangles){
        vsc += triangles.Size();
        DrawTriangles(triangles);
        triangles.Clear();
        return;
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
    draws.Clear();
    renders.Clear();
    triangles.Clear();
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
            gl->Scissor(draw.clipbox);
            continue;
        }
        if(type == cxRenderState::ClipOff){
            DrawAllRenders(prev);
            gl->Scissor();
            continue;
        }
        cxUInt64 id = draw.ID();
        if(cid != id){
            cid = id;
            DrawAllRenders(prev);
        }
        prev = &draw;
        if(type == cxRenderState::BoxRender){
            renders.Append(draw.render);
            continue;
        }
        if(type == cxRenderState::Triangles){
            triangles.Append(draw.triangles);
            continue;
        }
    }
    DrawAllRenders(prev);
}

CX_CPP_END

