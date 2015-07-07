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
    cxTexture *ptex;
public:
    cxAnimate *SetTexture(cchars key);
    cxAnimate *SetTexture(cxTexture *texture);
    cxAnimate *Append(cxFloat time,cchars key);
    cxAnimate *AppFmt(cxFloat time,cchars fmt,...);
    cxAnimate *SetLoop(cxBool v);
    cxAction *Reverse();
    cxAction *Clone();
};

CX_CPP_END

#endif


