//
//  cxSpatial.cpp
//  cxEngineCore
//
//  Created by xuhua on 7/13/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//


#include "cxSpatial.h"

CX_CPP_BEGIN

CX_IMPLEMENT(cxSpatial);

cxSpatial::cxSpatial()
{
    
}

cxSpatial::~cxSpatial()
{
    
}

cxSpatial *cxSpatial::Clear()
{
    types.clear();
    return this;
}

cxInt cxSpatial::Each(const cxPoint2F &cp,std::function<cxBool(const cxPoint2F &p)> func)
{
    cxInt c = 0;
    spatial::neighbor_iterator<SpatialType> it = spatial::neighbor_begin(types, cp);
    while(it != spatial::neighbor_end(types, cp)){
        const cxPoint2F &p = *it;
        if(!func(p)){
            break;
        }
        c++;
    }
    return c;
}

cxSpatial *cxSpatial::Append(const cxPoint2F &pos)
{
    types.insert(pos);
    return this;
}

cxSpatial *cxSpatial::Remove(const cxPoint2F &pos)
{
    SpatialType::iterator it = types.find(pos);
    if(it == types.end()){
        return this;
    }
    types.erase(it);
    return this;
}

CX_CPP_END

