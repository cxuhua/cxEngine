//
//  cxAndroidUtil.h
//  cxEngineCore
//
//  Created by xuhua on 5/31/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#ifndef cxEngineCore_cxAndroidUtil_h
#define cxEngineCore_cxAndroidUtil_h

#include <core/cxUtil.h>
#include <math/cxMath.h>
#include "cxAndroid.h"

CX_CPP_BEGIN

class cxAndroidUtil : public cxUtil
{
public:
    CX_DECLARE(cxAndroidUtil);
protected:
    explicit cxAndroidUtil();
    virtual ~cxAndroidUtil();
private:
public:
    cxStr *GetLang();
    cxStr *GetCountry();
    cxStr *DocumentPath(cchars file);
    void Logger(const char* type,const char*file,int line,const char* format,va_list ap);
    cxStr *AssetsData(cchars file);
    cxStr *UUID();
};

CX_CPP_END

#endif


