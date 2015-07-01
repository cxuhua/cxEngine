//
//  cxCurve.cpp
//  cxEngineCore
//
//  Created by xuhua on 6/29/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//


#include "cxTiming.h"

CX_CPP_BEGIN

cxFloat cxTiming::BounceIn(cxFloat t)
{
    t = 1.0f - t;
    if (t < 0.36363636363636365f){
        t = 7.5625f * t * t;
    }else if (t < 0.7272727272727273f){
        t -= 0.5454545454545454f;
        t = 7.5625f * t * t + 0.75f;
    }else if (t < 0.9090909090909091f){
        t -= 0.8181818181818182f;
        t = 7.5625f * t * t + 0.9375f;
    }else{
        t -= 0.9545454545454546f;
        t = 7.5625f * t * t + 0.984375f;
    }
    t = 1.0f - t;
    return t;
}

cxFloat cxTiming::BounceOut(cxFloat t)
{
    if (t < 0.36363636363636365f){
        t = 7.5625f * t * t;
    }else if (t < 0.7272727272727273f){
        t -= 0.5454545454545454f;
        t = 7.5625f * t * t + 0.75f;
    }else if (t < 0.9090909090909091f){
        t -= 0.8181818181818182f;
        t = 7.5625f * t * t + 0.9375f;
    }else{
        t -= 0.9545454545454546f;
        t = 7.5625f * t * t + 0.984375f;
    }
    return t;
}

cxFloat cxTiming::BounceInOut(cxFloat t)
{
    if (t < 0.5f){
        t = 1.0f - t * 2.0f;
        if (t < 0.36363636363636365f){
            t = 7.5625f * t * t;
        }else if (t < 0.7272727272727273f){
            t -= 0.5454545454545454f;
            t = 7.5625f * t * t + 0.75f;
        }else if (t < 0.9090909090909091f){
            t -= 0.8181818181818182f;
            t = 7.5625f * t * t + 0.9375f;
        }else{
            t -= 0.9545454545454546f;
            t = 7.5625f * t * t + 0.984375f;
        }
        t = (1.0f - t) * 0.5f;
    }else{
        t = t * 2.0f - 1.0f;
        if (t < 0.36363636363636365f){
            t = 7.5625f * t * t;
        }else if (t < 0.7272727272727273f){
            t -= 0.5454545454545454f;
            t = 7.5625f * t * t + 0.75f;
        }else if (t < 0.9090909090909091f){
            t -= 0.8181818181818182f;
            t = 7.5625f * t * t + 0.9375f;
        }else{
            t -= 0.9545454545454546f;
            t = 7.5625f * t * t + 0.984375f;
        }
        t = 0.5f * t + 0.5f;
    }
    return t;
}

cxFloat cxTiming::BounceOutIn(cxFloat t)
{
    if (t < 0.1818181818f){
        t = 15.125f * t * t;
    }else if (t < 0.3636363636f){
        t = 1.5f + (-8.250000001f + 15.125f * t) * t;
    }else if (t < 0.4545454546f){
        t = 3.0f + (-12.375f + 15.125f * t) * t;
    }else if (t < 0.5f){
        t = 3.9375f + (-14.4375f + 15.125f * t) * t;
    }else if (t <= 0.5454545455f){
        t = -3.625000004f + (15.81250001f - 15.125f * t) * t;
    }else if (t <= 0.6363636365f){
        t = -4.75f + (17.875f - 15.125f * t) * t;
    }else if (t <= 0.8181818180f){
        t = -7.374999995f + (21.99999999f - 15.125f * t) * t;
    }else{
        t = -14.125f + (30.25f - 15.125f * t) * t;
    }
    return t;
}

cxFloat cxTiming::OvershootIn(cxFloat t)
{
    t = t * t * (2.70158f * t - 1.70158f);
    return t;
}

cxFloat cxTiming::OvershootOut(cxFloat t)
{
    t--;
    t = t * t * (2.70158f * t + 1.70158f) + 1;
    return t;
}

cxFloat cxTiming::OvershootInOut(cxFloat t)
{
    t *= 2.0f;
    if (t < 1.0f){
        t = 0.5f * t * t * (3.5949095f * t - 2.5949095f);
    }else{
        t -= 2.0f;
        t = 0.5f * (t * t * (3.5949095f * t + 2.5949095f) + 2.0f);
    }
    return t;
}

