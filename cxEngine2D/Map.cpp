//
//  Map.cpp
//  cxEngineTest
//
//  Created by xuhua on 5/18/16.
//  Copyright © 2016 xuhua. All rights reserved.
//

#include <engine/cxEngine.h>
#include "Map.h"
#include "Shader.h"

CX_CPP_BEGIN

CX_IMPLEMENT(Map);

cxSprite *Map::CreateRangeView(cxFloat width)
{
    cxSize2F size = itemSize * width * 2.0f;
    cxSprite *rv = cxSprite::Create();
    rv->SetSize(size);
    rv->SetTexture("t.png");
    cxShader *shader = RangeShader::Create(1.f/size.w, cxPoint2F(1.0f, 1.0));
    rv->SetShader(shader);
    return rv;
}

Map::Map()
{
    //11
    cxFloat angel = sinf(cxDegreesToRadians(60));
    mapGrid = cxPoint2I(30, 50);
    cxSize2F wsiz = cxEngine::Instance()->WinSize();
    itemSize.w = wsiz.w/mapGrid.x;
    itemSize.h = itemSize.w * angel;
    mapSize.w = itemSize.w * mapGrid.x;
    mapSize.h = itemSize.h * mapGrid.y;
    SetSize(mapSize);
    
    
//    SetResizeFlags(ResizeTop);r
    
    cxFloat ar = 9.0f;
    
//    {
//        cxFloat w = 17.0f;
//        cxSprite *sp = cxSprite::Create();
//        sp->SetTexture("grid.png");
//        sp->SetSize(itemSize * w);
//        sp->SetPosition(ToPos(cxPoint2I(2, 6), w));
//        sp->SetColor(cxColor4F::YELLOW);
//        Append(sp);
//    }
    
    
    for(cxInt x=0;x<mapGrid.x;x++)
    for(cxInt y=0;y<mapGrid.y;y++){
        cxSprite *sp = cxSprite::Create();
        sp->SetTexture("grid.png");
        sp->SetSize(itemSize);
        sp->SetPosition(ToPos(cxPoint2I(x, y)));
        Append(sp);
    }
    //
//    for(cxInt x=0;x<mapGrid.x;x++)
//    for(cxInt y=0;y<mapGrid.y;y++){
//        if((x == 6||x == 14) || (y == 10 || y == 18)){
//            if(y >= 10 && y <= 18 && x >= 6 && x <= 14){
//                cxSprite *sp = cxSprite::Create();
//                sp->SetTexture("grid.png");
//                sp->SetSize(itemSize);
//                sp->SetPosition(ToPos(cxPoint2I(x, y)));
//                sp->SetColor(cxColor4F::BLUE);
//                Append(sp);
//            }
//        }
//    }
    //center
//    {
//        cxFloat w = 3.0f;
//        cxSprite *sp = cxSprite::Create();
//        sp->SetTexture("grid.png");
//        sp->SetSize(itemSize * w);
//        sp->SetPosition(ToPos(cxPoint2I(9, 13), w));
//        sp->SetColor(cxColor4F::RED);
//        Append(sp);
//        cxSprite *r = CreateRangeView(6.5f);
//        sp->Append(r);
//    }
    //left
    {
        cxFloat w = 4.0f;
        cxSprite *sp = cxSprite::Create();
        sp->SetTexture("grid.png");
        sp->SetSize(itemSize * w);
        sp->SetPosition(ToPos(cxPoint2I(2, 23), w));
        sp->SetColor(cxColor4F::RED);
        Append(sp);
        
        cxSprite *r = CreateRangeView(ar);
        sp->Append(r);
    }
    //right
    {
        cxFloat w = 4.0f;
        cxSprite *sp = cxSprite::Create();
        sp->SetTexture("grid.png");
        sp->SetSize(itemSize * w);
        sp->SetPosition(ToPos(cxPoint2I(24, 23), w));
        sp->SetColor(cxColor4F::RED);
        Append(sp);
        
        cxSprite *r = CreateRangeView(ar);
        sp->Append(r);
    }
    //top
    {
        cxFloat w = 4.0f;
        cxSprite *sp = cxSprite::Create();
        sp->SetTexture("grid.png");
        sp->SetSize(itemSize * w);
        sp->SetPosition(ToPos(cxPoint2I(13, 42), w));
        sp->SetColor(cxColor4F::RED);
        Append(sp);
        
        cxSprite *r = CreateRangeView(ar);
        sp->Append(r);
    }
    //bottom
    {
        cxFloat w = 4.0f;
        cxSprite *sp = cxSprite::Create();
        sp->SetTexture("grid.png");
        sp->SetSize(itemSize * w);
        sp->SetPosition(ToPos(cxPoint2I(13, 4), w));
        sp->SetColor(cxColor4F::RED);
        Append(sp);
        
        cxSprite *r = CreateRangeView(ar);
        sp->Append(r);
    }
    
//    {
//        cxFloat w = 6.0f;
//        cxSprite *sp = cxSprite::Create();
//        sp->SetTexture("grid.png");
//        sp->SetSize(itemSize * cxSize2F(5.0f, 6.0f));
//        cxPoint2F pos = ToPos(cxPoint2I(0, 0), w);
//        pos.x -= itemSize.w/2.0f;
//        sp->SetPosition(pos);
//        sp->SetColor(cxColor4F::GREEN);
//        sp->SetPixel(4.0f);
//        Append(sp);
//    }
//    {
//        cxFloat w = 6.0f;
//        cxSprite *sp = cxSprite::Create();
//        sp->SetTexture("grid.png");
//        sp->SetSize(itemSize * cxSize2F(5.0f, 6.0f));
//        cxPoint2F pos = ToPos(cxPoint2I(0, 0), w);
//        pos.x -= itemSize.w/2.0f;
//        pos.x += itemSize.w * 16;
//        sp->SetPosition(pos);
//        sp->SetColor(cxColor4F::GREEN);
//        Append(sp);
//    }
//    {
//        cxFloat w = 6.0f;
//        cxSprite *sp = cxSprite::Create();
//        sp->SetTexture("grid.png");
//        sp->SetSize(itemSize * cxSize2F(11.0f, 2.0f));
//        cxPoint2F pos = ToPos(cxPoint2I(0, 0), w);
//        pos.x -= itemSize.w/2.0f;
//        pos.x += itemSize.w * 8;
//        pos.y -= itemSize.h * 2.0f;
//        sp->SetPosition(pos);
//        sp->SetColor(cxColor4F::GREEN);
//        sp->SetPixel(4.0f);
//        Append(sp);
//    }
//    
//    {
//        cxFloat w = 6.0f;
//        cxSprite *sp = cxSprite::Create();
//        sp->SetTexture("grid.png");
//        sp->SetSize(itemSize * cxSize2F(5.0f, 6.0f));
//        cxPoint2F pos = ToPos(cxPoint2I(0, 0), w);
//        pos.x -= itemSize.w/2.0f;
//        pos.y += itemSize.h * 23;
//        sp->SetPosition(pos);
//        sp->SetColor(cxColor4F::GREEN);
//        sp->SetPixel(4.0f);
//        Append(sp);
//    }
//    {
//        cxFloat w = 6.0f;
//        cxSprite *sp = cxSprite::Create();
//        sp->SetTexture("grid.png");
//        sp->SetSize(itemSize * cxSize2F(5.0f, 6.0f));
//        cxPoint2F pos = ToPos(cxPoint2I(0, 0), w);
//        pos.x -= itemSize.w/2.0f;
//        pos.x += itemSize.w * 16;
//        pos.y += itemSize.h * 23;;
//        sp->SetPosition(pos);
//        sp->SetColor(cxColor4F::GREEN);
//        Append(sp);
//    }
//    {
//        cxFloat w = 6.0f;
//        cxSprite *sp = cxSprite::Create();
//        sp->SetTexture("grid.png");
//        sp->SetSize(itemSize * cxSize2F(11.0f, 2.0f));
//        cxPoint2F pos = ToPos(cxPoint2I(0, 0), w);
//        pos.x -= itemSize.w/2.0f;
//        pos.x += itemSize.w * 8;
//        pos.y += itemSize.h * 25;
//        sp->SetPosition(pos);
//        sp->SetColor(cxColor4F::GREEN);
//        sp->SetPixel(4.0f);
//        Append(sp);
//    }
}

Map::~Map()
{
    
}

cxPoint2F Map::ToPos(const cxPoint2I &idx,cxFloat width)
{
    cxPoint2F pos = ToPos(idx);
    pos.x += (width - 1.0f)*itemSize.w/2.0f;
    pos.y += (width - 1.0f)*itemSize.h/2.0f;
    return pos;
}

cxPoint2F Map::ToPos(const cxPoint2I &idx)
{
    cxSize2F siz = Size();
    cxFloat x = (itemSize.w-siz.w)/2.0f + idx.x * itemSize.w;
    cxFloat y = (itemSize.h-siz.h)/2.0f + idx.y * itemSize.h;
    return cxPoint2F(x, y);
}

CX_CPP_END

