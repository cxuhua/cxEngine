//
//  cxCircle.h
//  cxEngineCore
//
//  Created by xuhua on 7/16/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#ifndef cxEngineCore_cxCircle_h
#define cxEngineCore_cxCircle_h

#include "cxTriangles.h"

CX_CPP_BEGIN

class cxCircle : public cxTriangles
{
public:
    CX_DECLARE(cxCircle);
protected:
    explicit cxCircle();
    virtual ~cxCircle();
protected:
    void OnDirty();
private:
    cxFloat percent;
    cxBool isreverse;
    cxFloat start;        //start angle
public:
    cxCircle *SetPercent(cxFloat v);
    cxCircle *SetIsReverse(cxBool v);
    cxCircle *SetStart(cxFloat v);
};

CX_CPP_END

#endif


