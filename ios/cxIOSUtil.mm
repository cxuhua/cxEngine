//
//  cxIOSUtil.cpp
//  cxEngineCore
//
//  Created by xuhua on 5/30/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <CoreText/CoreText.h>
#include <engine/cxEngine.h>
#include <engine/cxLabel.h>
#include "cxIOSUtil.h"

CX_CPP_BEGIN

CX_IMPLEMENT(cxIOSUtil);

void cxIOSUtil::Logger(const char* type,const char*file,int line,const char* format,va_list ap)
{
    char *buffer = nullptr;
    vasprintf(&buffer, format, ap);
    NSString *txt = [NSString stringWithUTF8String:buffer];
    NSLog(@"[%s:%d] %s:%@\n",file,line,type,txt);
    free(buffer);
}

const cxStr *cxIOSUtil::DocumentPath(const cxStr *file)
{
    cxStr *rv = cxStr::Create();
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *path = [paths objectAtIndex:0];
    if(file != NULL){
        return rv->AppFmt("%s/%s", [path UTF8String],file->Data());
    }else{
        return rv->Init([path UTF8String]);
    }
    return rv;
}

const cxFloat cxIOSUtil::ScaleFactor() const
{
    return (cxFloat)[UIScreen mainScreen].scale;
}

const cxStr *cxIOSUtil::AssetsPath(const cxStr *file)
{
    cxStr *rv = cxStr::Create();
    NSString *path = [[NSBundle mainBundle] resourcePath];
    if(file != NULL){
        return rv->AppFmt("%s/%s", [path UTF8String],file->Data());
    }else{
        return rv->Init([path UTF8String]);
    }
    return rv;
}

CX_CPP_END

