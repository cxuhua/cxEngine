//
//  cxCircle.cpp
//  cxEngineCore
//
//  Created by xuhua on 7/16/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//


#include "cxCircle.h"

CX_CPP_BEGIN

CX_IMPLEMENT(cxCircle);

void cxCircle::OnDirty()
{
    if(Texture() == nullptr){
        return;
    }
    cxTriangles::OnDirty();
    cxRenderFArray &rs = Renders();
    rs.Clear();
    
    cxBoxCoord2F tbox = TexCoord()->BoxCoord(Pixel(), FlipX(), FlipY());
    
    cxFloat tw = tbox.rb.u - tbox.lb.u;
    cxFloat th = tbox.rb.v - tbox.rt.v;
    cxFloat tx = tbox.lt.u;
    cxFloat ty = tbox.lt.v;
    cxFloat cu = tx + tw / 2.0f;
    cxFloat cv = ty + th / 2.0f;
    
    cxColor4F color = Color();
    cxSize2F size = Size();
    cxFloat hw = size.w/2.0f;
    cxFloat hh = size.h/2.0f;
    
    cxFloat r = sqrtf(hw * hw + hh * hh);

    rs.Inc().vertices = cxPoint3F(0.0f);
    
    cxFloat angle = start;
    cxFloat ax = percent * 360.0f;
    
    cxBool quit = false;
    cxFloat dv;
    if(angle < 45){
        dv = isreverse?angle:(45 - angle);
    }else{
        dv = 45;
    }
    for(cxInt i = 1; i < rs.Capacity();i++){
        cxFloat ar = kmDegreesToRadians(angle);
        cxFloat x = cosf(ar) * r;
        cxFloat y = sinf(ar) * r;
        if(y > hh){
            y = hh;
        }
        if(y < -hh){
            y = -hh;
        }
        if(x > hw){
            x = hw;
        }
        if(x < -hw){
            x = -hw;
        }
        rs.Inc().vertices = cxPoint3F(x,y,0);
        if(quit)break;
        if(ax - dv < 0.0f){
            dv = ax;
            quit = true;
        }
        if(isreverse){
            angle -= dv;
        }else{
            angle += dv;
        }
        ax -= dv;
        dv = 45;
    }
    for(cxInt j=0; j < rs.Size();j++){
        cxRenderF &nv = rs.At(j);
        cxFloat u = cu + (nv.vertices.x / hw) * (tw / 2.0f);
        cxFloat v = cv - (nv.vertices.y / hh) * (th / 2.0f);
        nv.coords = cxCoord2F(u, v);
        nv.colors = color;
    }
}

cxCircle::cxCircle()
{
    percent = 0.5f;
    isreverse = true;
    start = 90;
    Renders().Init(11);
}

cxCircle::~cxCircle()
{
    
}

cxCircle *cxCircle::SetPercent(cxFloat v)
{
    if(!cxFloatIsEqual(percent, v)){
        percent = v;
        SetDirty(DirtyModeForce);
    }
    return this;
}

cxCircle *cxCircle::SetIsReverse(cxBool v)
{
    if(isreverse != v){
        isreverse = v;
        SetDirty(DirtyModeForce);
    }
    return this;
}

cxCircle *cxCircle::SetStart(cxFloat v)
{
    if(!cxFloatIsEqual(start, v)){
        start = v;
        SetDirty(DirtyModeForce);
    }
    return this;
}

CX_CPP_END

