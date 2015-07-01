//
//  cxEmitter.h
//  cxEngineCore
//
//  Created by xuhua on 6/28/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#ifndef cxEngineCore_cxEmitter_h
#define cxEngineCore_cxEmitter_h

#include "cxAtlas.h"

CX_CPP_BEGIN

class cxEmitter : public cxAtlas
{
public:
    CX_DECLARE(cxEmitter);
protected:
    explicit cxEmitter();
    virtual ~cxEmitter();
private:
public:
};

CX_CPP_END

#endif


