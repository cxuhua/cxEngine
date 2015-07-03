//
//  cxAnimate.h
//  cxEngineCore
//  cxSprite animate action
//  Created by xuhua on 7/3/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#ifndef cxEngineCore_cxAnimate_h
#define cxEngineCore_cxAnimate_h

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
    void OnTime();
    void OnInit();
private:
    cxTexture *ptex;
public:
    cxAnimate *SetTexture(cchars key);
    cxAnimate *Append(cxFloat time,cchars key);
    cxAnimate *AppFmt(cxFloat time,cchars fmt,...);
};

CX_CPP_END

#endif


