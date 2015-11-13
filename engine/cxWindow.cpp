//
//  cxWindow.cpp
//  cxEngineCore
//
//  Created by xuhua on 6/16/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#include "cxEngine.h"
#include "cxWindow.h"

CX_CPP_BEGIN

CX_IMPLEMENT(cxWindow);

cxWindow::cxWindow()
{
    views = cxStack::Alloc();
}

cxWindow::~cxWindow()
{
    views->Release();
}

cxView *cxWindow::Top() const
{
    return static_cast<cxView *>(views->Top());
}

cxBool cxWindow::OnDispatch(const cxTouchable *e)
{
    return false;
}

void cxWindow::Push(cxView *view)
{
    cxView *top = Top();
    if(top != nullptr && view->EnableHideTop()){
        top->SetEnableSleep(true);
    }
    Append(view);
    views->Push(view);
}

void cxWindow::Replace(cxView *view)
{
    cxView *top = Top();
    if(top != nullptr){
        top->Remove();
        views->Pop();
    }
    Push(view);
}

void cxWindow::Pop()
{
    cxView *top = Top();
    if(top != nullptr){
        top->Remove();
        views->Pop();
    }
    top = Top();
    if(top != nullptr){
        top->SetEnableSleep(false);
    }
}

CX_CPP_END

