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
    fpsTime = 0;
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

void cxRender::DrawRenders(cxDraw *draw)
{
    if(draw == nullptr){
        return;
    }
    draw->Using();
    vsc += renders.Size();
    vdc ++;
    DrawBoxRender(renders, indices);
    renders.Clear();
    prev = nullptr;
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
    vsc = 0;
    vdc = 0;
    prev= nullptr;
    cid = 0;
}

void cxRender::debug()
{
    fpsTime += cxEngine::Instance()->Delta();
    if(fpsTime >= 1.0f){
        cxLabel *label = cxEngine::Instance()->Window()->DebugLabel();
        cxInt fps = cxEngine::Instance()->FPS();
        label->SetText("%d,%d,%d",vdc,vsc,fps);
        fpsTime = 0.0f;
    }
}

void cxRender::Draw()
{
    for(cxInt i=0; i < draws.Size();i++){
        cxDraw &draw = draws.At(i);
        cxStateType type = draw.Type();
        if(type == cxRenderState::ClipOn){
            DrawRenders(prev);
            gl->Scissor(draw.clipbox);
            continue;
        }
        if(type == cxRenderState::ClipOff){
            DrawRenders(prev);
            gl->Scissor();
            continue;
        }
        if(type == cxRenderState::BoxRender){
            cxUInt64 id = draw.ID();
            if(cid != id){cid = id;DrawRenders(prev);}
            prev = &draw;
            renders.Append(draw.render);
            continue;
        }
    }
    DrawRenders(prev);
    #ifndef NDEBUG
    debug();
    #endif
}

CX_CPP_END

