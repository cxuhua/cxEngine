//
//  cxFrames.h
//  cxEngineCore
//
//  Created by xuhua on 7/8/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#ifndef cxEngineCore_cxFrames_h
#define cxEngineCore_cxFrames_h

#include "cxTexture.h"
#include "cxTimeLine.h"

CX_CPP_BEGIN

class cxFrames : public cxObject
{
public:
    CX_DECLARE(cxFrames);
protected:
    explicit cxFrames();
    virtual ~cxFrames();
private:
    cxTexture *ptex;
    cxInt layer;
    cxInt offset;
    cxInt group;
    cxInt count;
    cxFloat time;
    cxArray *points;
    cxInt mapnum;           //层数量，最大16
    cxInt map[16];          //层映射
public:
    const cxInt  Num() const;
    const cxInt *Map() const;
    void InitMaps(const cxStr *str);
    
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
};

CX_CPP_END

#endif


