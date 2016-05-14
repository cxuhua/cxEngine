//
//  AnimaAttr.h
//  godcity
//  帧动画组
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

class cxActionGroup : public cxObject
{
public:
    CX_DECLARE(cxActionGroup);
protected:
    explicit cxActionGroup();
    virtual ~cxActionGroup();
private:
    std::map<std::string,cxActionAttr> actions;
public:
    static void Load(cxHash *values,cchars file);
    const cxActionAttr *Action(cchars key) const;
};

CX_CPP_END

#endif


