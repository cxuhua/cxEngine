//
//  cxIOSUtil.cpp
//  cxEngineCore
//
//  Created by xuhua on 5/30/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#include <uuid/uuid.h>
#import <UIKit/UIKit.h>
#import <CoreText/CoreText.h>
#include <engine/cxEngine.h>
#include <engine/cxLabel.h>
#include "cxIOSUtil.h"

CX_CPP_BEGIN

CX_IMPLEMENT(cxIOSUtil);

cxStr *cxIOSUtil::GetLang() const
{
    NSLocale *locale = [NSLocale currentLocale];
    return cxStr::UTF8([[locale objectForKey:NSLocaleLanguageCode] UTF8String]);
}

cxStr *cxIOSUtil::GetCountry() const
{
    NSLocale *locale = [NSLocale currentLocale];
    return cxStr::UTF8([[locale objectForKey:NSLocaleCountryCode] UTF8String]);
}

void cxIOSUtil::Logger(const char* type,const char*file,int line,const char* format,va_list ap)
{
    char *buffer = nullptr;
    int bytes = vasprintf(&buffer, format, ap);
    NSString *txt = [NSString stringWithUTF8String:buffer];
    NSLog(@"[%s:%d] %s:%s[%d]\n",file,line,type,[txt UTF8String],bytes);
    free(buffer);
}

cxStr *cxIOSUtil::DocumentPath(cchars file)
{
    cxStr *rv = cxStr::Create();
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *path = [paths objectAtIndex:0];
    if(file != NULL){
        return rv->AppFmt("%s/%s", [path UTF8String],file);
    }else{
        return rv->Init([path UTF8String]);
    }
    return rv;
}

cxStr *cxIOSUtil::UUID() const
{
    uuid_t uuid;
    uuid_generate(uuid);
    return cxStr::Create()->Init(uuid, sizeof(uuid_t));
}

cxFloat cxIOSUtil::ScaleFactor() const
{
    return (cxFloat)[UIScreen mainScreen].scale;
}

cxStr *cxIOSUtil::AssetsPath(cchars file)
{
    cxStr *rv = cxStr::Create();
    NSString *path = [[NSBundle mainBundle] resourcePath];
    if(file != NULL){
        return rv->AppFmt("%s/%s", [path UTF8String],file);
    }else{
        return rv->Init([path UTF8String]);
    }
    return rv;
}

CX_CPP_END

