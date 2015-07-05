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
    label = nullptr;
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

void cxWindow::InitDebugLabel()
{
    label = cxLabel::CreateUTF8("0000000");
    label->SetZ(INT_MAX);
    label->SetFontSize(40);
    label->SetResizeBox(10.0f);
    label->SetColor(cxColor4F::RED);
    label->SetResizeFlags(cxView::ResizeRightBottom);
    label->SetStroke(1, cxColor4F::BLACK, 0.5f);
    Append(label);
}

cxLabel *cxWindow::DebugLabel()
{
    CX_ASSERT(label != nullptr, "debug label not init");
    return label;
}

void cxWindow::Push(cxView *view)
{
    cxView *top = Top();
    if(top != nullptr && view->EnableHideTop()){
        top->EnableSleep(true);
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
        top->EnableSleep(false);
    }
}

CX_CPP_END

