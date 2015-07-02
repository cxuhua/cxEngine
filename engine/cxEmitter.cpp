//
//  cxEmitter.cpp
//  cxEngineCore
//
//  Created by xuhua on 6/28/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//


#include "cxEmitter.h"

CX_CPP_BEGIN

CX_IMPLEMENT(cxEmitter);

cxEmitter::cxEmitter()
{
    axisspin = cxPoint3F::AxisX;
    emitcounter = 0;
    isActive = true;
    rate = 100;
    todir = false;
    units = nullptr;
    type = cxEmitterGravity;
    
    life = cxFloatRange(3, 3);
    startsize = cxFloatRange(50, 0);
    endsize = startsize;
    angle = cxFloatRange(0, 360);
    gravity = cxPoint2F(0, 1000);
    speed = cxFloatRange(0, 1000);
    startspin = cxFloatRange(2000, 0);
    endspin = cxFloatRange(0, 0);
    startcolor = cxColor4F::WHITE;
    endcolor = cxColor4F(1, 1, 1, 0.3);
    todir = true;
}

cxEmitter::~cxEmitter()
{
    delete []units;
}

cxEmitter *cxEmitter::SetRate(cxFloat v)
{
    rate = v;
    emitcounter = 0;
    return this;
}

cxEmitter *cxEmitter::SetType(cxEmitterType v)
{
    type = v;
    return this;
}

cxEmitter *cxEmitter::SetPosRange(const cxPoint2FRange &v)
{
    position = v;
    return this;
}

cxEmitter *cxEmitter::SetLifeRange(const cxFloatRange &v)
{
    life = v;
    return this;
}

cxEmitter *cxEmitter::SetAngleRange(const cxFloatRange &v)
{
    angle = v;
    return this;
}

cxEmitter *cxEmitter::SetStartSize(const cxFloatRange &v)
{
    startsize = v;
    return this;
}

cxEmitter *cxEmitter::SetEndSize(const cxFloatRange &v)
{
    endsize = v;
    return this;
}

cxEmitter *cxEmitter::SetStartColor(const cxColor4FRange &v)
{
    startcolor = v;
    return this;
}

cxEmitter *cxEmitter::SetEndColor(const cxColor4FRange &v)
{
    endcolor = v;
    return this;
}

cxEmitter *cxEmitter::SetStartSpin(const cxFloatRange &v)
{
    startspin = v;
    return this;
}

cxEmitter *cxEmitter::SetEndSpin(const cxFloatRange &v)
{
    endspin = v;
    return this;
}

cxEmitter *cxEmitter::SetAxisSpin(const cxPoint3F &v)
{
    axisspin = v;
    return this;
}

cxEmitter *cxEmitter::Create(cxInt max)
{
    cxEmitter *rv = cxEmitter::Create();
    rv->SetCapacity(max);
    rv->units = new cxEmitterUnit[max];
    return rv;
}

void cxEmitter::initEmitterUnit(cxEmitterUnit *unit)
{
    unit->position = position.ToValue();
    unit->life = life.ToValue();
    cxFloat speedv = speed.ToValue();
    cxFloat anglev = cxDegreesToRadians(angle.ToValue());
    cxColor4F scolor    = startcolor.ToValue();
    cxColor4F ecolor    = endcolor.ToValue();
    unit->color         = scolor;
    unit->deltacolor    = ecolor - scolor;
    
    cxFloat ssize       = startsize.ToValue();
    cxFloat esize       = endsize.ToValue();
    unit->size          = ssize;
    unit->deltasize     = (esize - ssize) / unit->life;
    
    cxFloat sspin = cxDegreesToRadians(startspin.ToValue());
    cxFloat espin = cxDegreesToRadians(endspin.ToValue());
    unit->rotation = sspin;
    unit->deltarotation = (espin - sspin) / unit->life;
    
    if(type == cxEmitterGravity){
        unit->dir = cxPoint2F(cosf(anglev),sinf(anglev)) * speedv;
        unit->radaccel = radaccel.ToValue();
        unit->tanaccel = tanaccel.ToValue();
        if(todir)unit->rotation = -unit->dir.Angle();
    }else if(type == cxEmitterRadial){
        cxFloat sradius = startradius.ToValue();
        cxFloat eradius = endradius.ToValue();
        unit->radius = sradius;
        if(eradius == -1){
            unit->deltaradius = 0;
        }else{
            unit->deltaradius = (eradius - sradius) / unit->life;
        }
        unit->angle = anglev;
        unit->degreespers = cxDegreesToRadians(rotatepers.ToValue());
    }
}

