//
//  cxIOSUtil.cpp
//  cxEngineCore
//
//  Created by xuhua on 5/30/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#include <sys/types.h>
#include <sys/sysctl.h>
#include <uuid/uuid.h>
#import <UIKit/UIKit.h>
#import <sys/utsname.h>
#import <CoreText/CoreText.h>
#include <engine/cxEngine.h>
#include <engine/cxLabel.h>
#include "cxIOSUtil.h"

typedef NS_ENUM(NSInteger, GBDeviceModel) {
    GBDeviceModelUnknown = 0,
    GBDeviceModelSimulatoriPhone,
    GBDeviceModelSimulatoriPad,
    GBDeviceModeliPhone1,
    GBDeviceModeliPhone3G,
    GBDeviceModeliPhone3GS,
    GBDeviceModeliPhone4,
    GBDeviceModeliPhone4S,
    GBDeviceModeliPhone5,
    GBDeviceModeliPhone5c,
    GBDeviceModeliPhone5s,
    GBDeviceModeliPhoneSE,
    GBDeviceModeliPhone6,
    GBDeviceModeliPhone6Plus,
    GBDeviceModeliPhone6s,
    GBDeviceModeliPhone6sPlus,
    GBDeviceModeliPhone7,
    GBDeviceModeliPhone7Plus,
    GBDeviceModeliPhone8,
    GBDeviceModeliPhone8Plus,
    GBDeviceModeliPhoneX,
    GBDeviceModeliPhoneXR,
    GBDeviceModeliPhoneXS,
    GBDeviceModeliPhoneXSMax,
    GBDeviceModeliPad1,
    GBDeviceModeliPad2,
    GBDeviceModeliPad3,
    GBDeviceModeliPad4,
    GBDeviceModeliPadMini1,
    GBDeviceModeliPadMini2,
    GBDeviceModeliPadMini3,
    GBDeviceModeliPadMini4,
    GBDeviceModeliPadAir1,
    GBDeviceModeliPadAir2,
    GBDeviceModeliPadPro9p7Inch,
    GBDeviceModeliPadPro10p5Inch,
    GBDeviceModeliPadPro12p9Inch,
    GBDeviceModeliPadPro10p5Inch2,
    GBDeviceModeliPadPro12p9Inch2,
    GBDeviceModeliPad5,
    GBDeviceModeliPad6,
    GBDeviceModeliPod1,
    GBDeviceModeliPod2,
    GBDeviceModeliPod3,
    GBDeviceModeliPod4,
    GBDeviceModeliPod5,
    GBDeviceModeliPod6,
};

typedef NS_ENUM(NSInteger, GBDeviceDisplay) {
    GBDeviceDisplayUnknown = 0,
    GBDeviceDisplay3p5Inch,
    GBDeviceDisplay4Inch,
    GBDeviceDisplay4p7Inch,
    GBDeviceDisplay5p5Inch,
    GBDeviceDisplay5p8Inch,
    GBDeviceDisplay6p1Inch,
    GBDeviceDisplay6p5Inch,
    GBDeviceDisplay7p9Inch,
    GBDeviceDisplay9p7Inch,
    GBDeviceDisplay10p5Inch,
    GBDeviceDisplay12p9Inch,
};

#if TARGET_OS_IPHONE
typedef NS_ENUM(NSInteger, GBDeviceFamily) {
    GBDeviceFamilyUnknown = 0,
    GBDeviceFamilyiPhone,
    GBDeviceFamilyiPad,
    GBDeviceFamilyiPod,
    GBDeviceFamilySimulator,
};
#else
typedef NS_ENUM(NSInteger, GBDeviceFamily) {
    GBDeviceFamilyUnknown = 0,
    GBDeviceFamilyiMac,
    GBDeviceFamilyMacMini,
    GBDeviceFamilyMacPro,
    GBDeviceFamilyMacBook,
    GBDeviceFamilyMacBookAir,
    GBDeviceFamilyMacBookPro,
    GBDeviceFamilyXserve,
};
#endif

