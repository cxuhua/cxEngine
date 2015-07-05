//
//  cxSequence.cpp
//  cxEngineCore
//
//  Created by xuhua on 7/5/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#include "cxView.h"
#include "cxSequence.h"

CX_CPP_BEGIN

CX_IMPLEMENT(cxSequence);

cxSequence::cxSequence()
{
    
}

cxSequence::~cxSequence()
{
    
}

void cxSequence::OnTime(const cxTimePoint *tp)
{
    cxAction *pav = tp->Object()->To<cxAction>();
    View()->Append(pav);
    cxTimeLine::OnTime(tp);
}

cxSequence *cxSequence::Append(cxAction *pav)
{
    CX_ASSERT(pav != nullptr, "args error");
    cxTimePoint *tp = cxTimeLine::Append(pav->Time());
    tp->SetObject(pav);
    return this;
}

CX_CPP_END

