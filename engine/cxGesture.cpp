//
//  cxGesture.cpp
//  cxEngineCore
//
//  Created by 徐华 on 2018/8/4.
//  Copyright © 2018 xuhua. All rights reserved.
//

#include <core/cxUtil.h>
#include "cxGesture.h"
#include "cxSprite.h"

CX_CPP_BEGIN

CX_IMPLEMENT(cxGesture);

cxGesture::cxGesture()
{
    tapCount = 0;
    flags = cxTouchPoint::FlagsGestureTypeSwipe|cxTouchPoint::FlagsGestureTypeDoubleTap;
    touchIsPass = false;
    swipeischeck = false;
    swipetrigger = false;
    swipesampletime = 0.01f;
    swipesampledistance = 3.0f;
    swipesamplesize = 3.0f;
    swipeminspeed = 2100;
}

cxGesture::~cxGesture()
{
    
}

cxGesture *cxGesture::SetSwipeAttr(cxFloat stime,cxFloat sdis,cxFloat snum,cxFloat mspeed)
{
    swipesampletime = stime;
    swipesampledistance = sdis;
    swipesamplesize = snum;
    swipeminspeed = mspeed;
    return this;
}

cxGesture *cxGesture::SetFlags(cxUInt v)
{
    flags |= v;
    return this;
}

cxGesture *cxGesture::DelFlags(cxUInt v)
{
    flags &= ~v;
    return this;
}

void cxGesture::resetSwipe()
{
    swipetrigger = false;
    swipeischeck = false;
    swipePoints.clear();
}

cxBool cxGesture::computeSwipe()
{
    cxInt size = swipePoints.size();
    if(size < swipesamplesize){
        return !touchIsPass;;
    }
    //速度集合
    speeds.clear();
    angles.clear();
    swipePoint pb = swipePoints.at(0);
    swipePoint p0 = pb;
    for(cxInt i= 1;i < size;i++){
        const swipePoint &tmp = swipePoints.at(i);
        cxFloat time = tmp.time - pb.time;
        //时间太小取下一点
        if(time < swipesampletime){
            continue;
        }
        cxFloat dis = tmp.pos.Distance(pb.pos);
        //距离太小取下一点
        if(dis < swipesampledistance){
            continue;
        }
        cxFloat angle = p0.pos.Angle(tmp.pos);
        angle = cxModDegrees(cxRadiansToDegrees(angle));
        angle = cxModDegrees(angle + 45.0f);
        angles.push_back(angle);
        //保存速度和角度
        cxFloat speed = dis / time;
        speeds.push_back(speed);
        pb = tmp;
    }
    //采样到3个点以上
    if(speeds.size() < swipesamplesize){
        return !touchIsPass;
    }
    //计算平均速度和角度
    cxFloat v = 0;
    cxFloat a = 0;
    for(cxInt i=0;i<speeds.size();i++){
        v += speeds.at(i);
        a += angles.at(i);
    }
    cxFloat sp = v / (cxFloat)speeds.size();
    cxFloat ap = a / (cxFloat)speeds.size();
    //平均速度限制
    if(sp < swipeminspeed){
        return !touchIsPass;
    }
    //清楚并禁止继续采样
    swipePoints.clear();
    swipeischeck = false;
    swipetrigger = true;
    //获取4个方向
    cxInt type = ap / 90.0f;
    OnSwipe(1 << type,sp);
    return !touchIsPass;
}

cxBool cxGesture::checkSwipe(const cxTouchPoint *ep)
{
    if(ep->IsTap()){
        return false;
    }
    cxHitInfo hit = HitTest(ep->wp);
    if(!hit.hited){
        return false;
    }
    if(ep->IsBegan()){
        *ep->Flags() |= cxTouchPoint::FlagsGestureTypeSwipe;
        swipePoints.clear();
        swipeischeck = true;
        swipetrigger = false;
        swipePoint p;
        p.time = cxUtil::Timestamp();
        p.pos = hit.point;
        swipePoints.push_back(p);
        OnSwipeBegin();
        return true;
    }
    if(!(*ep->Flags() & cxTouchPoint::FlagsGestureTypeSwipe)){
        return false;
    }
    if(ep->IsMoved() && swipeischeck){
        swipePoint p;
        p.time = cxUtil::Timestamp();
        p.pos = hit.point;
        swipePoints.push_back(p);
        return computeSwipe();
    }
    //如果触发过并且结束
    if(ep->IsEnded() && swipetrigger && !swipeischeck){
        OnSwipeEnd();
    }
    if(ep->IsEnded()){
        resetSwipe();
    }
    return !touchIsPass;
}

void cxGesture::OnDoubleTap()
{
    onDoubleTap.Fire(this);
}

void cxGesture::OnSwipeBegin()
{
    onSwipeBegin.Fire(this);
}

void cxGesture::OnSwipeEnd()
{
    onSwipeEnd.Fire(this);
}

cxBool cxGesture::OnDispatch(const cxTouchable *e)
{
    if(e->TouchCount() == 1 && (flags & cxTouchPoint::FlagsGestureTypeDoubleTap)){
        const cxTouchPoint *ep = e->TouchPoint(0);
        cxHitInfo hit = HitTest(ep->wp);
        if(hit.hited && ep->IsEnded() && ep->IsTap()){
            cxBool ret = false;
            onTap.Fire(this);
            tapTime[tapCount] = cxUtil::Timestamp();
            tapCount++;
            if(tapCount >= 2 && tapTime[tapCount-1] - tapTime[tapCount-2] < 0.8){
                OnDoubleTap();
                tapCount = 0;
                ret = true;
            }
            if(tapCount >= sizeof(tapTime)/sizeof(cxDouble)){
                tapCount = 0;
            }
            if(ret){
                resetSwipe();
                return true;
            }
        }
    }
    if(e->TouchCount() == 1 && (flags & cxTouchPoint::FlagsGestureTypeSwipe)){
        const cxTouchPoint *ep = e->TouchPoint(0);
        if(checkSwipe(ep)){
            return true;
        }
    }
    return false;
}

void cxGesture::OnSwipe(SwipeType type,cxFloat speed)
{
    onSwipe.Fire(this, type, speed);
}
CX_CPP_END

