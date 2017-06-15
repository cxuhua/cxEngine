//
//  Map.h
//  cxEngineTest
//
//  Created by xuhua on 5/18/16.
//  Copyright © 2016 xuhua. All rights reserved.
//

#ifndef cxEngineTest_Map_h
#define cxEngineTest_Map_h

#include <engine/cxView.h>

CX_CPP_BEGIN

class Map : public cxView
{
public:
    CX_DECLARE(Map);
protected:
    explicit Map();
    virtual ~Map();
protected:
    cxPoint2I mapGrid;
    cxSize2F mapSize;
    cxSize2F itemSize;
private:
public:
    virtual void Read(){};
    cxPoint2F ToPos(const cxPoint2I &idx);
    //radians为物体半径 width,3=3x3 4=4x4
    cxPoint2F ToPos(const cxPoint2I &idx,cxFloat width);
    //创建范围提示
    cxSprite *CreateRangeView(cxFloat width);
};

CX_CPP_END

#endif


