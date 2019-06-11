//
//  IOSEngine.h
//  cxEngine2D
//
//  Created by xuhua on 6/17/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#ifndef cxEngine2D_IOSEngine_h
#define cxEngine2D_IOSEngine_h

#include <engine/cxEngine.h>
#include "CGmSktUse.h"

CX_CPP_BEGIN

class Game : public cxEngine
{
public:
    CX_DECLARE(Game);
protected:
    explicit Game();
    virtual ~Game();
protected:
    void OnEvent(cxAsyncEvent *e);
private:
    CGmSktUse skt;
public:
    void OnMain();
};

CX_CPP_END

#endif


