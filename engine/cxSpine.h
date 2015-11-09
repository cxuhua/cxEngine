//
//  cxSpine.h
//  cxEngineCore
//
//  Created by xuhua on 11/6/15.
//  Copyright © 2015 xuhua. All rights reserved.
//

#ifndef cxEngineCore_cxSpine_h
#define cxEngineCore_cxSpine_h

#include <spine/spine.h>
#include "cxTriangles.h"

CX_CPP_BEGIN

class cxSpine : public cxTriangles
{
public:
    CX_DECLARE(cxSpine);
protected:
    explicit cxSpine();
    virtual ~cxSpine();
protected:
    void OnUpdate(cxFloat dt);
    void OnRender(cxRender *render, const cxMatrixF &model);
private:
    cxFloat *vertices;
    cxFloat timeScale;
    spAnimationState* spState;
    spBone* rootBone;
    spSkeletonData* skeletonData;
    spAtlas* atlas;
    spSkeleton* skeleton;
    cxInt verticesNum;
public:
    
    void SetTimeScale(cxFloat v);
    
    void SetMix(cchars from,cchars to,cxFloat duration);
    cxFloat GetMix(cchars from,cchars to);
    
    spAnimation *FindAnimation(cchars name);
    spTrackEntry *SetAnimation(cxInt idx,cchars name,cxBool loop);
    spTrackEntry *AddAnimation(cxInt idx,cchars name,cxBool loop,cxFloat delay);
    
    spSkin *FindSkin(cchars name);
    void SetSkin(cchars name);
public:
    virtual void AnimationListen(spAnimationState* state, cxInt trackIndex, spEventType type, spEvent* event, cxInt loopCount);
    void Init(cchars atlasFile, cchars jsonFile, cxFloat scale);
    static cxSpine *Create(cchars atlasFile, cchars jsonFile, cxFloat scale=1.0f);
};

CX_CPP_END

#endif