cxFloat cxTiming::OvershootOutIn(cxFloat t)
{
    t = 2.0f * t - 1.0f;
    if (t < 0.0f){
        t = 0.5f * (t * t * (3.5949095f * t + 2.5949095f) + 1.0f);
    }else{
        t = 0.5f * (t * t * (3.5949095f * t - 2.5949095f) + 1.0f);
    }
    return t;
}

cxFloat cxTiming::ElasticIn(cxFloat t)
{
    if (t != 0.0f && t != 1.0f){
        t = t - 1.0f;
        t = -1.0f * ( exp(10.0f * t) * sin( (t - 0.075f) * MATH_PIX2 / 0.3f ) );
    }
    return t;
}

cxFloat cxTiming::ElasticOut(cxFloat t)
{
    if (t != 0.0f && t != 1.0f){
        t = exp(-10.0f * t) * sin((t - 0.075f) * MATH_PIX2 / 0.3f) + 1.0f;
    }
    return t;
}

cxFloat cxTiming::ElasticInOut(cxFloat t)
{
    if (t != 0.0f && t != 1.0f){
        t = 2.0f * t - 1.0f;
        if (t < 0.0f){
            t = -0.5f * (exp((10 * t)) * sin(((t - 0.1125f) * MATH_PIX2 / 0.45f)));
        }else{
            t = 0.5f * exp((-10 * t)) * sin(((t - 0.1125f) * MATH_PIX2 / 0.45f)) + 1.0f;
        }
    }
    return t;
}

cxFloat cxTiming::ElasticOutIn(cxFloat t)
{
    if (t != 0.0f && t != 1.0f){
        t *= 2.0f;
        if (t < 1.0f){
            t = 0.5f * (exp((-10 * t)) * sin(((t - 0.1125f) * (MATH_PIX2) / 0.45f))) + 0.5f;
        }else{
            t = 0.5f * (exp((10 *(t - 2))) * sin(((t - 0.1125f) * (MATH_PIX2) / 0.45f))) + 0.5f;
        }
    }
    return t;
}

cxFloat cxTiming::CubicIn(cxFloat t)
{
    t *= t * t;
    return t;
}

cxFloat cxTiming::CubicOut(cxFloat t)
{
    t--;
    t = t * t * t + 1;
    return t;
}

cxFloat cxTiming::CubicInOut(cxFloat t)
{
    if ((t *= 2.0f) < 1.0f){
        t = t * t * t * 0.5f;
    }else{
        t -= 2.0f;
        t = (t * t * t + 2.0f) * 0.5f;
    }
    return t;
}

cxFloat cxTiming::CubicOutIn(cxFloat t)
{
    t = (2.0f * t - 1.0f);
    t = (t * t * t + 1) * 0.5f;
    return t;
}


cxFloat cxTiming::QuarIn(cxFloat t)
{
    t *= t * t * t;
    return t;
}

cxFloat cxTiming::QuarOut(cxFloat t)
{
    t--;
    t = -(t * t * t * t) + 1.0f;
    return t;
}

cxFloat cxTiming::QuarInOut(cxFloat t)
{
    t *= 2.0f;
    if (t < 1.0f){
        t = 0.5f * t * t * t * t;
    }else{
        t -= 2.0f;
        t = -0.5f * (t * t * t * t - 2.0f);
    }
    return t;
}

cxFloat cxTiming::QuarOutIn(cxFloat t)
{
    t = 2.0f * t - 1.0f;
    if (t < 0.0f){
        t = 0.5f * (-(t * t) * t * t + 1.0f);
    }else{
        t = 0.5f * (t * t * t * t + 1.0f);
    }
    return t;
}

cxFloat cxTiming::QuinIn(cxFloat t)
{
    t *= t * t * t * t;
    return t;
}

cxFloat cxTiming::QuinOut(cxFloat t)
{
    t--;
    t = t * t * t * t * t + 1.0f;
    return t;
}

