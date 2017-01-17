//
//  cxColor4F.cpp
//  cxEngineCore
//
//  Created by xuhua on 6/11/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//


#include <core/cxJson.h>
#include "cxColor4F.h"

CX_CPP_BEGIN

cxColor4FRange::cxColor4FRange()
{
    v = cxColor4F::WHITE;
    r = cxColor4F(0, 0, 0, 0);
}

cxColor4FRange::cxColor4FRange(const cxColor4FRange &av)
{
    v = av.v;
    r = av.r;
}

cxColor4FRange::cxColor4FRange(const cxColor4F &av,const cxColor4F &ar)
{
    v = av;
    r = ar;
}

cxColor4FRange::cxColor4FRange(const cxColor4F &av)
{
    v = av;
    r = cxColor4F(0,0,0,0);
}

const cxColor4F cxColor4FRange::ToValue() const
{
    cxColor4F color;
    color.r = v.r + r.r * CX_RAND_11f();
    color.g = v.g + r.g * CX_RAND_11f();
    color.b = v.b + r.b * CX_RAND_11f();
    color.a = v.a + r.a * CX_RAND_11f();
    return color;
}

cxColor4B::cxColor4B()
{
    r = 255;
    g = 255;
    b = 255;
    a = 255;
}

cxColor4B::cxColor4B(const cxColor4F &v)
{
    r = 255 * v.r;
    g = 255 * v.g;
    b = 255 * v.b;
    a = 255 * v.a;
}

const cxColor4F cxColor4F::WHITE   = cxColor4F(1.00f, 1.00f, 1.00f, 1.00f);
const cxColor4F cxColor4F::YELLOW  = cxColor4F(1.00f, 1.00f, 0.00f, 1.00f);
const cxColor4F cxColor4F::BLUE    = cxColor4F(0.00f, 0.00f, 1.00f, 1.00f);
const cxColor4F cxColor4F::GREEN   = cxColor4F(0.00f, 1.00f, 0.00f, 1.00f);
const cxColor4F cxColor4F::RED     = cxColor4F(1.00f, 0.00f, 0.00f, 1.00f);
const cxColor4F cxColor4F::MAGENTA = cxColor4F(1.00f, 0.00f, 1.00f, 1.00f);
const cxColor4F cxColor4F::BLACK   = cxColor4F(0.00f, 0.00f, 0.00f, 1.00f);
const cxColor4F cxColor4F::ORANGE  = cxColor4F(1.00f, 0.50f, 0.00f, 1.00f);
const cxColor4F cxColor4F::GRAY    = cxColor4F(0.65f, 0.65f, 0.65f, 1.00f);
const cxColor4F cxColor4F::PURPLE  = cxColor4F(0.63f, 0.13f, 0.94f, 1.00f);

cxColor4B cxColor4F::ToColor4B()
{
    return cxColor4B(*this);
}

cxColor4F::cxColor4F(const cxColor4F &v)
{
    r = v.r;
    g = v.g;
    b = v.b;
    a = v.a;
}

cxColor4F::cxColor4F(cxUInt32 color)
{
    cxInt ir = ((color >> 24) & 0xFF);
    cxInt ig = ((color >> 16) & 0xFF);
    cxInt ib = ((color >> 8) & 0xFF);
    cxInt ia = (color & 0xFF);
    r = (cxFloat)ir/255.0f;
    g = (cxFloat)ig/255.0f;
    b = (cxFloat)ib/255.0f;
    a = (cxFloat)ia/255.0f;
}

cxUInt32 cxColor4F::ToInt() const
{
    cxInt ir = r * 255;
    cxInt ig = g * 255;
    cxInt ib = b * 255;
    cxInt ia = a * 255;
    return  (ir << 24) || (ig << 16) || (ib << 8) || ia;
}

cxJson *cxColor4F::ToJson()
{
    cxJson *json = cxJson::Create()->Object();
    json->Set("r", r);
    json->Set("g", g);
    json->Set("b", b);
    json->Set("a", a);
    return json;
}

cxColor4F parseHexColor(cchars color)
{
    cxFloat r = 1.0f;
    cxFloat g = 1.0f;
    cxFloat b = 1.0f;
    cxFloat a = 1.0f;
    cxInt len = (cxInt)strlen(color);
    if(len >= 2){
        r = (cxHexCharToInt(color[0]) << 4) | cxHexCharToInt(color[1]);
        r = r/255.0f;
    }
    if(len >= 4){
        g = (cxHexCharToInt(color[2]) << 4) | cxHexCharToInt(color[3]);
        g = g/255.0f;
    }
    if(len >= 6){
        b = (cxHexCharToInt(color[4]) << 4) | cxHexCharToInt(color[5]);
        b = b/255.0f;
    }
    if(len >= 8){
        a = (cxHexCharToInt(color[6]) << 4) | cxHexCharToInt(color[7]);
        a = a/255.0f;
    }
    return cxColor4F(r, g, b, a);
}

cxColor4F parseArrayColor(const cxArray *ps)
{
    cxFloat r = 1.0f;
    cxFloat g = 1.0f;
    cxFloat b = 1.0f;
    cxFloat a = 1.0f;
    cxInt len = ps->Size();
    if(len >= 1){
        r = ps->At(0)->To<cxStr>()->ToFloat();
    }
    if(len >= 2){
        g = ps->At(1)->To<cxStr>()->ToFloat();
    }
    if(len >= 3){
        b = ps->At(2)->To<cxStr>()->ToFloat();
    }
    if(len >= 4){
        a = ps->At(3)->To<cxStr>()->ToFloat();
    }
    return cxColor4F(r, g, b, a);
}

