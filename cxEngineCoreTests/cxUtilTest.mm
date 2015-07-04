//
//  cxUtilTest.m
//  cxEngineCore
//
//  Created by xuhua on 5/30/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <XCTest/XCTest.h>
#include <ios/cxIOSUtil.h>

using namespace cxengine;

@interface cxUtilTest : XCTestCase

@end

@implementation cxUtilTest

- (void)setUp {
    [super setUp];
    // Put setup code here. This method is called before the invocation of each test method in the class.
}

- (void)tearDown {
    // Put teardown code here. This method is called after the invocation of each test method in the class.
    [super tearDown];
}

-(void)testWriteData{
    cxUtil *util = cxIOSUtil::Instance();
    util->WriteDocument("test.dat", cxStr::UTF8("123456"), true);
    const cxStr *data = util->DocumentData("test.dat");
    XCTAssertTrue(data->IsEqu("123456"),"write field error");
    XCTAssertTrue(util->DeleteDocument("test.dat"),"write field error");
}

- (void)testAssertData {
    cxUtil *util = cxIOSUtil::Instance();
    const cxStr *path = util->AssetsPath("test.json");
    XCTAssertTrue(path->HasSuffix("test.json"),"test path error");
}

- (void)testPerformanceExample {
    // This is an example of a performance test case.
    [self measureBlock:^{
        // Put the code you want to measure the time of here.
    }];
}

@end
