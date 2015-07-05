//
//  cxJump.h
//  cxEngineCore
//
//  Created by xuhua on 7/5/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#ifndef cxEngineCore_cxJump_h
#define cxEngineCore_cxJump_h

#include "cxAction.h"

CX_CPP_BEGIN

class cxJump : public cxAction
{
public:
    CX_DECLARE(cxJump);
protected:
    explicit cxJump();
    virtual ~cxJump();
protected:
    void OnStep(cxFloat dt);
    void OnInit();
private:
    cxPoint2F from;
    cxPoint2F prev;
    cxPoint2F to;
    cxFloat height;
    cxInt  jumps;
public:
    static cxJump *Create(const cxPoint2F &to,cxFloat height,cxInt jumps,cxFloat time);
    cxAction *Reverse();
    cxAction *Clone();
};

CX_CPP_END

#endif


