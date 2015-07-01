//
//  cxButton.h
//  cxEngineCore
//
//  Created by xuhua on 6/28/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#ifndef cxEngineCore_cxButton_h
#define cxEngineCore_cxButton_h

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
protected:
    cxBool OnDispatch(const cxTouchable *e);
public:
    
    cxEvent<cxButton> onTap;
    cxEvent<cxButton> onPress;
    cxEvent<cxButton> onRelease;
    cxEvent<cxButton> onMove;
    
    void SetIsPass(cxBool v);
    void SetIsEnable(cxBool v);
};

CX_CPP_END

#endif


