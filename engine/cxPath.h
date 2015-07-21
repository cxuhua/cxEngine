//
//  cxPath.h
//  cxEngineCore
//
//  Created by xuhua on 7/21/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#ifndef cxEngineCore_cxPath_h
#define cxEngineCore_cxPath_h

#include "cxTimeLine.h"

CX_CPP_BEGIN

class cxPath : public cxTimeLine
{
public:
    CX_DECLARE(cxPath);
protected:
    explicit cxPath();
    virtual ~cxPath();
protected:
    void OnTime(const cxTimePoint *tp);
    void OnStep(cxFloat dt);
    void OnInit();
private:
    cxFloat speed;
    cxPoint2F next;
    cxPoint2FArray points;
public:
    cxPath *Append(const cxPoint2F &pos);
    static cxPath *Create(cxFloat speed);
};

CX_CPP_END

#endif


