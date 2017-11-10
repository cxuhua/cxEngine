//
//  Move.hpp
//  cxEngineTest
//
//  Created by 徐华 on 08/11/2017.
//  Copyright © 2017 xuhua. All rights reserved.
//

#ifndef Move_hpp
#define Move_hpp

#include <engine/cxPoints.h>
#include "Point.h"

CX_CPP_BEGIN

class Move : public cxPoints
{
public:
    CX_DECLARE(Move);
protected:
    explicit Move();
    virtual ~Move();
private:
    cxAny map;
    PointArray mps;
protected:
    virtual cxBool OnArrive();
public:
    const Point GetPoint(cxInt idx);
    void AppendPoint(const Point &p);
    void AppendPoints(const PointArray &ps);
    static Move *Create(cxAny pmap);
};
CX_CPP_END

#endif /* Move_hpp */
