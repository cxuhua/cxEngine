//
//  cxFrames.cpp
//  cxEngineCore
//
//  Created by xuhua on 7/8/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#include <core/cxUtil.h>
#include <core/cxAutoPool.h>
#include <core/cxCSV.h>
#include "cxAnimate.h"
#include "cxFrames.h"

CX_CPP_BEGIN

CX_IMPLEMENT(cxFrames);

void cxFrames::Load(cxHash *values,cchars file)
{
    const cxStr *data = cxUtil::Content(file);
    CX_ASSERT(cxStr::IsOK(data), "get %s file data error",file);
    cxCSV *csv = cxCSV::Create(data);
    cxStr *name = nullptr;
    cxInt nrow = 0;
    cxInt level = 1;
    for(cxInt i=3; i < csv->Row(); i++){
        const cxStr *cn = csv->At(i, 0);
        if(cxStr::IsOK(cn)){
            cxObject::swap(&name, cn);
            nrow = i;
            level = 1;
        }else{
            level ++;
        }
        cxFrames *attr = cxFrames::Create();
        for(cxInt j=2;j<csv->Col(1);j++){
            const cxStr *ktype = csv->At(1, j);
            const cxStr *value = csv->At(i, j);
            cxInt c = i;
            while(!cxStr::IsOK(value) && c > nrow){
                value = csv->At(--c, j);
            }
            if(!cxStr::IsOK(value)){
                continue;
            }
            if(ktype->IsCaseEqu("Texture")){
                attr->SetTexture(value->ToString());
                continue;
            }
            if(ktype->IsCaseEqu("Size")){
                attr->SetSize(value->ToSize2F());
                continue;
            }
            if(ktype->IsCaseEqu("Scale")){
                attr->SetScale(value->ToSize2F());
                continue;
            }
            if(ktype->IsCaseEqu("Offset")){
                attr->SetOffset(value->ToInt());
                continue;
            }
            if(ktype->IsCaseEqu("Group")){
                attr->SetGroup(value->ToInt());
                continue;
            }
            if(ktype->IsCaseEqu("Count")){
                attr->SetCount(value->ToInt());
                continue;
            }
            if(ktype->IsCaseEqu("Layer")){
                attr->SetLayer(value->ToInt());
                continue;
            }
            if(ktype->IsCaseEqu("Map")){
                attr->SetMaps(value);
                continue;
            }
            if(ktype->IsCaseEqu("Repeats")){
                attr->SetRepeats(value);
                continue;
            }
        }
        if(attr->Init()){
            values->Set(cxHashKey::Format("%s_%d",name->ToString(),level), attr);
        }
    }
    cxObject::release(&name);
}

cxFrames::cxFrames()
{
    speed = 1.0f;
    delay = 0.0f;
    repeat = 1;
    blend = BlendFunc::ALPHA;
    scale = 1.0f;
    mapnum = 1;
    map[0] = 0;
    layer = 1;
    group = 1;
    offset = 0;
    time = 0.1f;
    count = 0;
    ptex = nullptr;
    points = cxArray::Alloc();
    scale = 1.0f;
}

cxFrames::~cxFrames()
{
    cxObject::release(&ptex);
    points->Release();
}

cxAnimate *cxFrames::Animate() const
{
    return cxAnimate::Create(this);
}

cxFrames *cxFrames::SetDelay(cxFloat v)
{
    delay = v;
    return this;
}

cxFloat cxFrames::Delay() const
{
    return delay;
}

cxFrames *cxFrames::SetSpeed(cxFloat v)
{
    speed = v;
    return this;
}

cxFloat cxFrames::Speed() const
{
    return speed;
}

const cxInt cxFrames::Num() const
{
    return mapnum;
}

const cxInt *cxFrames::Map() const
{
    return map;
}

void cxFrames::SetMaps(cxInt count,...)
{
    if(count == 0){
        return;
    }
    mapnum = count;
    va_list ap;
    va_start(ap, count);
    cxInt i = 0;
    do{
        map[i++] = va_arg(ap, cxInt);
    }while(--count > 0);
    va_end(ap);
}

void cxFrames::SetMaps(const cxStr *str)
{
    if(!cxStr::IsOK(str)){
        return;
    }
    SetMaps(str->ToString());
}

void cxFrames::SetMaps(cchars maps)
{
    if(!cxStr::IsOK(maps)){
        return;
    }
    char buffers[MAX_FRAME_COUNT]={0};
    cxInt len = (cxInt)strlen(maps);
    cxInt b = 0;
    mapnum = 0;
    for(cxInt i=0;i<len;i++){
        if(maps[i] != ',')continue;
        memcpy(buffers, maps + b, i-b);
        buffers[i - b] = 0;
        map[mapnum++] = atoi(buffers);
        b = i+1;
    }
    if(len > b){
        memcpy(buffers, maps + b, len-b);
        buffers[len - b] = 0;
        map[mapnum++] = atoi(buffers);
    }
}

void cxFrames::SetRepeats(const cxStr *str)
{
    if(!cxStr::IsOK(str)){
        return;
    }
    SetRepeats(str->ToString());
}

