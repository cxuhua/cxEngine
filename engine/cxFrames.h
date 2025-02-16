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

// 最大层数 0-9
#define MAX_LAYER_SIZE          10

// 每组最大帧数 0-99
#define MAX_GROUP_FRAME_SIZE    100

// 最大组数0-9
#define MAX_GROUP_SIZE          10

struct cxFrameMap
{
    cxInt num;
    cxPoint2F off[MAX_LAYER_SIZE];  //layer offset
    cxInt values[MAX_LAYER_SIZE];
};

class cxAnimate;
class cxFrames : public cxObject
{
public:
    CX_DECLARE(cxFrames);
protected:
    explicit cxFrames();
    virtual ~cxFrames();
private:
    std::vector<cxFloat> repeats;//每帧的重复倍数
    cxSize2F size;  //宽高
    cxInt repeat;
    BlendFunc blend;
    cxSize2F scale;
    cxTexture *ptex;
    cxInt layer;
    cxInt offset;
    cxInt group;    //最大支持10组 每组100帧
    cxInt count;
    cxFloat time;
    cxFloat speed;
    cxFloat delay;
    cxArray *points;
    cxFrameMap map;
private:
    cxTexCoord *layerEnd(cxInt group,cxInt count,cxInt layer);
    void loadlayers(cxArray *layers,cxInt c,cxInt g);
    void parseMapItem(cchars buf);
    cxPoint2F parseMapItemOff(cchars buf);
public:
    cxBool Init();
    //混合方式
    cxFrames *SetBlend(const BlendFunc &func);
    cxFrames *SetBlend(const cxStr *name);
    const BlendFunc Blend() const;
    //多层映射设置
    const cxFrameMap *Map() const;
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
    //每组帧总数
    cxFrames *SetCount(cxInt v);
    const cxInt Count() const;
    //单帧默认时间
    cxFrames *SetTime(cxFloat v);
    const cxFloat Time() const;
    //
    cxTimePoint *Append(cxFloat time);
    const cxTimePoint *At(cxInt idx) const;
    const cxArray *Points() const;
    const cxArray *Layers(cxInt idx) const;
    const cxTexCoord *TexCoord(cxInt idx=0,cxInt layer=0) const;
public:
    cxAnimate *Animate() const;
public:
    static void Load(cxHash *values, cchars file, std::function<cxTexture *(cchars file)>loadTexture);
};

CX_CPP_END

#endif


