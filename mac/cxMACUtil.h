//
//  cxIOSUtil.h
//  cxEngineCore
//
//  Created by xuhua on 5/30/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#ifndef cxEngineCore_cxMACUtil_h
#define cxEngineCore_cxMACUtil_h

#include <core/cxUtil.h>

CX_CPP_BEGIN

class cxMACUtil : public cxUtil
{
public:
    CX_DECLARE(cxMACUtil);
public:
    cxStr *UUID() const;
    void Logger(const char* type,const char*file,int line,const char* format,va_list ap);
};

CX_CPP_END

#endif