void cxFrames::SetRepeats(cchars str)
{
    if(!cxStr::IsOK(str)){
        return;
    }
    char buffers[MAX_FRAME_COUNT]={0};
    cxInt len = (cxInt)strlen(str);
    cxInt b = 0;
    repeats.clear();
    for(cxInt i=0;i<len;i++){
        if(str[i] != ',')continue;
        memcpy(buffers, str + b, i-b);
        buffers[i - b] = 0;
        cxFloat v = atof(buffers);
        repeats.push_back(v);
        b = i+1;
    }
    if(len > b){
        memcpy(buffers, str + b, len-b);
        buffers[len - b] = 0;
        cxFloat v = atof(buffers);
        repeats.push_back(v);
    }
}

const cxTexCoord *cxFrames::TexCoord(cxInt idx,cxInt layer) const
{
    const cxArray *layers = Layers(idx);
    return layers->At(layer)->To<cxTexCoord>();
}

const cxArray *cxFrames::Layers(cxInt idx) const
{
    const cxTimePoint *tp = At(idx);
    return tp->Object()->To<cxArray>();
}

//获取
cxTexCoord *cxFrames::layerEnd(cxInt group,cxInt count,cxInt layer)
{
    cxInt idx = group * Count() + count - 1;
    const cxTimePoint *tp = At(idx);
    cxArray *layers = tp->Object()->To<cxArray>();
    return layers->At(layer)->To<cxTexCoord>();
}

//i.l.png
//i ->帧名
//l -> 层
//例如:1701310.0.png
void cxFrames::loadlayers(cxArray *layers,cxInt c,cxInt g)
{
    const cxTexture *texture = Texture();
    for(cxInt l = 0;l < Layer();l++){
        char key[128]={0};
        //每组最多100张
        snprintf(key, 128, "%d.%d.png",Offset()+g*100+c,l);
        cxTexCoord *coord = texture->At(key);
        //如果当前层丢失帧图片，使用最后一个存在的图片
        if(coord == nullptr && c > 0){
            coord = layerEnd(g, c, l);
        }
        CX_ASSERT(coord != nullptr, "%s texture key miss",key);
        layers->Append(coord);
    }
}

cxBool cxFrames::Init()
{
    CX_ASSERT(Texture() != nullptr && Count() > 0 && Group() > 0 && Layer() > 0, "data format error");
    for(cxInt g = 0;g < Group();g++)
    for(cxInt c = 0;c < Count();c++){
        //每帧的重复倍数
        cxFloat repeat = (c < repeats.size())?repeats.at(c):1;
        //添加时间点
        cxTimePoint *tp = Append(Time() * repeat);
        //加载保存层
        cxArray *layers = cxArray::Alloc();
        tp->SetObject(layers);
        loadlayers(layers, c, g);
        layers->Release();
    }
    return true;
}

cxFrames *cxFrames::SetBlend(const BlendFunc &func)
{
    blend = func;
    return this;
}

cxFrames *cxFrames::SetBlend(const cxStr *name)
{
    blend = BlendFunc::To(name->ToString());
    return this;
}

const BlendFunc cxFrames::Blend() const
{
    return blend;
}

cxFrames *cxFrames::SetRepeat(cxInt v)
{
    repeat = v;
    return this;
}

const cxInt cxFrames::Repeat() const
{
    //forever loop play
    if(repeat == 0){
        return INT_MAX;
    }
    return repeat;
}

cxFrames *cxFrames::SetSize(const cxSize2F &v)
{
    size = v;
    return this;
}

const cxSize2F &cxFrames::Size() const
{
    return size;
}

cxFrames *cxFrames::SetScale(cxSize2F v)
{
    CX_ASSERT(!v.IsZero(), "scale must > 0");
    scale = v;
    return this;
}

const cxSize2F cxFrames::Scale() const
{
    return scale;
}

cxFrames *cxFrames::SetTexture(cchars key)
{
    cxTexture *ptex = cxObject::gcGet<cxTexture>(key);
    CX_ASSERT(ptex != nullptr, "texture key %s miss,need LoadTexture",key);
    SetTexture(ptex);
    return this;
}

cxFrames *cxFrames::SetTexture(const cxTexture *atex)
{
    cxObject::swap(&ptex, atex);
    return this;
}

const cxTexture *cxFrames::Texture() const
{
    return ptex;
}

cxFrames *cxFrames::SetOffset(cxInt v)
{
    offset = v;
    return this;
}

const cxInt cxFrames::Offset() const
{
    return offset;
}

cxFrames *cxFrames::SetGroup(cxInt v)
{
    group = v;
    return this;
}

const cxInt cxFrames::Group() const
{
    return group;
}

cxFrames *cxFrames::SetLayer(cxInt v)
{
    layer = v;
    return this;
}

const cxInt cxFrames::Layer() const
{
    return layer;
}

cxFrames *cxFrames::SetCount(cxInt v)
{
    count = v;
    return this;
}

const cxInt cxFrames::Count() const
{
    return count;
}

cxFrames *cxFrames::SetTime(cxFloat v)
{
    time = v;
    return this;
}

const cxFloat cxFrames::Time() const
{
    return time;
}

const cxTimePoint *cxFrames::At(cxInt idx) const
{
    return points->At(idx)->To<cxTimePoint>();
}

cxTimePoint *cxFrames::Append(cxFloat time)
{
    cxTimePoint *tp = cxTimePoint::Create()->Init(time);
    points->Append(tp);
    return tp;
}

const cxArray *cxFrames::Points() const
{
    return points;
}

CX_CPP_END

