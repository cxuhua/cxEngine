//
//  cxLineF.cpp
//  cxEngineCore
//
//  Created by xuhua on 6/27/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#include "cxBox4F.h"
#include "cxLineF.h"

CX_CPP_BEGIN

cxLineF::cxLineF()
{
    
}

cxLineF::cxLineF(const cxPoint2F &aa,const cxPoint2F &ab)
{
    b = aa;
    b = ab;
}

cxInt cxLineF::AtLineLR(const cxPoint2F &v) const
{
    cxFloat ax = a.x;
    cxFloat ay = a.y;
    cxFloat bx = b.x;
    cxFloat by = b.y;
    ax -= v.x;
    ay -= v.y;
    bx -= v.x;
    by -= v.y;
    cxInt nRet = ax * by - ay * bx;
    if (nRet == 0)
        return 0;
    else if (nRet > 0)
        return 1;
    else if (nRet < 0)
        return -1;
    return 0;
}

cxBool cxLineF::Intersect(const cxLineF &l)
{
    cxInt n1 = l.AtLineLR(a);
    cxInt n2 = l.AtLineLR(b);
    if (n1 * n2 > 0){
        return false;
    }
    cxInt n3 = AtLineLR(l.a);
    cxInt n4 = AtLineLR(l.b);
    if (n3 * n4 > 0){
        return false;
    }
    return true;
}

void cxLineF::Slope(cxFloat &vk,cxFloat &vb) const
{
    vk = (b.y-a.y)/(b.x-a.x);
    vb = a.y - a.x * vk;
}

cxPoint2F cxLineF::IntersectPoint(const cxLineF &l)
{
    cxFloat k1,b1,k2,b2,x,y;
    Slope(k1, b1);
    l.Slope(k2, b2);
    if((isinf(k1) && isinf(k2)) || cxFloatIsEqual(k1, k2)){
        return cxPoint2F(INFINITY, INFINITY);
    }
    if(isinf(k1)){
        x = a.x;
        y = k2 * x + b2;
    }else if(isinf(k2)){
        x = l.a.x;
        y = k1 * x + b1;
    }else{
        x = (b2-b1)/(k1-k2);
        y = k1 * x + b1;
    }
    return cxPoint2F(x,y);
}

cxBool cxLineF::Intersect(const cxBox4F &v)
{
    cxLineF l = cxLineF(cxPoint2F(v.l, v.t), cxPoint2F(v.l, v.b));
    if(Intersect(l)){
        return true;
    }
    cxLineF b = cxLineF(cxPoint2F(v.l, v.b), cxPoint2F(v.r, v.b));
    if(Intersect(b)){
        return true;
    }
    cxLineF r = cxLineF(cxPoint2F(v.r, v.b), cxPoint2F(v.r, v.t));
    if(Intersect(r)){
        return true;
    }
    cxLineF t = cxLineF(cxPoint2F(v.r, v.t), cxPoint2F(v.l, v.t));
    if(Intersect(t)){
        return true;
    }
    return false;
}

CX_CPP_END

