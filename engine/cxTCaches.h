//
//  cxTCaches.h
//  cxEngineCore
//  text texture cache
//  适合大量的小文字批显示，宽度不超过maxx的静态文字
//  Created by xuhua on 1/13/16.
//  Copyright © 2016 xuhua. All rights reserved.
//

#ifndef cxEngineCore_cxTCaches_h
#define cxEngineCore_cxTCaches_h

#include <core/cxStr.h>
#include <math/cxBox4F.h>
#include <core/cxHash.h>
#include "cxTexture.h"

CX_CPP_BEGIN

class cxTCaches : public cxObject
{
    friend cxCore;
private:
    CX_DECLARE(cxTCaches);
protected:
    explicit cxTCaches();
    virtual ~cxTCaches();
private:
    cxInt maxx;
    cxInt maxy;
    cxInt yv;
    struct Block
    {
        cxBool use;
        cxUInt16 x;
        cxUInt16 y;
        cxUInt16 w;
        cxUInt16 h;
        Block();
        Block(cxUInt16 xx,cxUInt16 yy,cxUInt16 ww,cxUInt16 hh);
        Block(const cxStr *data);
        cxStr *Create();
        cxUInt64 Key();
    };
    cxTexture *ptex;
    cxHash *blocks;
    cxByte *buffer;
    static cxTCaches *instance;
    static cxUInt64 getKey(cxUInt16 x,cxUInt16 y,cxUInt16 w,cxUInt16 h);
    void update();
    Block findBlock(cxUInt16 w,cxUInt16 h);
public:
    void Clear();
    cxTexCoord *Coord(cxUInt64 key);
    void Remove(cxUInt64 key);
    cxUInt64 Append(cxUInt16 w,cxUInt16 h,const cxStr *data);
    static cxTCaches *Instance();
};

CX_CPP_END

#endif


