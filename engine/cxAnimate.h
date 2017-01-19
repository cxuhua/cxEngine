//
//  cxAnimate.h
//  cxEngineCore
//  cxSprite animate action
//  Created by xuhua on 7/3/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#ifndef cxEngineCore_cxAnimate_h
#define cxEngineCore_cxAnimate_h

#include <math/cxPoint2I.h>
#include "cxTimeLine.h"

CX_CPP_BEGIN

#define CX_ACTION_MAX_KEY 8

struct cxActionAttr
{
    // 从from播放到to帧
    cxInt from;
    cxInt to;
    // 关键帧序号
    cxInt keynum;
    cxInt keyvar[CX_ACTION_MAX_KEY];
    // 播放速度
    cxFloat speed;
    // 播放延迟
    cxFloat delay;
    // 重复次数0(无限)
    cxInt repeat;
    cxActionAttr();
    // 反向播放获取
    cxActionAttr Reverse();
    //
    void SetKey(const cxStr *value);
    //
    void SetFrom(const cxStr *value);
    //
    void SetTo(const cxStr *value);
    //
    void SetRepeat(const cxStr *value);
    //
    void SetSpeed(const cxStr *value);
};

class cxFrames;

// 用于 cxAtlas 帧动画对象
class cxAnimate : public cxTimeLine
{
public:
    CX_DECLARE(cxAnimate);
protected:
    explicit cxAnimate();
    virtual ~cxAnimate();
protected:
    void OnTime(const cxTimePoint *tp);
    void OnInit();
private:
    cxInt group;            //agroup 1701100.0 -> 00-99(0) 100-199(1) 200-299(2).... 0层
    cxFrames *frames;       //has mul layer
    cxActionAttr attr;
    cxInt keyframe;
public:
    // 设置动作属性
    cxAnimate *SetAction(const cxActionAttr *pattr,cxInt agroup);
    // 设置动作组
    cxAnimate *SetGroup(cxInt agroup);
    // 设置播放范围
    cxAnimate *SetRange(cxInt afrom,cxInt ato);
    // 设置帧序列
    cxAnimate *SetFrames(const cxFrames *aframes);
public:
    cxAction *Reverse();
    cxAction *Clone();
public:
    // 当播放某帧时
    cxEvent<cxAnimate, cxInt> onFrame;
    // 当播放关键帧时
    cxEvent<cxAnimate, cxInt> onKey;
public:
    // 当前动作序列属性
    const cxActionAttr &ActionAttr() const;
    // >=0 表示有关键帧 onFrame or onKey 中 调用
    const cxInt KeyIndex() const; // 关键帧序号
    const cxInt KeyValue() const; // 关键帧值
public:
    static cxAnimate *Create(const cxFrames *frames);
};

CX_CPP_END

#endif


