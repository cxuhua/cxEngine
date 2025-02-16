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
#include <core/cxHash.h>
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
private:
    cxObject *object;
    cxFloat time;
public:
    cxTimePoint *Init(cxFloat at);
    
    cxObject *Object() const;
    void SetObject(cxObject *pobj);
    
    cxArray *Array() const;
    void SetArray(cxArray *parray);
    
    cxHash *HashMap() const;
    void SetHashMap(cxHash *phash);
    
    cxBool SetTime(cxFloat v);
    cxFloat Time() const;
};

enum TimeType
{
    // 按每帧平均时间进行
    TimeTypeFrame = 1,
    // 按每帧设置的时间线进行
    TimeTypeLine
};

class cxTimeLine : public cxAction
{
public:
    CX_DECLARE(cxTimeLine);
    typedef std::vector<cxFloat> Times;
protected:
    explicit cxTimeLine();
    virtual ~cxTimeLine();
protected:
    virtual void OnTime(const cxTimePoint *tp);
    void OnStep(cxFloat dt);
    void OnInit();
private:
    cxFloat looptime;
    Times times;    //
    cxArray *points;//
    cxBool isdirty; //
    cxInt prev;     //
    cxInt idx;      //0-n
    cxInt from;     //0-(n-1);
    cxInt to;       //0-(n-1)
    TimeType timeType;
public:
    cxEvent<cxTimeLine,const cxTimePoint *> onTime;
public:
    void SetTimeType(TimeType v);
    cxTimeLine *SetRange(cxInt afrom,cxInt ato);
    void UpdateTimes();
    
    void SetLoopTime(cxFloat v);
    
    cxTimeLine *SetPoints(const cxArray *ps);
    
    const cxTimePoint *TimePoint() const;
    const cxTimePoint *At(cxInt i) const;
    cxTimePoint *At(cxInt i);
    
    cxTimePoint *Append(cxFloat time);
    cxArray *Points();
    const cxArray *Points() const;
    //帧索引
    const cxInt Index() const;
    //播放帧数量
    const cxInt Length() const;
    //从*帧
    const cxInt From() const;
    //到*帧
    const cxInt To() const;
    
    cxAction *Reverse();
    cxAction *Clone();
};

CX_CPP_END

#endif


