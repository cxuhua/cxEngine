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

struct cxActionAttr
{
    cxInt   group;//>0fix group
    cxInt   from;
    cxInt   to;
    cxInt   key;
    cxFloat speed;
    cxFloat delay;
    cxInt   repeat;
    cxActionAttr();
    cxActionAttr Reverse();
};

class cxFrames;
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
    cxInt group;            //agroup
    cxFrames *frames;       //has mul layer
    cxActionAttr attr;
    cxBool iskeyframe;
public:
    cxAnimate *SetAction(const cxActionAttr *pattr,cxInt agroup);
    cxAnimate *SetGroup(cxInt agroup);
    cxAnimate *SetRange(cxInt afrom,cxInt ato);
    cxAnimate *SetFrames(const cxFrames *aframes);
    
    cxAction *Reverse();
    cxAction *Clone();
public:
    cxEvent<cxAnimate, cxInt> onFrame;
    const cxActionAttr &ActionAttr() const;
    const cxBool IsKeyFrame() const;
    
    static cxAnimate *Create(const cxFrames *frames);
};

CX_CPP_END

#endif


