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
    isscale9 = false;
}

cxAtlas::~cxAtlas()
{
    
}

cxAtlas *cxAtlas::SetCoords(const cxArray *coords,const cxFrames *frames)
{
    cxInt size = frames->MapNum();
    SetCapacity(size);
    const cxInt *map = frames->Map();
    for(cxInt i = 0;i < size;i++){
        cxBoxRender &render = renders.Inc();
        cxInt mapIdx = map[i];
        CX_ASSERT(mapIdx < coords->Size(), "map idx error");
        cxTexCoord *coord = coords->At(mapIdx)->To<cxTexCoord>();
        cxBoxPoint3F bp = coord->Trimmed(BoxPoint(), Size(), FlipX(), FlipY());
        const cxBoxCoord2F &tbox = coord->BoxCoord(Pixel(), FlipX(), FlipY());
        render.SetVertices(bp);
        render.SetColor(Color());
        render.SetCoords(tbox);
    }
    return this;
}

cxAtlas *cxAtlas::SetFrames(const cxFrames *frames,cxInt idx)
{
    const cxTimePoint *tp = frames->At(idx);
    const cxArray *coords = tp->Object()->To<cxArray>();
    SetCoords(coords, frames);
    return this;
}

cxAtlas *cxAtlas::SetCoords(const cxArray *coords)
{
    cxInt size = coords->Size();
    SetCapacity(size);
    for(cxInt i = 0;i < size;i++){
        cxBoxRender &render = renders.Inc();
        cxTexCoord *coord = coords->At(i)->To<cxTexCoord>();
        cxBoxPoint3F bp = coord->Trimmed(BoxPoint(), Size(), FlipX(), FlipY());
        const cxBoxCoord2F &tbox = coord->BoxCoord(Pixel(), FlipX(), FlipY());
        render.SetVertices(bp);
        render.SetColor(Color());
        render.SetCoords(tbox);
    }
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
        cxSize2F texsiz = Texture()->Size();
        cxFloat tx = coord->rotated?(coord->frame.y/texsiz.h):(coord->frame.x/texsiz.w);
        cxFloat ty = coord->rotated?((coord->frame.x+coord->frame.h)/texsiz.w):(coord->frame.y/texsiz.h);
        cxFloat tw = coord->frame.w/texsiz.w;
        cxFloat th = coord->frame.h/texsiz.h;
        cxSize2F tsize = cxSize2F(coord->frame.w, coord->frame.h);
        cxFloat txs[]={0.0f, scalebox.l/tsize.w, (tsize.w - scalebox.r)/tsize.w, 1.0f};
        cxFloat tys[]={0.0f, scalebox.t/tsize.h, (tsize.h - scalebox.b)/tsize.h, 1.0f};
        for(int i=0; i < 4; i++){
            txs[i] = coord->rotated?(txs[i] * tw + tx):(txs[i] * tw + tx);
            tys[i] = coord->rotated?(ty - tys[i] * th):(tys[i] * th + ty);
        }
        cxFloat bxs[]={0.0f, scalebox.l, size.w - scalebox.r, size.w};
        cxFloat bys[]={0.0f, scalebox.t, size.h - scalebox.b, size.h};
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
            bp.lb.colors = color;
            bp.lt.colors = color;
            bp.rb.colors = color;
            bp.rt.colors = color;
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
    if(IsDirtyMode(DirtyModeColor) && renders.Size() > 0){
        for(cxInt i=0; i < renders.Size();i++){
            cxBoxRender &bp = renders.At(i);
            bp.lb.colors = color;
            bp.lt.colors = color;
            bp.rb.colors = color;
            bp.rt.colors = color;
        }
    }
}

cxAtlas *cxAtlas::Clear()
{
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
        renders.SetColor(Color());
    }
}

void cxAtlas::OnRender(cxRender *render,const cxMatrixF &model)
{
    if(renders.Size() == 0 || Texture() == nullptr){
        return;
    }
    state.Set(cxRenderState::BoxRender);
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

