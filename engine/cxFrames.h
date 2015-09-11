//
//  cxFrames.h
//  cxEngineCore
//
//  Created by xuhua on 7/8/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#ifndef cxEngineCore_cxFrames_h
#define cxEngineCore_cxFrames_h

#include <vector>
#include "cxTexture.h"
#include "cxTimeLine.h"
#include "cxOpenGL.h"

CX_CPP_BEGIN

class cxFrames : public cxObject
{
public:
    CX_DECLARE(cxFrames);
protected:
    explicit cxFrames();
    virtual ~cxFrames();
private:
    std::vector<cxInt> repeats;
    cxSize2F size;
    cxInt repeat;
    BlendFunc blend;
    cxFloat scale;
    cxTexture *ptex;
    cxInt layer;
    cxInt offset;
    cxInt group;
    cxInt count;
    cxFloat time;
    cxArray *points;
    cxInt mapnum;           //层数量，最大16
    cxInt map[16];          //层映射
private:
    cxTexCoord *layerEnd(cxInt group,cxInt count,cxInt layer);
    void loadlayers(cxArray *layers,cxInt c,cxInt g);
public:
    cxBool Init();
    
    cxFrames *SetBlend(const BlendFunc &func);
    cxFrames *SetBlend(const cxStr *name);
    const BlendFunc Blend() const;
    
    const cxInt  Num() const;
    const cxInt *Map() const;
    void SetMaps(const cxStr *str);
    void SetMaps(cchars maps);
    void SetMaps(cxInt count,...);    //first = mapnum end SetMaps(2,0,1)
    
    void SetRepeats(const cxStr *str);
    void SetRepeats(cchars str);
    
    cxFrames *SetSize(const cxSize2F &size);
    const cxSize2F &Size() const;
    
    cxFrames *SetRepeat(cxInt v);
    const cxInt Repeat() const;
    
    cxFrames *SetScale(cxFloat v);
    const cxFloat Scale() const;
    
    cxFrames *SetTexture(const cxTexture *atex);
    const cxTexture *Texture() const;
    
    cxFrames *SetOffset(cxInt v);
    const cxInt Offset() const;
    
    cxFrames *SetGroup(cxInt v);
    const cxInt Group() const;
    
    cxFrames *SetLayer(cxInt v);
    const cxInt Layer() const;
    
    cxFrames *SetCount(cxInt v);
    const cxInt Count() const;
    
    cxFrames *SetTime(cxFloat v);
    const cxFloat Time() const;
    
    cxTimePoint *Append(cxFloat time);
    const cxTimePoint *At(cxInt idx) const;
    const cxArray *Points() const;
    const cxArray *Layers(cxInt idx) const;
    const cxTexCoord *TexCoord(cxInt idx=0,cxInt layer=0) const;
};

CX_CPP_END

#endif


