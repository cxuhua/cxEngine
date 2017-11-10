//
//  cxEmitter.cpp
//  cxEngineCore
//
//  Created by xuhua on 6/28/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#include <core/cxUtil.h>
#include "cxEmitter.h"

CX_CPP_BEGIN

CX_IMPLEMENT(cxEmitter);

cxEmitter *cxEmitter::Create(cxInt max)
{
    cxEmitter *ret = cxEmitter::Create();
    ret->SetMax(max);
    return ret;
}

cxEmitter::cxEmitter()
{
    autoRemove = false;
    frameTime = 0.1f;
    tkeys = cxArray::Alloc();
    max = 0;
    isinit = false;
    runtime = 0;
    systemtime = 0;
    axisspin = cxPoint3F::AxisZ;
    emitcounter = 0;
    rate = 1;
    todir = false;
    torotate = false;
    units = nullptr;
    type = cxEmitterGravity;
    SetSize(cxSize2F(8, 8));
}

cxEmitter::~cxEmitter()
{
    tkeys->Release();
    delete []units;
}

cxEmitter *cxEmitter::SetFrameTime(cxFloat v)
{
    frameTime = v;
    return this;
}

cxEmitter *cxEmitter::AppendFrameKey(cchars fmt,...)
{
    CX_ASSERT(cxStr::IsOK(fmt), "fmt args error");
    va_list ap;
    va_start(ap, fmt);
    cxStr *key = cxStr::Create()->AppFmt(fmt, ap);
    va_end(ap);
    tkeys->Append(key);
    return this;
}

cxEmitter *cxEmitter::AppendFrameKey(const cxStr *key)
{
    CX_ASSERT(cxStr::IsOK(key), "key args error");
    tkeys->Append(key->Clone());
    return this;
}

cxEmitter *cxEmitter::AppendFrameKeys(const cxArray *keys)
{
    CX_ASSERT(keys != nullptr, "keys args error");
    tkeys->AppendArray(keys);
    return this;
}

cxView *cxEmitter::Clone()
{
    cxEmitter *rv = cxEmitter::Create();
    rv->SetBlend(Blend());
    rv->SetTexture(Texture());
    rv->SetSystemTime(systemtime);
    rv->SetAutoRemove(autoRemove);
    rv->SetToRotate(torotate);
    rv->SetFrameTime(frameTime);
    rv->AppendFrameKeys(tkeys);
    rv->SetMax(max);
    rv->SetType(type);
    rv->SetRate(rate);
    rv->SetPosRange(position);
    rv->SetLifeRange(life);
    rv->SetAngleRange(angle);
    rv->SetStartSize(startsize);
    rv->SetEndSize(endsize);
    rv->SetStartColor(startcolor);
    rv->SetEndColor(endcolor);
    rv->SetStartSpin(startspin);
    rv->SetEndSpin(endspin);
    rv->SetAxisSpin(axisspin);
    //gravity mode
    rv->SetGravity(gravity);
    rv->SetToDir(todir);
    rv->SetSpeed(speed);
    rv->SetTanAccel(tanaccel);
    rv->SetRadAccel(radaccel);
    //radial mode
    rv->SetStartRadius(startradius);
    rv->SetEndRadius(endradius);
    rv->SetRotatePers(rotatepers);
    return rv;
}

cxEmitter *cxEmitter::SetSystemTime(cxFloat v)
{
    runtime = 0;
    systemtime = v;
    emitcounter = 0;
    Clear();
    return this;
}

cxEmitter *cxEmitter::SetGravity(const cxPoint2F &v)
{
    gravity = v;
    return this;
}

cxEmitter *cxEmitter::SetSpeed(const cxFloatRange &v)
{
    speed = v;
    return this;
}

cxEmitter *cxEmitter::SetTanAccel(const cxFloatRange &v)
{
    tanaccel = v;
    return this;
}

cxEmitter *cxEmitter::SetRadAccel(const cxFloatRange &v)
{
    radaccel = v;
    return this;
}

cxEmitter *cxEmitter::SetToRotate(cxBool v)
{
    torotate = v;
    return this;
}

