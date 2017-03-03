//
//  cxAtlas.cpp
//  cxEngineCore
//
//  Created by xuhua on 6/27/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#include "cxFrames.h"
#include "cxAtlas.h"

CX_CPP_BEGIN

CX_IMPLEMENT(cxAtlas);

cxAtlas::cxAtlas()
{
    coords = nullptr;
    cframes = nullptr;
    cidx = -1;
    isscale9 = false;
}

cxAtlas::~cxAtlas()
{
    cxObject::release(&coords);
    cxObject::release(&cframes);
}

cxAtlas *cxAtlas::Create(const cxFrames *frames)
{
    cxAtlas *rv = cxAtlas::Create();
    rv->SetBlend(frames->Blend());
    rv->SetSize(frames->Size() * frames->Scale());
    rv->SetFrames(frames);
    return rv;
}

void cxAtlas::SetCoords(const cxArray *acoords,const cxFrameMap *map)
{
    CX_ASSERT(acoords != nullptr && map != nullptr, "coords or map args error");
    CX_ASSERT(!Size().IsZero(), "size not set");
    SetCapacity(map->num);
    for(cxInt i = 0;i < map->num;i++){
        cxInt idx = map->values[i];
        //get map tex
        cxTexCoord *coord = acoords->At(idx)->To<cxTexCoord>();
        if(coord->IsEmpty()){
            continue;
        }
        CX_ASSERT(!coord->HasTriangles(), "error,coord has triangles");
        //trimmed box
        cxBoxPoint3F bp = coord->Trimmed(BoxPoint(), FlipX(), FlipY());
        if(bp.Size().IsZero()){
            continue;
        }
        //add render
        cxBoxRender &render = renders.Inc();
        render.SetVertices(bp);
        render.SetColor(Color());
        //get coord box
        const cxBoxCoord2F &tbox = coord->BoxCoord(Pixel(), FlipX(), FlipY());
        render.SetCoords(tbox);
    }
    cxObject::swap(&coords, acoords);
}

cxInt cxAtlas::TexCoordSize() const
{
    return (coords == nullptr)?0:coords->Size();
}

cxTexCoord *cxAtlas::TexCoord(cxInt idx) const
{
    CX_ASSERT(coords != nullptr && idx >= 0 && idx < coords->Size(), "idx error");
    return coords->At(idx)->To<cxTexCoord>();
}

const cxFrames *cxAtlas::GetFrames()
{
    return cframes;
}

cxAtlas *cxAtlas::SetFramesIdx(cxInt idx)
{
    CX_ASSERT(cframes != nullptr, "frames not set");
    cidx = idx;
    const cxArray *layers = cframes->Layers(idx);
    CX_ASSERT(layers != nullptr, "frames null");
    SetTexture(cframes->Texture());
    SetCoords(layers, cframes->Map());
    return this;
}

cxAtlas *cxAtlas::SetFrames(const cxFrames *frames,cxInt idx)
{
    if(frames == nullptr){
        Clear();
        return this;
    }
    if(frames == cframes && cidx == idx){
        return this;
    }
    cxObject::swap(&cframes, frames);
    SetFramesIdx(idx);
    return this;
}

