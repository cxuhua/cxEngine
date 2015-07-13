//
//  cxSpatial.h
//  cxEngineCore
//
//  Created by xuhua on 7/13/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#ifndef cxEngineCore_cxSpatial_h
#define cxEngineCore_cxSpatial_h

#include <map>
#include <spatial/idle_point_multiset.hpp>
#include <spatial/neighbor_iterator.hpp>
#include <math/cxPoint2F.h>
#include "cxObject.h"

CX_CPP_BEGIN

class cxSpatial : public cxObject
{
public:
    CX_DECLARE(cxSpatial);
protected:
    explicit cxSpatial();
    virtual ~cxSpatial();
private:
    typedef spatial::idle_point_multiset<2,cxPoint2F> SpatialType;
    SpatialType types;
public:
    cxSpatial *Append(const cxPoint2F &pos);
    cxSpatial *Remove(const cxPoint2F &pos);
    cxInt Each(const cxPoint2F &cp,std::function<cxBool(const cxPoint2F &p)> func);
    cxSpatial *Clear();
};

CX_CPP_END

#endif


