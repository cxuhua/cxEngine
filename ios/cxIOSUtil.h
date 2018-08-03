//
//  cxIOSUtil.h
//  cxEngineCore
//
//  Created by xuhua on 5/30/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#ifndef cxEngineCore_cxIOSUtil_h
#define cxEngineCore_cxIOSUtil_h

#include <core/cxUtil.h>

CX_CPP_BEGIN

class cxIOSUtil : public cxUtil
{
public:
    CX_DECLARE(cxIOSUtil);
public:
    cxFloat ScaleFactor();
    cxStr *UUID();
    
    cxStr *GetLang();
    cxStr *GetCountry();
    
    cxStr *DocumentPath(cchars file);
    cxStr *AssetsPath(cchars file);
    
    cxInt GetVersionCode();
    cxStr *GetVersionName();
    
    void Logger(const char* type,const char*file,int line,const char* format,va_list ap);
};

CX_CPP_END

#endif


