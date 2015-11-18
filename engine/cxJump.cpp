//
//  cxJump.cpp
//  cxEngineCore
//
//  Created by xuhua on 7/5/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#include "cxView.h"
#include "cxJump.h"

CX_CPP_BEGIN

CX_IMPLEMENT(cxJump);

cxJump::cxJump()
{
    
}

cxJump::~cxJump()
{
    
}

void cxJump::OnInit()
{
    from = View()->Position();
    prev = from;
}

void cxJump::OnStep(cxFloat dt)
{
    cxFloat time = Progress();
    cxFloat frac = fmodf(time * jumps, 1.0f );
    cxPoint2F npos = cxPoint2F::Lerp(from, to, time);
    npos.y += height * 4.0f * frac * (1.0f - frac);
    cxPoint2F diff = npos - prev;
    prev = npos;
    View()->AddPosition(diff);
}

cxAction *cxJump::Reverse()
{
    return cxJump::Create(from, height, jumps, Time());
}

cxAction *cxJump::Clone()
{
    return cxJump::Create(to, height, jumps, Time());
}

cxJump *cxJump::Create(const cxPoint2F &to,cxFloat height,cxInt jumps,cxFloat time)
{
    cxJump *rv = cxJump::Create();
    rv->to = to;
    rv->height = height;
    rv->jumps = jumps;
    rv->SetTime(time);
    return rv;
}

CX_CPP_END

