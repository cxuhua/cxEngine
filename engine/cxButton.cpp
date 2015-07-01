//
//  cxButton.cpp
//  cxEngineCore
//
//  Created by xuhua on 6/28/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//


#include "cxButton.h"

CX_CPP_BEGIN

CX_IMPLEMENT(cxButton);

cxButton::cxButton()
{
    isenable = true;
    ispass = false;
    ispress = false;
}

cxButton::~cxButton()
{
    
}

void cxButton::SetIsPass(cxBool v)
{
    ispass = v;
}

void cxButton::SetIsEnable(cxBool v)
{
    isenable = v;
}

cxBool cxButton::OnDispatch(const cxTouchable *e)
{
    if(e->TouchCount() != 1 || !isenable){
        return false;
    }
    const cxTouchPoint *ep = e->TouchPoint(0);
    cxHitInfo hit = HitTest(ep->wp);
    if(!hit.hited && ispress){
        onRelease.Fire(this);
        ispress = false;
    }
    if(!hit.hited){
        return false;
    }
    if(ep->IsBegan()){
        onPress.Fire(this);
        ispress = true;
        return !ispass;
    }
    if(!ispress){
        return false;
    }
    if(ep->IsMoved()){
        onMove.Fire(this);
        return !ispass;
    }
    if(ep->IsEnded()){
        onRelease.Fire(this);
        if(ep->IsTap())onTap.Fire(this);
        ispress = false;
        return !ispass;
    }
    return false;
}

CX_CPP_END

