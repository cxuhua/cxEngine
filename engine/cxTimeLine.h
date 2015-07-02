//
//  cxTimeLine.h
//  cxEngineCore
//
//  Created by xuhua on 7/2/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#ifndef cxEngineCore_cxTimeLine_h
#define cxEngineCore_cxTimeLine_h

#include <vector>
#include <core/cxArray.h>
#include "cxTexture.h"
#include "cxAction.h"

CX_CPP_BEGIN

class cxTimePoint : public cxObject
{
public:
    CX_DECLARE(cxTimePoint);
protected:
    explicit cxTimePoint();
    virtual ~cxTimePoint();
public:
    cxFloat time;
    cxTimePoint *Init(cxFloat at);
};

class cxTimeLine : public cxAction
{
public:
    CX_DECLARE(cxTimeLine);
protected:
    explicit cxTimeLine();
    virtual ~cxTimeLine();
protected:
    void OnStep(cxFloat dt);
    void OnInit();
private:
    std::vector<cxFloat> times;
    cxArray *frames;
    cxFloat elapsedTime;
    cxInt previdx;
    cxInt idx;      //0-n
    cxInt from;     //0-(size-1);
    cxInt to;       //0-(size-1)
public:
    cxEvent<cxTimeLine> onTime;
    
    cxTimeLine *SetRange(cxInt afrom,cxInt ato);
    
    cxInt Index() const;
    
    cxTimePoint *At(cxInt idx);
    
    cxTimePoint *Push(cxFloat time);
    
    cxInt Size() const;
};

CX_CPP_END

#endif


