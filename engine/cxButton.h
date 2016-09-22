//
//  cxButton.h
//  cxEngineCore
//
//  Created by xuhua on 6/28/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#ifndef cxEngineCore_cxButton_h
#define cxEngineCore_cxButton_h

#include "cxTimer.h"
#include "cxView.h"

CX_CPP_BEGIN

class cxButton : public cxView
{
public:
    CX_DECLARE(cxButton);
protected:
    explicit cxButton();
    virtual ~cxButton();
private:
    cxBool ispress;
    cxBool ispass;
    cxBool isenable;
    cxBool islong;
    cxTimer *timer;
    cxFloat longerTime;
    void timerArrive(cxAction *pav);
    void timerStart();
    void timerExit();
protected:
    cxBool OnDispatch(const cxTouchable *e);
public:
    cxEvent<cxButton> onTap;
    cxEvent<cxButton> onPress;
    cxEvent<cxButton> onLonger;
    cxEvent<cxButton> onRelease;
    cxEvent<cxButton> onMove;
public:
    void SetLongerTime(cxFloat v);
    void SetIsPass(cxBool v);
    void SetIsLong(cxBool v);
    void SetIsEnable(cxBool v);
};

CX_CPP_END

#endif