cxColor4F::cxColor4F(cchars v)
{
    if(!cxStr::IsOK(v)){
        *this = cxColor4F::WHITE;
        return;
    }
    // hex color(#FF0000FF)
    if(v[0] == '#'){
        *this = parseHexColor(v + 1);
        return;
    }
    // int color(11121212)
    if(cxStr::IsInt(v)){
        *this = cxColor4F((cxUInt32)atoi(v));
        return;
    }
    // array color(1.0,1.0,1.0,1.0)
    const cxArray *ps = cxStr::Split(v, ",");
    if(!ps->IsEmpty()){
        *this = parseArrayColor(ps);
        return;
    }
    *this = cxColor4F::WHITE;
}

cxColor4F::cxColor4F(const cxJson *json)
{
    CX_ASSERT(json != nullptr, "json format error");
    if(json->IsObject()){
        r = json->Get("r", r);
        g = json->Get("g", g);
        b = json->Get("b", b);
        a = json->Get("a", a);
    }else if(json->IsInt()){
        cxColor4F(json->ToInt());
    }else if(json->IsString()){
        const cxStr *color = json->ToStr();
        *this = color->ToColor4F();
    }else{
        CX_ASSERT(false, "color format error");
    }
}

cxBool cxColor4F::operator==(const cxColor4F &v) const
{
    if(!cxFloatIsEqual(r, v.r)){
        return false;
    }
    if(!cxFloatIsEqual(g, v.g)){
        return false;
    }
    if(!cxFloatIsEqual(b, v.b)){
        return false;
    }
    if(!cxFloatIsEqual(a, v.a)){
        return false;
    }
    return true;
}

cxBool cxColor4F::operator!=(const cxColor4F &v) const
{
    if(!cxFloatIsEqual(r, v.r)){
        return true;
    }
    if(!cxFloatIsEqual(g, v.g)){
        return true;
    }
    if(!cxFloatIsEqual(b, v.b)){
        return true;
    }
    if(!cxFloatIsEqual(a, v.a)){
        return true;
    }
    return false;
}

cxColor4F cxColor4F::operator-() const
{
    return cxColor4F(-r, -g, -b, -a);
}

cxColor4F::cxColor4F()
{
    r = 1.0f;
    g = 1.0f;
    b = 1.0f;
    a = 1.0f;
}

cxColor4F::cxColor4F(cxFloat ar,cxFloat ag,cxFloat ab,cxFloat aa)
{
    r = ar;
    g = ag;
    b = ab;
    a = aa;
}

cxColor4F::cxColor4F(cxFloat ar,cxFloat ag,cxFloat ab)
{
    r = ar;
    g = ag;
    b = ab;
    a = 1.0f;
}

void cxColor4F::SetAlpha(cxFloat aa)
{
    a = aa;
}


void cxColor4F::SetRGB(cxFloat ar,cxFloat ag,cxFloat ab)
{
    r = ar;
    g = ag;
    b = ab;
}

cxColor4F cxColor4F::operator+(const cxColor4F &v) const
{
    return cxColor4F(r+v.r, g+v.g, b+v.b, a+v.a);
}

cxColor4F cxColor4F::operator+(const cxFloat v) const
{
    return cxColor4F(r+v, g+v, b+v, a+v);
}

cxColor4F cxColor4F::operator-(const cxColor4F &v) const
{
    return cxColor4F(r-v.r, g-v.g, b-v.b, a-v.a);
}

cxColor4F cxColor4F::operator-(const cxFloat v) const
{
    return cxColor4F(r-v, g-v, b-v, a-v);
}

cxColor4F cxColor4F::operator*(const cxColor4F &v) const
{
    return cxColor4F(r*v.r, g*v.g, b*v.b, a*v.a);
}

cxColor4F cxColor4F::operator*(const cxFloat v) const
{
    return cxColor4F(r*v, g*v, b*v, a*v);
}

cxColor4F cxColor4F::operator/(const cxColor4F &v) const
{
    return cxColor4F(r/v.r, g/v.g, b/v.b, a/v.a);
}

cxColor4F cxColor4F::operator/(const cxFloat v) const
{
    return cxColor4F(r/v, g/v, b/v, a/v);
}

cxColor4F &cxColor4F::operator+=(const cxColor4F &v)
{
    r += v.r;
    g += v.g;
    b += v.b;
    a += v.a;
    return *this;
}

cxColor4F &cxColor4F::operator+=(const cxFloat v)
{
    r += v;
    g += v;
    b += v;
    a += v;
    return *this;
}

cxColor4F &cxColor4F::operator-=(const cxColor4F &v)
{
    r -= v.r;
    g -= v.g;
    b -= v.b;
    a -= v.a;
    return *this;
}

cxColor4F &cxColor4F::operator-=(const cxFloat v)
{
    r -= v;
    g -= v;
    b -= v;
    a -= v;
    return *this;
}

cxColor4F &cxColor4F::operator*=(const cxColor4F &v)
{
    r *= v.r;
    g *= v.g;
    b *= v.b;
    a *= v.a;
    return *this;
}

cxColor4F &cxColor4F::operator*=(const cxFloat v)
{
    r *= v;
    g *= v;
    b *= v;
    a *= v;
    return *this;
}

cxColor4F &cxColor4F::operator/=(const cxColor4F &v)
{
    r /= v.r;
    g /= v.g;
    b /= v.b;
    a /= v.a;
    return *this;
}

cxColor4F &cxColor4F::operator/=(const cxFloat v)
{
    r /= v;
    g /= v;
    b /= v;
    a /= v;
    return *this;
}

CX_CPP_END