cxFloat cxTiming::QuinInOut(cxFloat t)
{
    t *= 2.0f;
    if (t < 1.0f){
        t = 0.5f * t * t * t * t * t;
    }else{
        t -= 2.0f;
        t = 0.5f * (t * t * t * t * t + 2.0f);
    }
    return t;
}

cxFloat cxTiming::QuinOutIn(cxFloat t)
{
    t = 2.0f * t - 1.0f;
    t = 0.5f * (t * t * t * t * t + 1.0f);
    return t;
}


cxFloat cxTiming::SineIn(cxFloat t)
{
    t = -(cos(t * MATH_PIOVER2) - 1.0f);
    return t;
}

cxFloat cxTiming::SineOut(cxFloat t)
{
    t = -0.5f * (cos(MATH_PI * t) - 1.0f);
    return t;
}

cxFloat cxTiming::SineInOut(cxFloat t)
{
    t = -0.5f * (cos(MATH_PI * t) - 1.0f);
    return t;
}

cxFloat cxTiming::SineOutIn(cxFloat t)
{
    if (t < 0.5f){
        t = 0.5f * sin(MATH_PI * t);
    }else{
        t = -0.5f * cos(MATH_PIOVER2 * (2.0f * t - 1.0f)) + 1.0f;
    }
    return t;
}


cxFloat cxTiming::ExpoIn(cxFloat t)
{
    if (t != 0.0f){
        t = exp(10.0f * (t - 1.0f));
    }
    return t;
}

cxFloat cxTiming::ExpoOut(cxFloat t)
{
    if (t != 1.0f){
        t = -exp(-10.0f * t) + 1.0f;
    }
    return t;
}

cxFloat cxTiming::ExpoInOut(cxFloat t)
{
    if (t != 0.0f && t != 1.0f){
        if (t < 0.5f){
            t = 0.5f * exp(10.0f * (2.0f * t - 1.0f));
        }else{
            t = -0.5f * exp(10.0f * (-2.0f * t + 1.0f)) + 1.0f;
        }
    }
    return t;
}

cxFloat cxTiming::ExpoOutIn(cxFloat t)
{
    if (t != 0.0f && t != 1.0f){
        if (t < 0.5f){
            t = -0.5f * exp(-20.0f * t) + 0.5f;
        }else{
            t = 0.5f * exp(20.0f * (t - 1.0f)) + 0.5f;
        }
    }
    return t;
}


cxFloat cxTiming::SqrtIn(cxFloat t)
{
    t = -(sqrt(1.0f - t * t) - 1.0f);
    return t;
}

cxFloat cxTiming::SqrtOut(cxFloat t)
{
    t--;
    t = sqrt(1.0f - t * t);
    return t;
}

cxFloat cxTiming::SqrtInOut(cxFloat t)
{
    t *= 2.0f;
    if (t < 1.0f){
        t = 0.5f * (-sqrt((1.0f - t * t)) + 1.0f);
    }else{
        t -= 2.0f;
        t = 0.5f * (sqrt((1.0f - t * t)) + 1.0f);
    }
    return t;
}

cxFloat cxTiming::SqrtOutIn(cxFloat t)
{
    t = 2.0f * t - 1.0f;
    if (t < 0.0f){
        t = 0.5f * sqrt(1.0f - t * t);
    }else{
        t = 0.5f * (2.0f - sqrt(1.0f - t * t));
    }
    return t;
}


cxFloat cxTiming::QuadIn(cxFloat t)
{
    t *= t;
    return t;
}

cxFloat cxTiming::QuadOut(cxFloat t)
{
    t *= -(t - 2.0f);
    return t;
}

cxFloat cxTiming::QuadInOut(cxFloat t)
{
    cxFloat tx2 = t * 2.0f;
    if (tx2 < 1.0f){
        t = 0.5f * (tx2 * tx2);
    }else{
        cxFloat tmp = tx2 - 1.0f;
        t = 0.5f * (-( tmp * (tmp - 2.0f)) + 1.0f);
    }
    return t;
}

cxFloat cxTiming::QuadOutIn(cxFloat t)
{
    if (t < 0.5f){
        t = 2.0f * t * (1.0f - t);
    }else{
        t = 1.0f + 2.0f * t * (t - 1.0f);
    }
    return t;
}

CX_CPP_END

