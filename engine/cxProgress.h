//
//  cxProgress.h
//  cxEngineCore
//
//  Created by xuhua on 7/13/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#ifndef cxEngineCore_cxProgress_h
#define cxEngineCore_cxProgress_h

#include "cxView.h"
#include "cxTimer.h"

CX_CPP_BEGIN

class cxProgress : public cxView
{
public:
    CX_DECLARE(cxProgress);
protected:
    explicit cxProgress();
    virtual ~cxProgress();
public:
    typedef cxUInt DirType;
    static const DirType None        = 0;
    static const DirType LeftToRight = 1 << 0;
    static const DirType RightToLeft = 1 << 1;
    static const DirType TopToBottom = 1 << 2;
    static const DirType BottomToTop = 1 << 3;
protected:
    void OnDirty();
    void UpdateValue(cxFloat v);
private:
    cxTimer *timer;
    cxBox4F inner;
    DirType dir;
    cxView *bg;
    cxView *vv;
    cxRange2F range;
    cxFloat value;
public:
    void SetInner(const cxBox4F &v);
    cxProgress *SetDir(DirType v);
    cxProgress *SetValue(cxFloat v);
    cxProgress *SetRange(const cxRange2F &v);
    cxProgress *Init(cxView *pbg,cxView *pvv,cxFloat v);
    static cxProgress *Create(cxView *pbg,cxView *pvv,cxFloat v);
};

CX_CPP_END

#endif


