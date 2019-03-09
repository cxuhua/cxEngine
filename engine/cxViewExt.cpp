//
//  cxViewExt.cpp
//  cxEngineCore
//
//  Created by 徐华 on 2019/3/8.
//  Copyright © 2019 xuhua. All rights reserved.
//

#include "cxViewExt.h"

CX_CPP_BEGIN

CX_IMPLEMENT(cxViewExt);

cxViewExt::cxViewExt()
{
    
}

cxViewExt::~cxViewExt()
{
    
}

void cxViewExt::OnAttchView(cxView *pv)
{
    pview = pv;
}

void cxViewExt::Attach(cxView *pv)
{
    CX_ASSERT(pv != nullptr, "pv args error");
    pv->SetExt(this);
}

void cxViewExt::OnUpdate(const cxFloat &dt)
{
    
}

void cxViewExt::OnLeave()
{
    
}

//return true view will skip setangle
bool cxViewExt::SetAngle(const cxFloat &v)
{
    return false;
}

//return true view will skip setposition
bool cxViewExt::SetPosition(const cxPoint2F &v)
{
    return false;
}


CX_CPP_END

