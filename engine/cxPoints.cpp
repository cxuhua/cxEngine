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
    min = 0.0f;
    speed = 100.0f;
    Forever();
}

cxPoints::~cxPoints()
{
    
}

cxAction *cxPoints::Reverse()
{
    cxPoints *ret = cxPoints::Create();
    cxPoint2FArray rps;
    for(cxInt i=ps.Size()-1; i >= 0; i--){
        rps.Append(ps.At(i));
    }
    ret->SetPoints(rps);
    ret->SetSpeed(speed);
    ret->SetMin(min);
    return ret;
}

cxAction *cxPoints::Clone()
{
    cxPoints *ret = cxPoints::Create();
    ret->SetPoints(ps,false);
    ret->SetSpeed(speed);
    ret->SetMin(min);
    return ret;
}

void cxPoints::OnStep(cxFloat dt)
{
    cxAction::OnStep(dt);
    if(IsExit()){
        return;
    }
    cxView *pv = View();
    cxPoint2F wp = pv->Position() + angle * (dt * speed);
    if(!pv->SetPosition(np, wp, min)){
        return;
    }
    if(nextPoint(++idx)){
        Exit(true);
        return;
    }
}

void cxPoints::SetMin(const cxFloat &v)
{
    min = v;
}

void cxPoints::SetSpeed(const cxFloat &v)
{
    speed = v;
}

const cxPoint2FArray &cxPoints::Points()
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

void cxPoints::SetPoints(const cxPoint2FArray &v,cxBool combine)
{
    ps = v;
}

// if exit return true
cxBool cxPoints::OnArrive(const cxInt &v)
{
    return false;
}

cxBool cxPoints::nextPoint(cxInt i)
{
    if(i >= ps.Size()){
        return true;
    }
    cxPoint2F cp = View()->Position();
    np = At(i);
    cxFloat av = cp.Angle(np);
    angle.x = cosf(av);
    angle.y = sinf(av);
    return OnArrive(i);
}

cxPoint2F cxPoints::At(cxInt i)
{
    CX_ASSERT(i >= 0 && i < ps.Size(), "i out bound");
    return ps.At(i);
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

