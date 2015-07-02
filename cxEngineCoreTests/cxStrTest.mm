//
//  cxStrTest.m
//  cxEngineCore
//
//  Created by xuhua on 5/24/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#include <sstream>
#include <core/cxStr.h>
#import <UIKit/UIKit.h>
#import <XCTest/XCTest.h>
#include <core/cxAutoPool.h>

using namespace cxengine;

@interface cxStrTest : XCTestCase

@end

@implementation cxStrTest

- (void)setUp {
    [super setUp];
    // Put setup code here. This method is called before the invocation of each test method in the class.
}

- (void)tearDown {
    // Put teardown code here. This method is called after the invocation of each test method in the class.
    [super tearDown];
}

-(void)testAES
{
    cxStr *s = cxStr::Alloc()->Init("12345678901234567890123456789012345678901234567890123456789012345678901234567890");
    const cxStr *l = s->AESEncode(cxStr::UTF8("1234567"));
    const cxStr *r = l->AESDecode(cxStr::UTF8("1234567"));
    XCTAssertTrue(s->IsEqu(r),"test failed");
    const cxStr *r2 = l->AESDecode(cxStr::UTF8("12345678"));
    XCTAssertTrue(!s->IsEqu(r2),"test failed");
    s->Release();
}

-(void)testUTF8
{
    cxStr *s = cxStr::Alloc()->Init("我和123");
    XCTAssertTrue(s->Size() == 9,"test failed");
    XCTAssertTrue(s->UTF8Size() == 5,"test failed");
    s->Release();
}

-(void)testTEA
{
    cxStr *s = cxStr::Alloc()->Init("12345678901234567890123456789012345678901234567890123456789012345678901234567890");
    const cxStr *l = s->TeaEncode(cxStr::UTF8("1234567"));
    const cxStr *r = l->TeaDecode(cxStr::UTF8("1234567"));
    XCTAssertTrue(s->IsEqu(r),"test failed");
    const cxStr *r2 = l->TeaDecode(cxStr::UTF8("12345678"));
    XCTAssertTrue(!s->IsEqu(r2),"test failed");
    s->Release();
}

-(void)testLZMA
{
    cxStr *s = cxStr::Alloc()->Init("12345678901234567890123456789012345678901234567890123456789012345678901234567890");
    const cxStr *l = s->LzmaCompress();
    const cxStr *r = l->LzmaUncompress();
    XCTAssertTrue(s->IsEqu(r),"test failed");
    s->Release();
}

-(void)testMD5{
    cxStr *s = cxStr::Alloc()->Init("helloword");
    const cxStr *md5 = s->MD5();
    XCTAssertTrue(md5->IsEqu("59284aa85709ddaf3bd246030060f6a2"),"md5 error");
    s->Release();
}

-(void)testBuffer{
    cxStr *s = cxStr::Alloc()->Init(20, 0);
    chars buffer = s->Buffer();
    memcpy(buffer, "1234567890123456789", 20);
    XCTAssertTrue(s->IsEqu("1234567890123456789"),"== operator error");
    s->Release();
}

-(void)testFormat{
    cxStr *s = cxStr::Alloc()->AppFmt("%d",1234);
    XCTAssertTrue(s->IsEqu("1234"),"== operator error");
    s->Release();
}

- (void)testAppend {

    cxStr *s = cxStr::Alloc();
    s->Append("123");
    XCTAssertTrue(s->At(0) == '1',"== operator error");
    XCTAssertTrue(s->At(1) == '2',"== operator error");
    XCTAssertTrue(s->At(2) == '3',"== operator error");
    XCTAssertTrue(s->IsEqu("123"),"== operator error");
    s->Append("456");
    
    XCTAssertTrue(s->HasPrefix("123"),"== operator error");
    XCTAssertTrue(s->HasSuffix("456"),"== operator error");
    
    XCTAssertTrue(s->IsEqu("123456"),"== operator error");
    s->Erase(0, 2);
    XCTAssertTrue(s->IsEqu("3456"),"== operator error");
    s->Erase(1, 2);
    XCTAssertTrue(s->IsEqu("36"),"== operator error");
    s->Clear();
    XCTAssertTrue(s->Size() == 0,"== operator error");
    s->Release();
}

- (void)testPerformanceExample {
    // This is an example of a performance test case.
    [self measureBlock:^{
        // Put the code you want to measure the time of here.
    }];
}

@end
