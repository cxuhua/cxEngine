//
//  cxEmitter.h
//  cxEngineCore
//
//  Created by xuhua on 6/28/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#ifndef cxEngineCore_cxEmitter_h
#define cxEngineCore_cxEmitter_h

#include <core/cxXml.h>
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
    cxInt idx;
    cxFloat time;
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
    virtual void OnBoxRender(cxEmitterUnit *unit,cxBoxRender &box);
    virtual void Init();
private:
    cxBool autoRemove;
    cxFloat frameTime;
    cxArray *tkeys;
    cxInt max;
    cxBool isinit;
    void initEmitterUnit(cxEmitterUnit *unit);
    void addEmitterUnit();
    void unitToBoxPoint3F(cxEmitterUnit *unit,cxBoxPoint3F &vq);
    cxBoxPoint3F vbp;
    cxFloat runtime;
    cxEmitterUnit *units;
    cxFloat emitcounter;
    
    cxFloat systemtime;
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
    cxBool torotate;
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
public:
    cxEvent<cxEmitter> onExit;
    cxEvent<cxEmitter> onStart;
public:
    cxEmitter *SetFrameTime(cxFloat v);
    cxEmitter *AppendFrameKey(cchars fmt,...);
    cxEmitter *AppendFrameKeys(const cxArray *keys);
    cxEmitter *AppendFrameKey(const cxStr *key);
    
    cxEmitter *SetMax(cxInt v);
    
    cxView *Clone();
    
    cxEmitter *SetAutoRemove(cxBool v);
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
    
    cxEmitter *SetGravity(const cxPoint2F &v);
    cxEmitter *SetToRotate(cxBool v);
    cxEmitter *SetToDir(cxBool v);
    cxEmitter *SetSpeed(const cxFloatRange &v);
    cxEmitter *SetTanAccel(const cxFloatRange &v);
    cxEmitter *SetRadAccel(const cxFloatRange &v);
    
    cxEmitter *SetStartRadius(const cxFloatRange &v);
    cxEmitter *SetEndRadius(const cxFloatRange &v);
    cxEmitter *SetRotatePers(const cxFloatRange &v);
};

// xml format emitter
class cxEmitterXml : public cxXml
{
public:
    CX_DECLARE(cxEmitterXml);
protected:
    explicit cxEmitterXml();
    virtual ~cxEmitterXml();
private:
    cxStr *skey;
    cxStr *svalue;
private:
    const cxStr *ToStr(cchars name,const cxStr *value);
    cxInt ToInt(cchars name,const cxStr *value);
    cxUInt ToUInt(cchars name,const cxStr *value);
    cxFloat ToFloat(cchars name,const cxStr *value);
    
    cxTexture *ptex;
    cxStr *textureFileName;
    cxStr *textureImageData;
    
    void parseTexture();
    
    cxFloat angle;
    cxFloat angleVariance;
    
    cxUInt blendFuncDestination;
    cxUInt blendFuncSource;
    
    cxFloat duration;
    cxInt emitterType;
    cxFloat emissionRate;
    
    cxFloat finishColorAlpha;
    cxFloat finishColorBlue;
    cxFloat finishColorGreen;
    cxFloat finishColorRed;
    cxFloat finishColorVarianceAlpha;
    cxFloat finishColorVarianceBlue;
    cxFloat finishColorVarianceGreen;
    cxFloat finishColorVarianceRed;
    
    cxFloat rotationStart;
    cxFloat rotationStartVariance;
    cxFloat rotationEnd;
    cxFloat rotationEndVariance;
    cxFloat finishParticleSize;
    cxFloat finishParticleSizeVariance;
    
    cxFloat gravityx;
    cxFloat gravityy;
    
    cxInt maxParticles;
    cxFloat maxRadius;
    cxFloat maxRadiusVariance;
    
    cxFloat minRadius;
    cxFloat minRadiusVariance;
    cxFloat particleLifespan;
    cxFloat particleLifespanVariance;

    cxFloat radialAcceleration;
    cxFloat radialAccelVariance;
    cxFloat rotatePerSecond;
    cxFloat rotatePerSecondVariance;
    
    cxFloat sourcePositionVariancex;
    cxFloat sourcePositionVariancey;
    
    cxFloat speed;
    cxFloat speedVariance;
    
    cxFloat startColorAlpha;
    cxFloat startColorBlue;
    cxFloat startColorGreen;
    cxFloat startColorRed;
    cxFloat startColorVarianceAlpha;
    cxFloat startColorVarianceBlue;
    cxFloat startColorVarianceGreen;
    cxFloat startColorVarianceRed;
    
    cxFloat startParticleSize;
    cxFloat startParticleSizeVariance;
    cxFloat tangentialAcceleration;
    cxFloat tangentialAccelVariance;
    
protected:
    void OnKeyValue(const cxStr *key,cchars name,const cxStr *value);
    void OnElementStart(cchars name,cchars *attr);
    void OnElementEnd(cchars name);
    void OnCharacter(cchars data,cxInt len);
public:
    cxEmitter *CreateEmitter();
public:
    // open {{name}}.xml file
    // file ext must .xml
    static cxEmitterXml *CreateXml(cchars name);
};

// json format emitter

CX_CPP_END

#endif


