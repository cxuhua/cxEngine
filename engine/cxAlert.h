//
//  cxAlert.h
//  cxEngineCore
//
//  Created by xuhua on 8/11/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#ifndef cxEngineCore_cxAlert_h
#define cxEngineCore_cxAlert_h

#include "cxSprite.h"

CX_CPP_BEGIN

class cxAlert : public cxSprite
{
public:
    CX_DECLARE(cxAlert);
protected:
    explicit cxAlert();
    virtual ~cxAlert();
protected:
    void OnEnter();
    cxBool OnDispatch(const cxTouchable *e);
private:

public:
    virtual void Show();
    virtual void Hide();
    cxView *Body();
public:
    cxEvent<cxAlert> onShow;
    cxEvent<cxAlert> onHide;
};

CX_CPP_END

#endif


