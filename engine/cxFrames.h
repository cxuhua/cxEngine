//
//  cxFrames.h
//  cxEngineCore
//
//  Created by xuhua on 7/8/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#ifndef cxEngineCore_cxFrames_h
#define cxEngineCore_cxFrames_h

#include <vector>
#include "cxTexture.h"
#include "cxTimeLine.h"
#include "cxOpenGL.h"

CX_CPP_BEGIN

class cxAnimate;
class cxFrames : public cxObject
{
public:
    CX_DECLARE(cxFrames);
protected:
    explicit cxFrames();
    virtual ~cxFrames();
private:
    std::vector<cxInt> repeats;//每帧的重复倍数
    cxSize2F size;//宽高
    cxInt repeat;
    BlendFunc blend;
    cxSize2F scale;
    cxTexture *ptex;
    cxInt layer;
    cxInt offset;
    cxInt group;
    cxInt count;
    cxFloat time;
    cxFloat speed;
    cxFloat delay;
    cxArray *points;
    cxInt mapnum;           //层数量，最大16
    cxInt map[16];          //层映射
private:
    cxTexCoord *layerEnd(cxInt group,cxInt count,cxInt layer);
    void loadlayers(cxArray *layers,cxInt c,cxInt g);
public:
    cxBool Init();
    //混合方式
    cxFrames *SetBlend(const BlendFunc &func);
    cxFrames *SetBlend(const cxStr *name);
    const BlendFunc Blend() const;
    //多层映射设置
    const cxInt  Num() const;
    const cxInt *Map() const;
    void SetMaps(const cxStr *str);
    void SetMaps(cchars maps);
    void SetMaps(cxInt count,...);    //first = mapnum end SetMaps(2,0,1)
    //帧重复播放设置
    void SetRepeats(const cxStr *str);
    void SetRepeats(cchars str);
    //延迟设置
    cxFrames *SetDelay(cxFloat v);
    cxFloat Delay() const;
    //速度设置
    cxFrames *SetSpeed(cxFloat v);
    cxFloat Speed() const;
    //宽高设置
    cxFrames *SetSize(const cxSize2F &size);
    const cxSize2F &Size() const;
    //播放次数
    cxFrames *SetRepeat(cxInt v);
    const cxInt Repeat() const;
    //size缩放
    cxFrames *SetScale(cxSize2F v);
    const cxSize2F Scale() const;
    //纹理
    cxFrames *SetTexture(cchars key);
    cxFrames *SetTexture(const cxTexture *atex);
    const cxTexture *Texture() const;
    //帧偏移
    cxFrames *SetOffset(cxInt v);
    const cxInt Offset() const;
    //帧组
    cxFrames *SetGroup(cxInt v);
    const cxInt Group() const;
    //层数
    cxFrames *SetLayer(cxInt v);
    const cxInt Layer() const;
    //帧总数
    cxFrames *SetCount(cxInt v);
    const cxInt Count() const;
    //单帧默认时间
    cxFrames *SetTime(cxFloat v);
    const cxFloat Time() const;
    cxTimePoint *Append(cxFloat time);
    const cxTimePoint *At(cxInt idx) const;
    const cxArray *Points() const;
    const cxArray *Layers(cxInt idx) const;
    const cxTexCoord *TexCoord(cxInt idx=0,cxInt layer=0) const;
public:
    cxAnimate *Animate() const;
public:
    static void Load(cxHash *values,cchars file);
};

CX_CPP_END

#endif


