//
//  cxTCaches.cpp
//  cxEngineCore
//
//  Created by xuhua on 1/13/16.
//  Copyright © 2016 xuhua. All rights reserved.
//


#include "cxOpenGL.h"
#include "cxTCaches.h"

CX_CPP_BEGIN

cxStr *cxTCaches::Block::Create()
{
    cxStr *ret = cxStr::Create()->Init(sizeof(Block));
    memcpy(ret->Buffer(), this, sizeof(Block));
    return ret;
}

cxTCaches::Block::Block(cxUInt16 xx,cxUInt16 yy,cxUInt16 ww,cxUInt16 hh)
{
    x = xx;
    y = yy;
    w = ww;
    h = hh;
}

cxTCaches::Block::Block()
{
    x = 0;
    y = 0;
    w = 0;
    h = 0;
}

cxTCaches::Block::Block(const cxStr *data)
{
    memcpy(this, data->Buffer(), sizeof(Block));
}

cxUInt64 cxTCaches::Block::Key()
{
    return cxTCaches::getKey(x, y, w, h);
}

CX_IMPLEMENT(cxTCaches);

cxTCaches *cxTCaches::instance = nullptr;

cxTCaches *cxTCaches::Instance()
{
    return cxCore::One<cxTCaches>(&instance);
}

cxTCaches::cxTCaches()
{
    maxx = 256;
    maxy = cxOpenGL::Instance()->maxTextureSize;
    yv = 0;
    blocks = cxHash::Alloc();
    buffer = (cxByte *)malloc(maxx*maxy*4);
    ptex = cxTexture::Alloc();
    ptex->FromRGBA((cchars)buffer, maxx, maxy);
}

cxTCaches::~cxTCaches()
{
    blocks->Release();
    free(buffer);
    ptex->Release();
}

cxTexCoord *cxTCaches::Coord(cxUInt64 key)
{
    Block b;
    const cxStr *data = blocks->Get(key)->To<cxStr>();
    if(data != nullptr){
        b = Block(data);
    }
    if(b.w == 0){
        b.w = maxx;
    }
    if(b.h == 0){
        b.h = maxy;
    }
    cxTexCoord *coord = cxTexCoord::Create();
    coord->SetTexture(ptex);
    coord->frame = cxRect4F(b.x, b.y, b.w, b.h);
    coord->spriteSourceSize = cxRect4F(0, 0, b.w, b.h);
    coord->sourceSize = cxSize2F(b.w, b.h);
    return coord;
}

void cxTCaches::update()
{
    ptex->UpdateRGBA((cchars)buffer);
}

cxUInt64 cxTCaches::getKey(cxUInt16 x,cxUInt16 y,cxUInt16 w,cxUInt16 h)
{
    cxUInt64 ret = 0;
    ret |= (((cxUInt64)x) << 48);
    ret |= (((cxUInt64)y) << 32);
    ret |= (((cxUInt64)w) << 16);
    ret |= ((cxUInt64)h);
    return ret;
}

void cxTCaches::Clear()
{
    yv = 0;
    blocks->Clear();
}

void cxTCaches::Remove(cxUInt64 key)
{
    cxStr *data = blocks->Get(key)->To<cxStr>();
    if(data == nullptr){
        return;
    }
    //设置为不可用
    Block *b = (Block *)data->Buffer();
    b->use = false;
}

cxTCaches::Block cxTCaches::findBlock(cxUInt16 w,cxUInt16 h)
{
    //查找可用的block
    for(cxHash::Iter it=blocks->Begin();it!=blocks->End();it++){
        Block *b = (Block *)it->second->To<cxStr>()->Buffer();
        if(b->use){
            continue;
        }
        if(b->w <= w && b->h <= h){
            b->use = true;
            return *b;
        }
    }
    //新建block
    cxUInt64 key = getKey(0, yv, w, h);
    Block b(0,yv,w,h);
    b.use = true;
    blocks->Set(key, b.Create());
    yv += h;
    return b;
}

cxUInt64 cxTCaches::Append(cxUInt16 w,cxUInt16 h,const cxStr *data)
{
    CX_ASSERT(w <= maxx, "w too lager > %d",maxx);
    //find empty block
    Block b = findBlock(w, h);
    cxUInt16 x = b.x;
    cxUInt16 y = b.y;
    //copy block data
    cxInt sbytes = w * 4;
    cxInt dbytes = maxx * 4;
    for(cxInt row=0;row<h;row++){
        cxAny psrc = data->Buffer() + row * sbytes;
        cxAny pdst = buffer + x * 4 + (y + row) * dbytes;
        memcpy(pdst, psrc, sbytes);
    }
    //update texture
    update();
    return b.Key();
}

CX_CPP_END

