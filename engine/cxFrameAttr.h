//
//  FrameAttr.h
//  godcity
//
//  Created by xuhua on 7/8/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#ifndef godcity_FrameAttr_h
#define godcity_FrameAttr_h

#include <engine/cxFrames.h>

CX_CPP_BEGIN

/* CVS field
 ID,    名字,     纹理,      帧偏移,   组数,     每组帧数,   层数,     层映射,    帧重复
 Name,  TID,    Texture,    Offset,  Group,   Count,     Layer,   Map,      Repeats
*/
class cxFrameAttr : public cxFrames
{
public:
    CX_DECLARE(cxFrameAttr);
protected:
    explicit cxFrameAttr();
    virtual ~cxFrameAttr();
public:
    static void Load(cxHash *values, cchars file);
};

CX_CPP_END

#endif