CGFloat getDPI() {
    CGFloat pixelsPerInch = 0;
    if (TARGET_IPHONE_SIMULATOR) {
        return pixelsPerInch;
    }
    NSDictionary *familyManifest = @{@"iPhone": @(GBDeviceFamilyiPhone),@"iPad": @(GBDeviceFamilyiPad),@"iPod": @(GBDeviceFamilyiPod)};
    NSDictionary *modelManifest = @{
    @"iPhone": @{
        // 1st Gen
        @[@1, @1]: @[@(GBDeviceModeliPhone1), @"iPhone 1", @(GBDeviceDisplay3p5Inch), @163],
        // 3G
        @[@1, @2]: @[@(GBDeviceModeliPhone3G), @"iPhone 3G", @(GBDeviceDisplay3p5Inch), @163],
        // 3GS
        @[@2, @1]: @[@(GBDeviceModeliPhone3GS), @"iPhone 3GS", @(GBDeviceDisplay3p5Inch), @163],
        // 4
        @[@3, @1]: @[@(GBDeviceModeliPhone4), @"iPhone 4", @(GBDeviceDisplay3p5Inch), @326],
        @[@3, @2]: @[@(GBDeviceModeliPhone4), @"iPhone 4", @(GBDeviceDisplay3p5Inch), @326],
        @[@3, @3]: @[@(GBDeviceModeliPhone4), @"iPhone 4", @(GBDeviceDisplay3p5Inch), @326],
        // 4S
        @[@4, @1]: @[@(GBDeviceModeliPhone4S), @"iPhone 4S", @(GBDeviceDisplay3p5Inch), @326],
        // 5
        @[@5, @1]: @[@(GBDeviceModeliPhone5), @"iPhone 5", @(GBDeviceDisplay4Inch), @326],
        @[@5, @2]: @[@(GBDeviceModeliPhone5), @"iPhone 5", @(GBDeviceDisplay4Inch), @326],
        // 5c
        @[@5, @3]: @[@(GBDeviceModeliPhone5c), @"iPhone 5c", @(GBDeviceDisplay4Inch), @326],
        @[@5, @4]: @[@(GBDeviceModeliPhone5c), @"iPhone 5c", @(GBDeviceDisplay4Inch), @326],
        // 5s
        @[@6, @1]: @[@(GBDeviceModeliPhone5s), @"iPhone 5s", @(GBDeviceDisplay4Inch), @326],
        @[@6, @2]: @[@(GBDeviceModeliPhone5s), @"iPhone 5s", @(GBDeviceDisplay4Inch), @326],
        // 6 Plus
        @[@7, @1]: @[@(GBDeviceModeliPhone6Plus), @"iPhone 6 Plus", @(GBDeviceDisplay5p5Inch), @401],
        // 6
        @[@7, @2]: @[@(GBDeviceModeliPhone6), @"iPhone 6", @(GBDeviceDisplay4p7Inch), @326],
        // 6s
        @[@8, @1]: @[@(GBDeviceModeliPhone6s), @"iPhone 6s", @(GBDeviceDisplay4p7Inch), @326],
        // 6s Plus
        @[@8, @2]: @[@(GBDeviceModeliPhone6sPlus), @"iPhone 6s Plus", @(GBDeviceDisplay5p5Inch), @401],
        // SE
        @[@8, @4]: @[@(GBDeviceModeliPhoneSE), @"iPhone SE", @(GBDeviceDisplay4Inch), @326],
        // 7
        @[@9, @1]: @[@(GBDeviceModeliPhone7), @"iPhone 7", @(GBDeviceDisplay4p7Inch), @326],
        @[@9, @3]: @[@(GBDeviceModeliPhone7), @"iPhone 7", @(GBDeviceDisplay4p7Inch), @326],
        // 7 Plus
        @[@9, @2]: @[@(GBDeviceModeliPhone7Plus), @"iPhone 7 Plus", @(GBDeviceDisplay5p5Inch), @401],
        @[@9, @4]: @[@(GBDeviceModeliPhone7Plus), @"iPhone 7 Plus", @(GBDeviceDisplay5p5Inch), @401],
        // 8
        @[@10, @1]: @[@(GBDeviceModeliPhone8), @"iPhone 8", @(GBDeviceDisplay4p7Inch), @326],
        @[@10, @4]: @[@(GBDeviceModeliPhone8), @"iPhone 8", @(GBDeviceDisplay4p7Inch), @326],
        // 8 Plus
        @[@10, @2]: @[@(GBDeviceModeliPhone8Plus), @"iPhone 8 Plus", @(GBDeviceDisplay5p5Inch), @401],
        @[@10, @5]: @[@(GBDeviceModeliPhone8Plus), @"iPhone 8 Plus", @(GBDeviceDisplay5p5Inch), @401],
        // X
        @[@10, @3]: @[@(GBDeviceModeliPhoneX), @"iPhone X", @(GBDeviceDisplay5p8Inch), @458],
        @[@10, @6]: @[@(GBDeviceModeliPhoneX), @"iPhone X", @(GBDeviceDisplay5p8Inch), @458],
        // XR
        @[@11, @8]: @[@(GBDeviceModeliPhoneXR), @"iPhone XR", @(GBDeviceDisplay6p1Inch), @326],
        // XS
        @[@11, @2]: @[@(GBDeviceModeliPhoneXS), @"iPhone XS", @(GBDeviceDisplay5p8Inch), @458],
        // XS Max
        @[@11, @4]: @[@(GBDeviceModeliPhoneXSMax), @"iPhone XS Max", @(GBDeviceDisplay6p5Inch), @458],
        @[@11, @6]: @[@(GBDeviceModeliPhoneXSMax), @"iPhone XS Max", @(GBDeviceDisplay6p5Inch), @458],
    },
    @"iPad": @{
        // 1
        @[@1, @1]: @[@(GBDeviceModeliPad1), @"iPad 1", @(GBDeviceDisplay9p7Inch), @132],
        // 2
        @[@2, @1]: @[@(GBDeviceModeliPad2), @"iPad 2", @(GBDeviceDisplay9p7Inch), @132],
        @[@2, @2]: @[@(GBDeviceModeliPad2), @"iPad 2", @(GBDeviceDisplay9p7Inch), @132],
        @[@2, @3]: @[@(GBDeviceModeliPad2), @"iPad 2", @(GBDeviceDisplay9p7Inch), @132],
        @[@2, @4]: @[@(GBDeviceModeliPad2), @"iPad 2", @(GBDeviceDisplay9p7Inch), @132],
        // Mini
        @[@2, @5]: @[@(GBDeviceModeliPadMini1), @"iPad Mini 1", @(GBDeviceDisplay7p9Inch), @163],
        @[@2, @6]: @[@(GBDeviceModeliPadMini1), @"iPad Mini 1", @(GBDeviceDisplay7p9Inch), @163],
        @[@2, @7]: @[@(GBDeviceModeliPadMini1), @"iPad Mini 1", @(GBDeviceDisplay7p9Inch), @163],
        // 3
        @[@3, @1]: @[@(GBDeviceModeliPad3), @"iPad 3", @(GBDeviceDisplay9p7Inch), @264],
        @[@3, @2]: @[@(GBDeviceModeliPad3), @"iPad 3", @(GBDeviceDisplay9p7Inch), @264],
        @[@3, @3]: @[@(GBDeviceModeliPad3), @"iPad 3", @(GBDeviceDisplay9p7Inch), @264],
        // 4
        @[@3, @4]: @[@(GBDeviceModeliPad4), @"iPad 4", @(GBDeviceDisplay9p7Inch), @264],
        @[@3, @5]: @[@(GBDeviceModeliPad4), @"iPad 4", @(GBDeviceDisplay9p7Inch), @264],
        @[@3, @6]: @[@(GBDeviceModeliPad4), @"iPad 4", @(GBDeviceDisplay9p7Inch), @264],
        // Air
        @[@4, @1]: @[@(GBDeviceModeliPadAir1), @"iPad Air 1", @(GBDeviceDisplay9p7Inch), @264],
        @[@4, @2]: @[@(GBDeviceModeliPadAir1), @"iPad Air 1", @(GBDeviceDisplay9p7Inch), @264],
        @[@4, @3]: @[@(GBDeviceModeliPadAir1), @"iPad Air 1", @(GBDeviceDisplay9p7Inch), @264],
        // Mini 2
        @[@4, @4]: @[@(GBDeviceModeliPadMini2), @"iPad Mini 2", @(GBDeviceDisplay7p9Inch), @326],
        @[@4, @5]: @[@(GBDeviceModeliPadMini2), @"iPad Mini 2", @(GBDeviceDisplay7p9Inch), @326],
        @[@4, @6]: @[@(GBDeviceModeliPadMini2), @"iPad Mini 2", @(GBDeviceDisplay7p9Inch), @326],
        // Mini 3
        @[@4, @7]: @[@(GBDeviceModeliPadMini3), @"iPad Mini 3", @(GBDeviceDisplay7p9Inch), @326],
        @[@4, @8]: @[@(GBDeviceModeliPadMini3), @"iPad Mini 3", @(GBDeviceDisplay7p9Inch), @326],
        @[@4, @9]: @[@(GBDeviceModeliPadMini3), @"iPad Mini 3", @(GBDeviceDisplay7p9Inch), @326],
        // Mini 4
        @[@5, @1]: @[@(GBDeviceModeliPadMini4), @"iPad Mini 4", @(GBDeviceDisplay7p9Inch), @326],
        @[@5, @2]: @[@(GBDeviceModeliPadMini4), @"iPad Mini 4", @(GBDeviceDisplay7p9Inch), @326],
        // Air 2
        @[@5, @3]: @[@(GBDeviceModeliPadAir2), @"iPad Air 2", @(GBDeviceDisplay9p7Inch), @264],
        @[@5, @4]: @[@(GBDeviceModeliPadAir2), @"iPad Air 2", @(GBDeviceDisplay9p7Inch), @264],
        // Pro 12.9-inch
        @[@6, @7]: @[@(GBDeviceModeliPadPro12p9Inch), @"iPad Pro 12.9-inch", @(GBDeviceDisplay12p9Inch), @264],
        @[@6, @8]: @[@(GBDeviceModeliPadPro12p9Inch), @"iPad Pro 12.9-inch", @(GBDeviceDisplay12p9Inch), @264],
        // Pro 9.7-inch
        @[@6, @3]: @[@(GBDeviceModeliPadPro9p7Inch), @"iPad Pro 9.7-inch", @(GBDeviceDisplay9p7Inch), @264],
        @[@6, @4]: @[@(GBDeviceModeliPadPro9p7Inch), @"iPad Pro 9.7-inch", @(GBDeviceDisplay9p7Inch), @264],
        // iPad 5th Gen, 2017
        @[@6, @11]: @[@(GBDeviceModeliPad5), @"iPad 2017", @(GBDeviceDisplay9p7Inch), @264],
        @[@6, @12]: @[@(GBDeviceModeliPad5), @"iPad 2017", @(GBDeviceDisplay9p7Inch), @264],
        // Pro 12.9-inch, 2017
        @[@7, @1]: @[@(GBDeviceModeliPadPro12p9Inch2), @"iPad Pro 12.9-inch 2017", @(GBDeviceDisplay12p9Inch), @264],
        @[@7, @2]: @[@(GBDeviceModeliPadPro12p9Inch2), @"iPad Pro 12.9-inch 2017", @(GBDeviceDisplay12p9Inch), @264],
        // Pro 10.5-inch, 2017
        @[@7, @3]: @[@(GBDeviceModeliPadPro10p5Inch2), @"iPad Pro 10.5-inch 2017", @(GBDeviceDisplay10p5Inch), @264],
        @[@7, @4]: @[@(GBDeviceModeliPadPro10p5Inch2), @"iPad Pro 10.5-inch 2017", @(GBDeviceDisplay10p5Inch), @264],
        // iPad 6th Gen, 2018
        @[@7, @5]: @[@(GBDeviceModeliPad6), @"iPad 2018", @(GBDeviceDisplay9p7Inch), @264],
        @[@7, @6]: @[@(GBDeviceModeliPad6), @"iPad 2018", @(GBDeviceDisplay9p7Inch), @264],
    },
    @"iPod": @{
        // 1st Gen
        @[@1, @1]: @[@(GBDeviceModeliPod1), @"iPod Touch 1", @(GBDeviceDisplay3p5Inch), @163],
        // 2nd Gen
        @[@2, @1]: @[@(GBDeviceModeliPod2), @"iPod Touch 2", @(GBDeviceDisplay3p5Inch), @163],
        // 3rd Gen
        @[@3, @1]: @[@(GBDeviceModeliPod3), @"iPod Touch 3", @(GBDeviceDisplay3p5Inch), @163],
        // 4th Gen
        @[@4, @1]: @[@(GBDeviceModeliPod4), @"iPod Touch 4", @(GBDeviceDisplay3p5Inch), @326],
        // 5th Gen
        @[@5, @1]: @[@(GBDeviceModeliPod5), @"iPod Touch 5", @(GBDeviceDisplay4Inch), @326],
        // 6th Gen
        @[@7, @1]: @[@(GBDeviceModeliPod6), @"iPod Touch 6", @(GBDeviceDisplay4Inch), @326],
    }};
    struct utsname systemInfo;
    uname(&systemInfo);
    NSString *machine = [NSString stringWithCString:systemInfo.machine encoding:NSUTF8StringEncoding];
    NSUInteger positionOfFirstInteger = [machine rangeOfCharacterFromSet:[NSCharacterSet decimalDigitCharacterSet]].location;
    NSUInteger positionOfComma = [machine rangeOfString:@","].location;
    NSUInteger major = 0;
    NSUInteger minor = 0;
    if (positionOfComma != NSNotFound) {
        major = [[machine substringWithRange:NSMakeRange(positionOfFirstInteger, positionOfComma - positionOfFirstInteger)] integerValue];
        minor = [[machine substringFromIndex:positionOfComma + 1] integerValue];
    }
    for (NSString *familyString in familyManifest) {
        if ([machine hasPrefix:familyString]) {
            NSArray *modelNuances = modelManifest[familyString][@[@(major), @(minor)]];
            if (modelNuances) {
                pixelsPerInch = [modelNuances[3] doubleValue];
            } else {
                pixelsPerInch = [UIFont systemFontOfSize:72].lineHeight;
            }
            break;
        }
    }
    return pixelsPerInch;
}

