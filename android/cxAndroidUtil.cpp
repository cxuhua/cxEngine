//
//  cxAndroidUtil.cpp
//  cxEngineCore
//
//  Created by xuhua on 5/31/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#include "cxAndroid.h"
#include "cxAndroidUtil.h"

CX_CPP_BEGIN

CX_IMPLEMENT(cxAndroidUtil);

void cxAndroidUtil::Logger(const char* type,const char*file,int line,const char* format,va_list ap)
{
    char *buffer = nullptr;
    vasprintf(&buffer, format, ap);
    if(strcmp(type, "ERROR") == 0 || strcmp(type, "ASSERT") == 0){
        __android_log_print(ANDROID_LOG_ERROR, "cxEngine", "[%s:%d] %s:%s",file,line,type,buffer);
    }else if(strcmp(type, "WARN") == 0){
        __android_log_print(ANDROID_LOG_WARN, "cxEngine", "[%s:%d] %s:%s",file,line,type,buffer);
    }else{
        __android_log_print(ANDROID_LOG_INFO, "cxEngine", "[%s:%d] %s:%s",file,line,type,buffer);
    }
    free(buffer);
}

cxAndroidUtil::cxAndroidUtil()
{
    
}

cxAndroidUtil::~cxAndroidUtil()
{
    
}

CX_CPP_END