cxEmitter *cxEmitter::SetToDir(cxBool v)
{
    todir = v;
    return this;
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

cxEmitter *cxEmitter::SetStartRadius(const cxFloatRange &v)
{
    startradius = v;
    return this;
}

cxEmitter *cxEmitter::SetEndRadius(const cxFloatRange &v)
{
    endradius = v;
    return this;
}

cxEmitter *cxEmitter::SetRotatePers(const cxFloatRange &v)
{
    rotatepers = v;
    return this;
}

cxEmitter *cxEmitter::SetMax(cxInt v)
{
    max = v;
    return this;
}

void cxEmitter::Init()
{
    CX_ASSERT(max > 0, "max not set");
    SetCapacity(max);
    if(units != nullptr){
        delete []units;
    }
    units = new cxEmitterUnit[max];
}

void cxEmitter::initEmitterUnit(cxEmitterUnit *unit)
{
    unit->idx           = -1;
    unit->position      = position.ToValue();
    unit->life          = life.ToValue();
    unit->time          = unit->life;
    cxFloat speedv      = speed.ToValue();
    cxFloat anglev      = angle.ToRadians();
    cxColor4F scolor    = startcolor.ToValue();
    cxColor4F ecolor    = endcolor.ToValue();
    unit->color         = scolor;
    unit->deltacolor    = ecolor - scolor;
    
    cxFloat ssize       = startsize.ToValue();
    cxFloat esize       = endsize.ToValue();
    unit->size          = ssize;
    unit->deltasize     = (esize - ssize) / unit->life;
    
    cxFloat sspin       = startspin.ToRadians();
    cxFloat espin       = endspin.ToRadians();
    unit->rotation      = sspin;
    unit->deltarotation = (espin - sspin) / unit->life;
    unit->dir           = cxPoint2F(cosf(anglev),sinf(anglev)) * speedv;
    
    if(type == cxEmitterGravity){
        unit->radaccel = radaccel.ToValue();
        unit->tanaccel = tanaccel.ToValue();
        if(torotate && fabsf(unit->deltarotation) > 0){
            todir = false;
        }
        if(todir || torotate){
            unit->rotation = -unit->dir.Angle();
        }
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
        unit->degreespers = rotatepers.ToRadians();
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

void cxEmitter::unitToBoxPoint3F(cxEmitterUnit *unit,cxBoxPoint3F &vq)
{
    cxFloat sizeh = unit->size/2;
    cxPoint3F pos = cxPoint3F(unit->position.x,unit->position.y,0.0f);
    cxFloat l = -sizeh;
    cxFloat b = -sizeh;
    cxFloat r = +sizeh;
    cxFloat t = +sizeh;
    if(torotate){
        axisspin = cxPoint3F(unit->dir.x, unit->dir.y, 0).Normalize();
    }
    if(todir){
        unit->rotation = -unit->dir.Angle();
    }
    if(unit->rotation){
        cxMatrixF mat4;
        mat4.InitRotation(axisspin, -unit->rotation);
        vq.lb = cxPoint3F(l, b, 0) * mat4 + pos;
        vq.rb = cxPoint3F(r, b, 0) * mat4 + pos;
        vq.rt = cxPoint3F(r, t, 0) * mat4 + pos;
        vq.lt = cxPoint3F(l, t, 0) * mat4 + pos;
    }else{
        vq.lb.x = pos.x + l;
        vq.lb.y = pos.y + b;
        vq.rb.x = pos.x + r;
        vq.rb.y = pos.y + b;
        vq.lt.x = pos.x + l;
        vq.lt.y = pos.y + t;
        vq.rt.x = pos.x + r;
        vq.rt.y = pos.y + t;
    }
}

cxEmitter *cxEmitter::SetAutoRemove(cxBool v)
{
    autoRemove = v;
    return this;
}

cxEmitter *cxEmitter::Stop()
{
    runtime+=systemtime;
    return this;
}

void cxEmitter::OnUpdate(cxFloat dt)
{
    if(!isinit){
        Init();
        onStart.Fire(this);
        isinit = true;
    }
    CX_ASSERT(systemtime != 0 && rate != 0, "system time must set");
    if(Capacity() == 0 || Texture() == nullptr){
        return;
    }
    runtime += dt;
    if(runtime < systemtime || systemtime < 0){
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
            cxPoint2F tgv = radial;
            radial *= p->radaccel;
            //compute tangential
            cxFloat newy = tgv.x;
            tgv.x = -tgv.y;
            tgv.y = newy;
            tgv *= p->tanaccel;
            //compute position
            tmp = (radial + tgv + gravity) * dt;
            p->dir +=  tmp;
            tmp = p->dir * dt;
            p->position += tmp;
        }else if(type == cxEmitterRadial){
            p->angle += p->degreespers * dt;
            p->radius += p->deltaradius * dt;
            p->position.x = -cosf(p->angle) * p->radius;
            p->position.y = -sinf(p->angle) * p->radius;
        }else{
            CX_ASSERT(false, "type error");
        }
        // color
        p->color += p->deltacolor * dt;
        //size
        cxFloat vsiz = p->size + p->deltasize * dt;
        p->size = CX_MAX(0, vsiz);
        //angle
        p->rotation += (p->deltarotation * dt);
        cxBoxRender &box = At(index);
        box.SetColor(p->color);
        unitToBoxPoint3F(p, vbp);
        box.SetVertices(vbp);
        OnBoxRender(p, box);
        index ++;
    }
    if(systemtime > 0 && runtime >= systemtime && Number() == 0){
        onExit.Fire(this);
        if(autoRemove){
            Remove();
        }
    }
}

void cxEmitter::OnBoxRender(cxEmitterUnit *unit,cxBoxRender &box)
{
    if(tkeys->Size() ==  0){
        box.SetCoords(BoxCoord());
        return;
    }
    cxTexture *ptex = Texture();
    if(ptex->CoordCount() < tkeys->Size()){
        CX_WARN("texture coord count < tkeys count");
        return;
    }
    cxInt idx = (unit->time - unit->life)/frameTime;
    idx = idx % tkeys->Size();
    if(unit->idx == idx){
        return;
    }
    CX_ASSERT(ptex != nullptr, "atlas texture null");
    const cxStr *key = tkeys->At(idx)->To<cxStr>();
    cxTexCoord *coord = ptex->At(key);
    CX_ASSERT(coord != nullptr, "texuture key %s miss",key->ToChars());
    unit->idx = idx;
    if(!coord->TrimmedCoord(Pixel(), FlipX(), FlipY())){
        return;
    }
    box.SetCoords(coord->coord);
}

CX_CPP_END

