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

CX_CPP_BEGIN

class cxButton;
class IOSEngine : public cxEngine
{
public:
    CX_DECLARE(IOSEngine);
protected:
    explicit IOSEngine();
    virtual ~IOSEngine();
    virtual void OnUpdate(cxFloat dt);
public:
    void OnMain();
    void test(cxButton *sender);
};

CX_CPP_END

#endif


