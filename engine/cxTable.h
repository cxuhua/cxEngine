//
//  cxTable.h
//  cxEngineCore
//
//  Created by xuhua on 7/15/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#ifndef cxEngineCore_cxTable_h
#define cxEngineCore_cxTable_h

#include <math/cxPoint2I.h>
#include <math/cxBox4F.h>
#include "cxSprite.h"

CX_CPP_BEGIN

class cxTable : public cxSprite
{
public:
    CX_DECLARE(cxTable);
protected:
    explicit cxTable();
    virtual ~cxTable();
protected:
    void OnDirty();
private:
    cxLong ignore;
    cxInt rownum;
    cxBox4F padding;
    cxBox4F margin;
public:
    cxTable *SetIgnore(cxLong tag);
    cxTable *UpdateViews();
    //内边距
    cxTable *SetPadding(const cxBox4F &v);
    //外边距
    cxTable *SetMargin(const cxBox4F &v);
    cxTable *SetRowNum(cxInt v);
};

CX_CPP_END

#endif


