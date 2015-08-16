//
//  cxDraw.cpp
//  cxEngineCore
//
//  Created by xuhua on 6/25/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#include "cxEngine.h"
#include "cxDraw.h"

CX_CPP_BEGIN

cxDraw::cxDraw()
{
    
}

cxBool cxDraw::Render(cxBoxRender &r,const cxMatrixF &m,const cxRenderState &s,cxUInt flags)
{
    cxBox4F wbox = s.view->ParentBox();
    state = s;
    render.Clear();
    cxBoxRender b = r * m;
    if(wbox.Contain(b.ToBoxPoint())){
        render.Append(b);
    }
    return render.Size() > 0;
}

cxBool cxDraw::Render(cxBoxRenderArray &rs,const cxMatrixF &m,const cxRenderState &s,cxUInt flags)
{
    cxBox4F wbox = s.view->ParentBox();
    state = s;
    render.Clear();
    for(cxInt i=0; i < rs.Size(); i++){
        cxBoxRender b = rs.At(i) * m;
        if(!wbox.Contain(b.ToBoxPoint())){
            continue;
        }
        render.Append(b);
    }
    return render.Size() > 0;
}

cxBool cxDraw::Render(cxRenderFArray &rs,const cxMatrixF &m,const cxRenderState &s,cxUInt flags)
{
    state = s;
    triangles.Clear();
    for(cxInt i=0; i < rs.Size(); i++){
        cxRenderF p = rs.At(i) * m;
        triangles.Append(p);
    }
    return triangles.Size() > 0;
}

cxBool cxDraw::Clip(cxStateType type,const cxBox4F &box)
{
    cxSize2F size = cxEngine::Instance()->WinSize();
    state.Set(type);
    clipbox.x = box.X() + size.w/2.0f;
    clipbox.y = box.Y() + size.h/2.0f;
    clipbox.w = box.W();
    clipbox.h = box.H();
    return true;
}

void cxDraw::Using()
{
    TDrawable::UseBlend(state.blend);
    state.shader->Using(state.view);
    state.texture->Bind();
}

cxDraw::~cxDraw()
{
    
}

cxUInt64 cxDraw::ID() const
{
    return state.ID();
}

cxStateType cxDraw::Type() const
{
    return state.type;
}

cxDrawArray::cxDrawArray()
{
    number = 0;
}

cxDrawArray::~cxDrawArray()
{
    
}

void cxDrawArray::Clear()
{
    number = 0;
}

cxInt cxDrawArray::Size() const
{
    return number;
}

cxInt cxDrawArray::Capacity() const
{
    return (cxInt)size();
}

cxInt cxDrawArray::Inc(cxInt inc)
{
    number+=inc;
    return number;
}

cxDraw &cxDrawArray::Inc()
{
    cxDraw &rv = At(number);
    number++;
    return rv;
}

void cxDrawArray::Append(cxInt num)
{
    for(cxInt i=0;i<num;i++){
        Append(cxDraw());
    }
}

void cxDrawArray::Append(const cxDraw &v)
{
    if(size() > number){
        At(number) = v;
    }else{
        push_back(v);
    }
    number++;
}

cxDraw &cxDrawArray::At(cxInt idx)
{
    return at(idx);
}

CX_CPP_END

