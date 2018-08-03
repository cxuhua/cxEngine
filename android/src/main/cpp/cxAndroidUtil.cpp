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


cxAndroidUtil::cxAndroidUtil()
{
    
}

cxAndroidUtil::~cxAndroidUtil()
{
    
}

cxInt cxAndroidUtil::GetVersionCode()
{
    return cxAndroid::Instance()->GetVersionCode();
}

cxStr *cxAndroidUtil::GetVersionName()
{
    return cxAndroid::Instance()->GetVersionName();
}


cxStr *cxAndroidUtil::UUID()
{
    return cxAndroid::Instance()->UUID();
}

cxStr *cxAndroidUtil::AssetsData(cchars file)
{
    return cxAndroid::Instance()->AssetsData(file);
}

void cxAndroidUtil::Logger(const char* type,const char*file,int line,const char* format,va_list ap)
{
    cxAndroid::Instance()->Logger(type, file, line, format, ap);
}

cxStr *cxAndroidUtil::GetLang()
{
    return cxAndroid::Instance()->GetLang();
}

cxStr *cxAndroidUtil::GetCountry()
{
    return cxAndroid::Instance()->GetCountry();
}

cxStr *cxAndroidUtil::DocumentPath(cchars file)
{
    return cxAndroid::Instance()->DocumentPath(file);
}

CX_CPP_END

