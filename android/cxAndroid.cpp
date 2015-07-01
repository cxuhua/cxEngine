//
//  cxMain.c
//  cxEngine2D
//
//  Created by xuhua on 9/25/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include "cxAndroid.h"

//android log print
void cxUtilPrint(const char* type,const char*file,int line,const char* format,va_list ap)
{
    char buffer[4096]={0};
    vsnprintf(buffer, 4096, format, ap);
    if(strcmp(type, "ERROR") == 0 || strcmp(type, "ASSERT") == 0){
        __android_log_print(ANDROID_LOG_ERROR, "cxEngine", "[%s:%d] %s:%s",file,line,type,buffer);
    }else if(strcmp(type, "WARN") == 0){
        __android_log_print(ANDROID_LOG_WARN, "cxEngine", "[%s:%d] %s:%s",file,line,type,buffer);
    }else{
        __android_log_print(ANDROID_LOG_INFO, "cxEngine", "[%s:%d] %s:%s",file,line,type,buffer);
    }
}