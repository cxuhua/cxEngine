//
//  cxCurve.h
//  cxEngineCore
//
//  Created by xuhua on 6/29/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#ifndef cxEngineCore_cxCurve_h
#define cxEngineCore_cxCurve_h

#include <math/cxMath.h>

CX_CPP_BEGIN

//time 0-1 -> 0-1
typedef cxFloat (*cxTimingFunc)(cxFloat time);

struct cxTiming
{
    static cxFloat BounceIn(cxFloat t);
    static cxFloat BounceOut(cxFloat t);
    static cxFloat BounceInOut(cxFloat t);
    static cxFloat BounceOutIn(cxFloat t);
    
    static cxFloat OvershootIn(cxFloat t);
    static cxFloat OvershootOut(cxFloat t);
    static cxFloat OvershootInOut(cxFloat t);
    static cxFloat OvershootOutIn(cxFloat t);
    
    static cxFloat ElasticIn(cxFloat t);
    static cxFloat ElasticOut(cxFloat t);
    static cxFloat ElasticInOut(cxFloat t);
    static cxFloat ElasticOutIn(cxFloat t);
    
    static cxFloat QuadIn(cxFloat t);
    static cxFloat QuadOut(cxFloat t);
    static cxFloat QuadInOut(cxFloat t);
    static cxFloat QuadOutIn(cxFloat t);
    
    static cxFloat CubicIn(cxFloat t);
    static cxFloat CubicOut(cxFloat t);
    static cxFloat CubicInOut(cxFloat t);
    static cxFloat CubicOutIn(cxFloat t);
    
    static cxFloat QuarIn(cxFloat t);
    static cxFloat QuarOut(cxFloat t);
    static cxFloat QuarInOut(cxFloat t);
    static cxFloat QuarOutIn(cxFloat t);
    
    static cxFloat QuinIn(cxFloat t);
    static cxFloat QuinOut(cxFloat t);
    static cxFloat QuinInOut(cxFloat t);
    static cxFloat QuinOutIn(cxFloat t);
    
    static cxFloat SineIn(cxFloat t);
    static cxFloat SineOut(cxFloat t);
    static cxFloat SineInOut(cxFloat t);
    static cxFloat SineOutIn(cxFloat t);
    
    static cxFloat ExpoIn(cxFloat t);
    static cxFloat ExpoOut(cxFloat t);
    static cxFloat ExpoInOut(cxFloat t);
    static cxFloat ExpoOutIn(cxFloat t);
    
    static cxFloat SqrtIn(cxFloat t);
    static cxFloat SqrtOut(cxFloat t);
    static cxFloat SqrtInOut(cxFloat t);
    static cxFloat SqrtOutIn(cxFloat t);
};

CX_CPP_END

#endif


