//
//  Point.h
//  cxEngineCore
//  移动路径点
//  Created by xuhua on 6/1/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#ifndef cxEngineCore_Point_h
#define cxEngineCore_Point_h

#include <vector>
#include <math/cxPoint2I.h>

CX_CPP_BEGIN

//如果要保留点(合并路径时忽略)
#define ATTR_IS_KEEP (1 << 0)
//跳跃到下点
#define ATTR_IS_JUMP (1 << 1)
//上个跳跃的目标点
#define ATTR_IS_FALL (1 << 2)

struct Point
{
    cxInt x;
    cxInt y;
    cxUInt a;
    Point();
    Point(cxInt ax,cxInt ay,cxInt aa);
    Point(cxPoint2I ap,cxInt aa);
    const cxBool IsFall() const;
    const cxBool IsJump() const;
    const cxBool IsKeep() const;
    const cxPoint2F ToFloat() const;
    const cxPoint2F ToFloat(const cxFloat &add) const;
    const cxFloat Angle() const;
    const cxFloat Angle(const Point &p) const;
    cxPoint2I ToPoint2I();
    const cxPoint2I ToPoint2I() const;
    cxBool operator==(const Point &v) const;
    cxBool operator!=(const Point &v) const;
};

class PointArray : private std::vector<Point>
{
public:
    explicit PointArray();
    virtual ~PointArray();
public:
    const Point& Last() const;
    const Point& First() const;
    void Clear();
    cxBool HasPoint(const Point &v);
    cxBool HasPoint(const cxPoint2I &v);
    void Append(const Point &v);
    void Append(const cxPoint2I &v,cxUInt attr);
    void Append(const PointArray &v);
    void Remove(cxInt idx);
    void Remove(cxInt idx,cxInt n);
    const Point *Buffer()const ;
    const cxInt Size() const;
    const Point& At(cxInt idx) const;
    Point& At(cxInt idx);
    cxBool IsEmpty() const;
    //combine angle equ point
    PointArray Combine(cxFloat equa = cxEqualFloat) const;
    PointArray Reverse() const;
public:
    const Point& operator [](cxInt idx) const;
    Point& operator [](cxInt idx);
};

CX_CPP_END

#endif


