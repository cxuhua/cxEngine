//
// cxViewExt.h
// cxEngineCore
//
// Created by 徐华 on 2019/3/8.
//Copyright © 2019 xuhua. All rights reserved.
//

#ifndef cxViewExt_h
#define cxViewExt_h

#include "cxView.h"

CX_CPP_BEGIN

class cxViewExt : public cxObject
{
public:
    friend cxView;
    CX_DECLARE(cxViewExt);
protected:
    explicit cxViewExt();
    virtual ~cxViewExt();
protected:
    virtual void OnLeave();
    virtual void OnEnter();
    virtual void OnAttchView(cxView *pv);
    virtual void OnUpdate(const cxFloat &dt);
    //return true will skip position set
    virtual bool SetPosition(const cxPoint2F &v);
    virtual bool SetAngle(const cxFloat &v);
protected:
    cxView *pview;
public:
    void Attach(cxView *pv);
};

CX_CPP_END

#endif