void cxEmitter::addEmitterUnit()
{
    cxInt num = Number();
    if(num < Capacity()){
        initEmitterUnit(&units[num]);
        Inc();
    }
}

void cxEmitter::unitToBoxPoint3F(cxEmitterUnit *unit,cxBoxPoint3F *vq)
{
    cxFloat sizeh = unit->size/2;
    cxPoint3F pos = cxPoint3F(unit->position.x,unit->position.y,0.0f);
    cxFloat l = -sizeh;
    cxFloat b = -sizeh;
    cxFloat r = +sizeh;
    cxFloat t = +sizeh;
    if(this->todir){
        cxPoint3F axis =  cxPoint3F(unit->dir.x, unit->dir.y, 0);
        axisspin = axis.Normalize();
    }
    if (unit->rotation){
        cxMatrixF mat4;
        mat4.InitRotation(axisspin, -unit->rotation);
        vq->lb = cxPoint3F(l, b, 0) * mat4 + pos;
        vq->rb = cxPoint3F(r, b, 0) * mat4 + pos;
        vq->rt = cxPoint3F(r, t, 0) * mat4 + pos;
        vq->lt = cxPoint3F(l, t, 0) * mat4 + pos;
    }else{
        vq->lb.x = pos.x + l;
        vq->lb.y = pos.y + b;
        vq->rb.x = pos.x + r;
        vq->rb.y = pos.y + b;
        vq->lt.x = pos.x + l;
        vq->lt.y = pos.y + t;
        vq->rt.x = pos.x + r;
        vq->rt.y = pos.y + t;
    }
}

void cxEmitter::unitSetBox(cxEmitterUnit *unit,cxInt idx)
{
    cxBoxRender &box = At(idx);
    box.SetColor(unit->color);
    
    cxBoxPoint3F vbp;
    unitToBoxPoint3F(unit, &vbp);
    box.SetVertices(vbp);
    
    cxTexCoord *coord = TexCoord();
    const cxBoxCoord2F *bt = coord->BoxCoord(0.0f, false, false);
    box.SetCoords(*bt);
}

void cxEmitter::OnUpdate(cxFloat dt)
{
    if(Capacity() == 0){
        return;
    }
    //active add item
    if(isActive){
        cxFloat ratev = 1.0f/rate;
        if(Number() < Capacity()){
            emitcounter += dt;
        }
        while(Number() < Capacity() && emitcounter > ratev){
            addEmitterUnit();
            emitcounter -= ratev;
        }
    }
    cxInt index = 0;
    cxInt num = Number();
    while(index < num){
        cxEmitterUnit *p = &units[index];
        if(p->life < 0){
            if(index == (num - 1)){
                Inc(-1);
                break;
            }
            units[index] = units[num - 1];
            p = &units[index];
            num = Inc(-1);
        }
        p->life -= dt;
        if(type == cxEmitterGravity){
            cxPoint2F tmp;
            cxPoint2F radial;
            if(p->position.x || p->position.y){
                radial = p->position;
                radial.Normalize();
            }
            cxPoint2F tangential = radial;
            radial *= p->radaccel;
            //compute tangential
            cxFloat newy = tangential.x;
            tangential.x = -tangential.y;
            tangential.y = newy;
            tangential *= p->tanaccel;
            //compute position
            tmp = radial + tangential;
            tmp += gravity;
            tmp *= dt;
            p->dir +=  tmp;
            tmp = p->dir * dt;
            p->position += tmp;
        }else{
            p->angle += p->degreespers * dt;
            p->radius += p->deltaradius * dt;
            p->position.x = -cosf(p->angle) * p->radius;
            p->position.y = -sinf(p->angle) * p->radius;
        }
        // color
        p->color += p->deltacolor * dt;
        //size
        p->size = CX_MAX(0, p->size + p->deltasize * dt);
        //angle
        p->rotation += (p->deltarotation * dt);
        unitSetBox(p, index);
        index ++;
    }
}

CX_CPP_END

