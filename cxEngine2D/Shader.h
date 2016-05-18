//
//  Shader.h
//  godcity
//
//  Created by xuhua on 7/30/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#ifndef godcity_Shader_h
#define godcity_Shader_h

#include <engine/cxShader.h>

CX_CPP_BEGIN

class RangeShader : public cxShader
{
public:
    CX_DECLARE(RangeShader);
protected:
    explicit RangeShader();
    virtual ~RangeShader();
protected:
    cxBool InitUniform();
private:
    glUint onepixel;
    glUint range;
public:
    static cxShader *Create(const cxFloat p,const cxPoint2F &r);
};

CX_CPP_END

#endif


