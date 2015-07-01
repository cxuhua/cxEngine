//
//  cxRect4F.cpp
//  cxEngineCore
//
//  Created by xuhua on 6/11/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//


#include <core/cxJson.h>
#include "cxBox4F.h"
#include "cxRect4F.h"

CX_CPP_BEGIN

cxRect4F::cxRect4F()
{
    x = 0;
    y = 0;
    w = 0;
    h = 0;
}

cxRect4F::cxRect4F(const cxBox4F &box)
{
    w = box.r - box.l;
    h = box.t - box.b;
    x = box.l;
    y = box.t;
}

cxRect4F::cxRect4F(const cxRect4F &v)
{
    x = v.x;
    y = v.y;
    w = v.w;
    h = v.h;
}

cxRect4F::cxRect4F(const cxJson *json)
{
    CX_ASSERT(json != nullptr && json->IsObject(), "json format error");
    x = json->Get("x", x);
    y = json->Get("y", y);
    w = json->Get("w", w);
    h = json->Get("h", h);
}

cxJson *cxRect4F::ToJson()
{
    cxJson *json = cxJson::Create()->Object();
    json->Set("x", x);
    json->Set("y", y);
    json->Set("w", w);
    json->Set("h", h);
    return json;
}

cxBool cxRect4F::operator==(const cxRect4F &v) const
{
    if(!cxFloatIsEqual(x, v.x)){
        return false;
    }
    if(!cxFloatIsEqual(y, v.y)){
        return false;
    }
    if(!cxFloatIsEqual(w, v.w)){
        return false;
    }
    if(!cxFloatIsEqual(h, v.h)){
        return false;
    }
    return true;
}

cxBool cxRect4F::operator!=(const cxRect4F &v) const
{
    if(!cxFloatIsEqual(x, v.x)){
        return true;
    }
    if(!cxFloatIsEqual(y, v.y)){
        return true;
    }
    if(!cxFloatIsEqual(w, v.w)){
        return true;
    }
    if(!cxFloatIsEqual(h, v.h)){
        return true;
    }
    return false;
}

cxRect4F::cxRect4F(cxFloat ax,cxFloat ay,cxFloat aw,cxFloat ah)
{
    x = ax;
    y = ay;
    w = aw;
    h = ah;
}

cxRect4F::cxRect4F(const cxPoint2F &acpos,const cxSize2F &asize)
{
    x = acpos.x;
    y = acpos.y;
    w = asize.w;
    h = asize.h;
}

cxBox4F cxRect4F::ToBox4F() const
{
    return cxBox4F(x, x + w, y, y - h);
}

cxBool cxRect4F::Contain(const cxPoint2F &p)
{
    cxBox4F box = ToBox4F();
    return box.Contain(p);
}

CX_CPP_END

