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
    cxInt   from;
    cxInt   to;
    cxInt   key;
    cxFloat speed;
    cxInt   repeat;
    cxActionAttr();
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
    cxInt group;
    cxFrames *frames;       //has mul layer
    cxActionAttr attr;
public:
    
    cxAnimate *SetAction(const cxActionAttr *pattr,cxInt agroup);
    cxAnimate *SetGroup(cxInt agroup);
    cxAnimate *SetFrames(const cxFrames *aframes);
    
    cxAction *Reverse();
    cxAction *Clone();
};

CX_CPP_END

#endif


