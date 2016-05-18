//
//  Shader.cpp
//  godcity
//
//  Created by xuhua on 7/30/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#include "Shader.h"
CX_CPP_BEGIN

#include "game.shader"

CX_IMPLEMENT(RangeShader);

RangeShader::RangeShader()
{
    
}

RangeShader::~RangeShader()
{
    
}

cxBool RangeShader::InitUniform()
{
    onepixel = GetUniform("onepixel");
    range = GetUniform("range");
    return true;
}

cxShader *RangeShader::Create(const cxFloat p,const cxPoint2F &r)
{
    RangeShader *rs = RangeShader::Create();
    rs->Init(RangeVSH, RangeFSH);
    rs->SetUniform(rs->onepixel, p);
    rs->SetUniform(rs->range, r);
    return rs;
}

CX_CPP_END

