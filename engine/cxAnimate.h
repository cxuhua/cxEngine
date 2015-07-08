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
    cxFloat time;
    cxBool  loop;
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
    cxFrames *frames;
    cxActionAttr attr;
    cxInt count;
    cxInt group;
    cxTexture *ptex;
public:
    cxAnimate *SetAttr(const cxActionAttr *pattr,cxInt acount,cxInt agroup);
    cxAnimate *SetAttr(const cxActionAttr *pattr,cxInt agroup);
    cxAnimate *SetTexture(cchars key);
    cxAnimate *SetTexture(cxTexture *texture);
    cxAnimate *SetFrames(const cxFrames *aframes);
    cxAnimate *Append(cxFloat time,cchars key);
    cxAnimate *AppFmt(cxFloat time,cchars fmt,...);
    cxAnimate *SetLoop(cxBool v);
    cxAction *Reverse();
    cxAction *Clone();
};

CX_CPP_END

#endif


