//
//  Body.h
//  runner
//
//  Created by xuhua on 11/12/15.
//  Copyright © 2015 xuhua. All rights reserved.
//

#ifndef runner_Body_h
#define runner_Body_h

#include "cxObject.h"

CX_CPP_BEGIN

class Body : public cxObject
{
public:
    CX_DECLARE(Body);
protected:
    explicit Body();
    virtual ~Body();
private:
public:
};

CX_CPP_END

#endif


