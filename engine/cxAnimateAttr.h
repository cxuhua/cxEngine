//
//  AnimaAttr.h
//  godcity
//
//  Created by xuhua on 7/7/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#ifndef godcity_AnimaAttr_h
#define godcity_AnimaAttr_h

#include <map>
#include <math/cxPoint2I.h>
#include <core/cxHash.h>
#include <engine/cxAnimate.h>

CX_CPP_BEGIN

/* CVS field
ID      名字ID	动作         播放次数     播放速度	组编码	开始帧	结束帧	关键帧
Name	TID     Action      Repeat      Speed	Group	From	To      Key
String	String	String      Int         Float	Int     Int     Int     Int
*/
class cxAnimateAttr : public cxObject
{
public:
    CX_DECLARE(cxAnimateAttr);
protected:
    explicit cxAnimateAttr();
    virtual ~cxAnimateAttr();
private:
    std::map<std::string,cxActionAttr> actions;
public:
    static void Load(cxHash *values,cchars file);
    const cxActionAttr *Action(cchars key) const;
};

CX_CPP_END

#endif


