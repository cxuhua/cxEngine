//
// cxBoxBody.h
// cxEngineTest
//
// Created by 徐华 on 2019/2/28.
//Copyright © 2019 xuhua. All rights reserved.
//

#ifndef cxBoxBody_h
#define cxBoxBody_h

#include <engine/cxView.h>

CX_CPP_BEGIN

class cxBoxBody : public cxView
{
public:
    CX_DECLARE(cxBoxBody);
protected:
    explicit cxBoxBody();
    virtual ~cxBoxBody();
};

CX_CPP_END

#endif
