//
//  cxWindow.h
//  cxEngineCore
//
//  Created by xuhua on 6/16/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#ifndef cxEngineCore_cxWindow_h
#define cxEngineCore_cxWindow_h

#include <core/cxStack.h>
#include "cxView.h"
#include "cxLabel.h"

CX_CPP_BEGIN

class cxWindow : public cxView
{
public:
    CX_DECLARE(cxWindow);
protected:
    explicit cxWindow();
    virtual ~cxWindow();
protected:
    cxBool OnDispatch(const cxTouchable *e);
private:
    cxStack *views;
public:
    void Push(cxView *view);
    void Replace(cxView *view);
    void Pop();
    cxView *Top() const;
};

CX_CPP_END

#endif


