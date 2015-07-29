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

typedef enum {
    cxEmitterGravity,
    cxEmitterRadial,
}cxEmitterType;

typedef struct {
    cxPoint2F position;
    cxColor4F color;
    cxColor4F deltacolor;
    cxFloat size;
    cxFloat deltasize;
    cxFloat rotation;
    cxFloat deltarotation;
    cxFloat life;
    //gravity mode
    cxPoint2F dir;
    cxFloat radaccel;
    cxFloat tanaccel;
    //radial mode
    cxFloat angle;
    cxFloat degreespers;
    cxFloat radius;
    cxFloat deltaradius;
}cxEmitterUnit;

class cxEmitter : public cxAtlas
{
public:
    CX_DECLARE(cxEmitter);
protected:
    explicit cxEmitter();
    virtual ~cxEmitter();
protected:
    void OnUpdate(cxFloat dt);
private:
    void initEmitterUnit(cxEmitterUnit *unit);
    void addEmitterUnit();
    void unitToBoxPoint3F(cxEmitterUnit *unit,cxBoxPoint3F &vq);
    cxBoxPoint3F vbp;
    cxFloat systemtime;
    cxFloat runtime;
    cxEmitterUnit *units;
    cxFloat emitcounter;
    
    cxFloat rate;
    cxEmitterType type;
    cxPoint2FRange position;
    cxFloatRange life;
    cxFloatRange angle;
    
    cxFloatRange startsize;
    cxFloatRange endsize;
    
    cxColor4FRange startcolor;
    cxColor4FRange endcolor;
    
    cxFloatRange startspin;
    cxFloatRange endspin;
    
    cxPoint3F axisspin;
    //gravity mode
    cxPoint2F gravity;
    cxBool todir;
    cxFloatRange speed;
    cxFloatRange tanaccel;
    cxFloatRange radaccel;
    //radial mode
    cxFloatRange startradius;
    cxFloatRange endradius;
    cxFloatRange rotatepers;
public:
    static cxEmitter *Create(cxInt max);
    
    cxEvent<cxEmitter> onExit;
    
    cxView *Clone();
    
    cxEmitter *Stop();
    
    cxEmitter *SetRate(cxFloat v);
    
    cxEmitter *SetType(cxEmitterType v);
    
    cxEmitter *SetPosRange(const cxPoint2FRange &v);
    
    cxEmitter *SetLifeRange(const cxFloatRange &v);
    
    cxEmitter *SetAngleRange(const cxFloatRange &v);
    
    cxEmitter *SetStartSize(const cxFloatRange &v);
    cxEmitter *SetEndSize(const cxFloatRange &v);
    
    cxEmitter *SetStartColor(const cxColor4FRange &v);
    cxEmitter *SetEndColor(const cxColor4FRange &v);
    
    cxEmitter *SetStartSpin(const cxFloatRange &v);
    cxEmitter *SetEndSpin(const cxFloatRange &v);
    
    cxEmitter *SetAxisSpin(const cxPoint3F &v);
    
    cxEmitter *SetSystemTime(cxFloat v);
    
    cxEmitter *SetToDir(cxBool v);
};

CX_CPP_END

#endif


