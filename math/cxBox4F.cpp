//
//  cxBox4F.cpp
//  cxEngineCore
//
//  Created by xuhua on 6/11/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#include <core/cxJson.h>
#include "cxRect4F.h"
#include "cxBox4F.h"
#include "cxPoint3F.h"
#include "cxBoxPoint3F.h"

CX_CPP_BEGIN

cxBox4F::cxBox4F()
{
    l = 0;
    r = 0;
    t = 0;
    b = 0;
}

cxBox4F::cxBox4F(const cxBox4F &v)
{
    l = v.l;
    r = v.r;
    t = v.t;
    b = v.b;
}

cxBox4F::cxBox4F(cxFloat al,cxFloat ar,cxFloat at,cxFloat ab)
{
    l = al;
    r = ar;
    t = at;
    b = ab;
}

cxBox4F::cxBox4F(cxFloat a)
{
    l = a;
    r = a;
    t = a;
    b = a;
}

cxBox4F::cxBox4F(const cxRect4F &rect)
{
    l = rect.x;
    r = rect.x + rect.w;
    t = rect.y;
    b = rect.y - rect.h;
}

cxRect4F cxBox4F::ToRect4F() const
{
    return cxRect4F(l, t, r-l, t-b);
}

cxPoint2F cxBox4F::Center() const
{
    return cxPoint2F(l + H()/2.0f, b + W()/2.0f);
}

const cxPoint2FArray cxBox4F::Points() const
{
    cxPoint2FArray rets;
    rets.Append(LT());
    rets.Append(RT());
    rets.Append(RB());
    rets.Append(LB());
    return rets;
}

cxFloat cxBox4F::X() const
{
    return l;
}

cxFloat cxBox4F::Y() const
{
    return b;
}

cxFloat cxBox4F::W() const
{
    return r - l;
}

cxFloat cxBox4F::H() const
{
    return t - b;
}

cxPoint2F cxBox4F::LT() const
{
    return cxPoint2F(l, t);
}

cxPoint2F cxBox4F::RT() const
{
    return cxPoint2F(r, t);
}

cxPoint2F cxBox4F::LB() const
{
    return cxPoint2F(l, b);
}

cxPoint2F cxBox4F::RB() const
{
    return cxPoint2F(r, b);
}

cxSize2F cxBox4F::Size() const
{
    return cxSize2F(W(), H());
}

cxBool cxBox4F::Intersect(const cxBoxPoint3F &v)
{
    return false;
}

cxBool cxBox4F::Contain(const cxBoxPoint3F &v)
{
    if(Contain(v.lt)){
        return true;
    }
    if(Contain(v.lb)){
        return true;
    }
    if(Contain(v.rt)){
        return true;
    }
    if(Contain(v.rb)){
        return true;
    }
    cxBox4F vb = v.ToMaxRect4F();
    if(vb.t >= t && vb.b <= b){
        return true;
    }
    if(vb.l <= l && vb.r >= r){
        return true;
    }
    if(Contain(vb.LT())){
        return true;
    }
    if(Contain(vb.LB())){
        return true;
    }
    if(Contain(vb.RT())){
        return true;
    }
    if(Contain(vb.RB())){
        return true;
    }
    return false;
}

cxBox4F cxBox4F::operator+(const cxPoint2F &v) const
{
    return cxBox4F(l+v.x, r+v.x, t+v.y, b+v.y);
}

cxBox4F &cxBox4F::operator+=(const cxPoint2F &v)
{
    l += v.x;
    r += v.x;
    t += v.y;
    b += v.y;
    return *this;
}

cxBox4F cxBox4F::operator*(const cxFloat v)
{
    return cxBox4F(l*v, r*v, t*v, b*v);
}

cxBox4F cxBox4F::operator*(const cxPoint2F &v) const
{
    return cxBox4F(l*v.x, r*v.x, t*v.y, b*v.y);
}

cxBox4F cxBox4F::operator*(const cxPoint2F &v)
{
    return cxBox4F(l*v.x, r*v.x, t*v.y, b*v.y);
}

cxBox4F cxBox4F::operator/(const cxFloat v)
{
    return cxBox4F(l/v, r/v, t/v, b/v);
}

cxBox4F cxBox4F::operator/(const cxPoint2F &v) const
{
    return cxBox4F(l/v.x, r/v.x, t/v.y, b/v.y);
}

cxBox4F cxBox4F::operator/(const cxPoint2F &v)
{
    return cxBox4F(l/v.x, r/v.x, t/v.y, b/v.y);
}


cxBool cxBox4F::operator==(const cxBox4F &v) const
{
    if(!cxFloatIsEqual(l, v.l)){
        return false;
    }
    if(!cxFloatIsEqual(r, v.r)){
        return false;
    }
    if(!cxFloatIsEqual(t, v.t)){
        return false;
    }
    if(!cxFloatIsEqual(b, v.b)){
        return false;
    }
    return true;
}

cxBool cxBox4F::operator!=(const cxBox4F &v) const
{
    if(!cxFloatIsEqual(l, v.l)){
        return true;
    }
    if(!cxFloatIsEqual(r, v.r)){
        return true;
    }
    if(!cxFloatIsEqual(t, v.t)){
        return true;
    }
    if(!cxFloatIsEqual(b, v.b)){
        return true;
    }
    return false;
}

cxJson *cxBox4F::ToJson()
{
    cxJson *json = cxJson::Create()->Object();
    json->Set("l", l);
    json->Set("r", r);
    json->Set("t", t);
    json->Set("b", b);
    return json;
}

cxBox4F::cxBox4F(const cxJson *json)
{
    CX_ASSERT(json != nullptr && json->IsObject(), "json format error");
    l = json->Get("l", l);
    r = json->Get("r", r);
    t = json->Get("t", t);
    b = json->Get("b", b);
}

cxBool cxBox4F::Contain(const cxBox4F &p)
{
    return (l <= p.l && r >= p.r && b <= p.b && t >= p.t);
}

cxBool cxBox4F::Contain(const cxPoint2F &p)
{
    return (p.x >= l && p.x <= r && p.y >= b && p.y <= t);
}

cxBool cxBox4F::Contain(const cxPoint3F &p)
{
    return (p.x >= l && p.x <= r && p.y >= b && p.y <= t);
}

cxBox4I::cxBox4I()
{
    l = 0;
    r = 0;
    t = 0;
    b = 0;
}

cxBox4I::cxBox4I(const cxBox4I &v)
{
    l = v.l;
    r = v.r;
    t = v.t;
    b = v.b;
}

cxBox4I::cxBox4I(const cxJson *json)
{
    CX_ASSERT(json != nullptr && json->IsObject(), "json format error");
    l = json->Get("l", l);
    r = json->Get("r", r);
    t = json->Get("t", t);
    b = json->Get("b", b);
}

cxBox4I::cxBox4I(cxInt a)
{
    l = a;
    r = a;
    t = a;
    b = a;
}

cxBox4I::cxBox4I(cxInt al,cxInt ar,cxInt at,cxInt ab)
{
    l = al;
    r = ar;
    t = at;
    b = ab;
}

CX_CPP_END

