//
//  cxPoints.cpp
//  cxEngineCore
//
//  Created by xuhua on 26/03/2017.
//  Copyright © 2017 xuhua. All rights reserved.
//

#include "cxView.h"
#include "cxPoints.h"

CX_CPP_BEGIN

CX_IMPLEMENT(cxPoints);

cxPoints::cxPoints()
{
    speed = 100;
    Forever();
}

cxPoints::~cxPoints()
{
    
}

cxPoint2F cxPoints::ToPos(const cxPoint2I &v)
{
    return cxPoint2F(v.x * 50, v.y * 50);
}

cxPoint2I cxPoints::ToIdx(const cxPoint2F &v)
{
    return cxPoint2I(v.x/50.0f, v.y/50.0f);
}

void cxPoints::OnStep(cxFloat dt)
{
    cxAction::OnStep(dt);
    if(IsExit()){
        return;
    }
    cxView *pv = View();
    cxPoint2F wp = pv->Position();
    wp += angle * (dt * speed);
    if(!pv->SetPosition(np, wp)){
        return;
    }
    if(nextPoint(++idx)){
        Exit(true);
        return;
    }
}

void cxPoints::SetSpeed(const cxFloat &v)
{
    speed = v;
}

const cxPoint2IArray &cxPoints::Points()
{
    return ps;
}

const cxInt cxPoints::Index()
{
    return idx;
}

const cxFloat cxPoints::Speed()
{
    return speed;
}

void cxPoints::SetPoints(const cxPoint2IArray &v,cxBool combine)
{
    ps = combine ? v.CombineAngle() : v;
    if(ps.Size() == 0){
        Exit(true);
        return;
    }
}

// if exit return true
cxBool cxPoints::OnArrive(const cxInt &v)
{
    return false;
}

cxBool cxPoints::nextPoint(cxInt i)
{
    cxBool ret = OnArrive(i);
    if(i >= ps.Size()){
        return true;
    }
    cxPoint2F cp = View()->Position();
    np = ToPos(ps.At(i));
    cxFloat av = cp.Angle(np);
    angle.x = cosf(av);
    angle.y = sinf(av);
    return ret;
}

void cxPoints::OnInit()
{
    cxAction::OnInit();
    idx = -1;
    if(ps.IsEmpty()){
        Exit(true);
        return;
    }
    if(nextPoint(++idx)){
        Exit(true);
        return;
    }
}

CX_CPP_END

