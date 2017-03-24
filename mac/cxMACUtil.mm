//
//  cxIOSUtil.cpp
//  cxEngineCore
//
//  Created by xuhua on 5/30/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#include <uuid/uuid.h>
#import <CoreText/CoreText.h>
#import <Foundation/Foundation.h>
#include <engine/cxEngine.h>
#include <engine/cxLabel.h>
#include "cxMACUtil.h"

CX_CPP_BEGIN

CX_IMPLEMENT(cxMACUtil);


void cxMACUtil::Logger(const char* type,const char*file,int line,const char* format,va_list ap)
{
    char *buffer = nullptr;
    int bytes = vasprintf(&buffer, format, ap);
    NSString *txt = [NSString stringWithUTF8String:buffer];
    NSLog(@"[%s:%d] %s:%s[%d]\n",file,line,type,[txt UTF8String],bytes);
    free(buffer);
}

cxStr *cxMACUtil::UUID() const
{
    uuid_t uuid;
    uuid_generate(uuid);
    return cxStr::Create()->Init(uuid, sizeof(uuid_t));
}

CX_CPP_END

