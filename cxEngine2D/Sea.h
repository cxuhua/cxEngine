//
//  Sea.h
//  cxEngineTest
//
//  Created by xuhua on 7/21/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#ifndef cxEngineTest_Sea_h
#define cxEngineTest_Sea_h

#include <engine/cxShader.h>

CX_CPP_BEGIN

class Sea : public cxShader
{
public:
    CX_DECLARE(Sea);
protected:
    explicit Sea();
    virtual ~Sea();
protected:
    cxBool InitUniform();
    cxBool InitAttrib();
    
    glUint u_time;
    glUint u_1DivLevelWidth;
    glUint u_1DivLevelHeight;
    glUint WAVE_HEIGHT;
    glUint WAVE_MOVEMENT;
    
    glUint SHORE_DARK;
    glUint SHORE_LIGHT;
    glUint SEA_DARK;
    glUint SEA_LIGHT;
    
    glUint u_lightPos;
private:
public:
    void Using() const;
};

CX_CPP_END

#endif


