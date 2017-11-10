//
//  Point.cpp
//  cxEngineCore
//
//  Created by xuhua on 6/1/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#include <math/cxPoint2F.h>
#include "Point.h"

CX_CPP_BEGIN

Point::Point()
{
    x = 0;
    y = 0;
    a = 0;
}

Point::Point(cxInt ax,cxInt ay,cxInt aa)
{
    x = ax;
    y = ay;
    a = aa;
}

Point::Point(cxPoint2I ap,cxInt aa)
{
    x = ap.x;
    y = ap.y;
    a = aa;
}

const cxPoint2F Point::ToFloat(const cxFloat &add) const
{
    return cxPoint2F(x,y) + add;
}

cxPoint2I Point::ToPoint2I()
{
    return cxPoint2I(x, y);
}

const cxPoint2I Point::ToPoint2I() const
{
    return cxPoint2I(x, y);
}

const cxFloat Point::Angle() const
{
    return ToFloat().Angle();
}

const cxFloat Point::Angle(const Point &p) const
{
    return ToFloat().Angle(p.ToFloat());
}

const cxBool Point::IsJump() const
{
    return (a & ATTR_IS_JUMP) != 0;
}

const cxBool Point::IsFall() const
{
    return (a & ATTR_IS_FALL) != 0;
}

const cxBool Point::IsKeep() const
{
    return (a & ATTR_IS_KEEP) != 0;
}

const cxPoint2F Point::ToFloat() const
{
    return cxPoint2F(x,y);
}

cxBool Point::operator==(const Point &v) const
{
    return x == v.x && y == v.y;
}

cxBool Point::operator!=(const Point &v) const
{
    return x != v.x || y != v.y;
}


PointArray::PointArray()
{
    
}

PointArray::~PointArray()
{
    
}

cxBool floatIsEqu(cxFloat a,cxFloat b,cxFloat equa)
{
    return fabsf(a - b) < equa;
}

// 合并同方向连续的点
PointArray PointArray::Combine(cxFloat equa) const
{
    cxInt siz = Size();
    PointArray ret;
    if(siz < 3){
        return *this;
    }
    //第一个点必须加入
    Point p1 = At(0);
    ret.Append(p1);
    Point p2 = At(1);
    for(cxInt i=2; i<siz; i++){
        Point p3 = At(i);
        //方向改变或者必须保留
        cxFloat a1 = p1.Angle(p2);
        cxFloat a2 = p2.Angle(p3);
        if(p2.IsKeep() || !cxRadiansEqu(a1, a2)){
            ret.Append(p2);
        }
        //最后一个点或者必须保留
        if(p3.IsKeep() ||  i == (siz - 1)){
            ret.Append(p3);
        }
        p1 = p2;
        p2 = p3;
    }
    return ret;
}

void PointArray::Append(const PointArray &v)
{
    for(cxInt i=0; i<v.Size(); i++){
        Append(v.At(i));
    }
}

const Point& PointArray::Last() const
{
    CX_ASSERT(Size() > 0, "array empty");
    return At(Size() - 1);
}

const Point& PointArray::First() const
{
    CX_ASSERT(Size() > 0, "array empty");
    return At(0);
}

cxBool PointArray::HasPoint(const Point &v)
{
    for(cxInt i=0; i<Size(); i++){
        Point tmp = At(i);
        if(tmp.x == v.x && tmp.y == v.y){
            return true;
        }
    }
    return false;
}

cxBool PointArray::HasPoint(const cxPoint2I &v)
{
    for(cxInt i=0; i<Size(); i++){
        Point tmp = At(i);
        if(tmp.x == v.x && tmp.y == v.y){
            return true;
        }
    }
    return false;
}

void PointArray::Append(const cxPoint2I &v,cxUInt attr)
{
    Point p = Point(v, attr);
    if(HasPoint(p) && !p.IsKeep()){
        return;
    }
    push_back(Point(v,attr));
}

void PointArray::Append(const Point &v)
{
    if(HasPoint(v) && !v.IsKeep()){
        return;
    }
    push_back(v);
}

void PointArray::Remove(cxInt idx)
{
    erase(begin() + idx);
}

void PointArray::Remove(cxInt idx,cxInt n)
{
    erase(begin() + idx, begin() + idx + n);
}

void PointArray::Clear()
{
    clear();
}

const Point& PointArray::At(cxInt idx) const
{
    return at(idx);
}

Point& PointArray::At(cxInt idx)
{
    return at(idx);
}

const Point *PointArray::Buffer() const
{
    return data();
}

Point& PointArray::operator [](cxInt idx)
{
    return at(idx);
}

const Point& PointArray::operator [](cxInt idx) const
{
    return at(idx);
}

const cxInt PointArray::Size() const
{
    return (cxInt)size();
}

cxBool PointArray::IsEmpty() const
{
    return empty();
}

CX_CPP_END