void cxAtlas::updateScale9()
{
    cxTexCoord *coord = TexCoord();
    if(coord == nullptr){
        renders.Clear();
        return;
    }
    const cxColor4F &color = Color();
    if(IsDirtyMode(DirtyModeSize|DirtyModeTexture)){
        Clear();
        cxSize2F size = Size();
        cxBox4F box = scalebox;
        //
        if(size.w < scalebox.l + scalebox.r){
            box.l = (scalebox.l / scalebox.W()) * size.w;
            box.r = (scalebox.r / scalebox.W()) * size.w;
        }
        if(size.h < scalebox.t + scalebox.b){
            box.t = (scalebox.t / scalebox.H()) * size.h;
            box.b = (scalebox.b / scalebox.H()) * size.h;
        }
        box.l = !cxFloatIsOK(box.l)?0:box.l;
        box.r = !cxFloatIsOK(box.r)?0:box.r;
        box.t = !cxFloatIsOK(box.t)?0:box.t;
        box.b = !cxFloatIsOK(box.b)?0:box.b;
        //
        cxSize2F texsiz = Texture()->Size();
        cxSize2F tsize = coord->FrameSize();
        
        cxFloat tx = coord->rotated?(coord->frame.y/texsiz.h):(coord->frame.x/texsiz.w);
        cxFloat ty = coord->rotated?((coord->frame.x+coord->frame.h)/texsiz.w):(coord->frame.y/texsiz.h);
        cxFloat tw = coord->rotated?(coord->frame.w/texsiz.h):(coord->frame.w/texsiz.w);
        cxFloat th = coord->rotated?(coord->frame.h/texsiz.w):(coord->frame.h/texsiz.h);
        
        cxFloat txs[]={0.0f, box.l/tsize.w, (tsize.w - box.r)/tsize.w, 1.0f};
        cxFloat tys[]={0.0f, box.t/tsize.h, (tsize.h - box.b)/tsize.h, 1.0f};
        for(int i=0; i < 4; i++){
            txs[i] = coord->rotated?(txs[i] * tw + tx):(txs[i] * tw + tx);
            tys[i] = coord->rotated?(ty - tys[i] * th):(tys[i] * th + ty);
        }
        cxFloat bxs[]={0.0f, box.l, size.w - box.r, size.w};
        cxFloat bys[]={0.0f, box.t, size.h - box.b, size.h};
        
        cxFloat tx1=0,ty1=0,tx2=0,ty2=0;
        cxFloat bx1=0,by1=0,bx2=0,by2=0;
        cxFloat offx=0,offy=0;
        for(cxInt i=0; i < 9;i++){
            tx1 = txs[i%3];
            tx2 = txs[i%3 + 1];
            ty1 = tys[i/3];
            ty2 = tys[i/3 + 1];
            bx1 = bxs[i%3];
            bx2 = bxs[i%3 + 1];
            by1 = bys[i/3];
            by2 = bys[i/3 + 1];
            cxFloat hw = (bx2 - bx1)/2.0f;
            cxFloat hh = (by2 - by1)/2.0f;
            if(cxFloatIsEqual(hw, 0.0f) || cxFloatIsEqual(hh, 0.0f)){
                continue;
            }
            offx = (bx2 - bx1)/2.0f + bx1 - size.w/2.0f;
            offy = (size.h - (by2 - by1))/2.0f - by1;
            cxBoxRender &bp = renders.Inc();
            bp.SetColor(color);
            bp.lb.vertices = cxPoint3F(-hw + offx, -hh + offy, 0.0f);
            bp.rb.vertices = cxPoint3F( hw + offx, -hh + offy, 0.0f);
            bp.lt.vertices = cxPoint3F(-hw + offx,  hh + offy, 0.0f);
            bp.rt.vertices = cxPoint3F( hw + offx,  hh + offy, 0.0f);
            bp.lb.coords = coord->rotated?cxCoord2F(ty2, tx1):cxCoord2F(tx1, ty2);
            bp.rb.coords = coord->rotated?cxCoord2F(ty2, tx2):cxCoord2F(tx2, ty2);
            bp.lt.coords = coord->rotated?cxCoord2F(ty1, tx1):cxCoord2F(tx1, ty1);
            bp.rt.coords = coord->rotated?cxCoord2F(ty1, tx2):cxCoord2F(tx2, ty1);
        }
    }
    if(IsDirtyMode(DirtyModeColor)){
        for(cxInt i=0; i < renders.Size();i++){
            cxBoxRender &bp = renders.At(i);
            bp.SetColor(color);
        }
    }
}

cxAtlas *cxAtlas::Clear()
{
    cidx = -1;
    renders.Clear();
    return this;
}

cxInt cxAtlas::Inc(cxInt inc)
{
    return renders.Inc(inc);
}

cxBoxRender &cxAtlas::Inc()
{
    return renders.Inc();
}

cxInt cxAtlas::Number() const
{
    return renders.Size();
}

cxInt cxAtlas::Capacity() const
{
    return renders.Capacity();
}

cxBoxRender &cxAtlas::At(cxInt idx)
{
    return renders.At(idx);
}

cxAtlas *cxAtlas::SetCapacity(cxInt cap)
{
    if(renders.Capacity() < cap){
        renders.Append(cap - renders.Capacity());
    }
    renders.Clear();
    return this;
}

void cxAtlas::OnDirty()
{
    if(isscale9){
        updateScale9();
        return;
    }
    if(IsDirtyMode(DirtyModeColor)){
        renders.MulColor(Color());
    }
}

void cxAtlas::OnRender(cxRender *render,const cxMatrixF &model)
{
    if(renders.Size() == 0 || Texture() == nullptr){
        return;
    }
    state.Set(cxRenderState::TrianglesVBO);
    state.Set(Texture());
    state.Set(Blend());
    render->Render(renders, model, State(), Flags());
}

cxAtlas *cxAtlas::SetScale9(const cxBox4F &sb)
{
    if(scalebox != sb){
        isscale9 = true;
        scalebox = sb;
        SetCapacity(9);
        SetDirty(DirtyModeTexture);
    }
    return this;
}

CX_CPP_END