CX_CPP_BEGIN

CX_IMPLEMENT(cxIOSUtil);

cxStr *cxIOSUtil::GetLang()
{
    NSLocale *locale = [NSLocale currentLocale];
    return cxStr::UTF8([[locale objectForKey:NSLocaleLanguageCode] UTF8String]);
}

cxStr *cxIOSUtil::GetCountry()
{
    NSLocale *locale = [NSLocale currentLocale];
    return cxStr::UTF8([[locale objectForKey:NSLocaleCountryCode] UTF8String]);
}

void cxIOSUtil::Logger(const char* type,const char*file,int line,const char* format,va_list ap)
{
    char *buffer = nullptr;
    int bytes = vasprintf(&buffer, format, ap);
    NSString *txt = [NSString stringWithUTF8String:buffer];
    FILE *fout = stdout;
    if(strcmp(type, "ERROR") == 0){
        fout = stderr;
    }
    fprintf(fout, "[%s:%d] %s:%s[%d]\n",file,line,type,[txt UTF8String],bytes);
    free(buffer);
}

cxStr *cxIOSUtil::ExtPath(cchars file)
{
    return DocumentPath(file);
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

cxInt cxIOSUtil::GetVersionCode()
{
    NSDictionary *info = [[NSBundle mainBundle] infoDictionary];
    NSString *build = [info objectForKey:@"CFBundleVersion"];
    return [build integerValue];
}

cxStr *cxIOSUtil::GetVersionName()
{
    NSDictionary *info = [[NSBundle mainBundle] infoDictionary];
    NSString *build = [info objectForKey:@"CFBundleShortVersionString"];
    return cxStr::UTF8(build.UTF8String);
}

cxStr *cxIOSUtil::UUID()
{
    uuid_t uuid;
    uuid_generate(uuid);
    return cxStr::Create()->Init(uuid, sizeof(uuid_t));
}

cxFloat cxIOSUtil::ScaleFactor()
{
    return (cxFloat)[UIScreen mainScreen].scale;
}

cxFloat cxIOSUtil::GetDPI()
{
    return (cxFloat)getDPI();
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

