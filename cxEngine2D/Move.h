//
//  Move.hpp
//  cxEngineTest
//
//  Created by 徐华 on 08/11/2017.
//  Copyright © 2017 xuhua. All rights reserved.
//

#ifndef Move_hpp
#define Move_hpp

#include <engine/cxPath.h>

CX_CPP_BEGIN
class Move : public cxPath
{
public:
    CX_DECLARE(Move);
protected:
    explicit Move();
    virtual ~Move();
public:
    static Move *Make();
};
CX_CPP_END

#endif /* Move_hpp */
