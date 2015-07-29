//
//  cxLocalized.h
//  cxEngineCore
//
//  Created by xuhua on 7/11/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#ifndef cxEngineCore_cxLocalized_h
#define cxEngineCore_cxLocalized_h

#include <core/cxHash.h>

CX_CPP_BEGIN

class cxLocalized : public cxObject
{
    friend cxCore;
private:
    CX_DECLARE(cxLocalized);
protected:
    explicit cxLocalized();
    virtual ~cxLocalized();
private:
    static cxLocalized *instance;
    cxStr *lang;
    cxHash *texts;
public:
    static const cxStr *GetLang();
    static void SetLang(const cxStr *lng);
    
    static void Load(cchars file);
    
    static const cxStr *Text(cchars key);
    
    static cxLocalized *Instance();
};

CX_CPP_END

#endif


