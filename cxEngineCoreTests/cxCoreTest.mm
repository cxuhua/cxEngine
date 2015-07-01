//
//  cxCore.m
//  cxEngineCore
//
//  Created by xuhua on 5/23/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#include <core/cxCore.h>
#include <core/cxAutoPool.h>
#import <XCTest/XCTest.h>
#include <core/cxNumber.h>

using namespace cxengine;

@interface cxCoreTest : XCTestCase

@end

@implementation cxCoreTest

- (void)setUp {
    [super setUp];
    // Put setup code here. This method is called before the invocation of each test method in the class.
}

- (void)tearDown {
    // Put teardown code here. This method is called after the invocation of each test method in the class.
    [super tearDown];
}

-(void)testNumber{
    cxNumber *num = cxNumber::Create(123);
    XCTAssertEqual(num->ToInt(), 123,"number int error");
    num = cxNumber::Create(123.1f);
    XCTAssertTrue(num->GetType() == cxNumber::Float,"type errpr");
    num = cxNumber::Create(123.1);
    XCTAssertTrue(num->GetType() == cxNumber::Double,"type errpr");
    num = cxNumber::Create(false);
    XCTAssertTrue(num->GetType() == cxNumber::Bool,"type errpr");
    num = cxNumber::Create((int64_t)123);
    XCTAssertTrue(num->GetType() == cxNumber::Int64,"type errpr");
}

-(void)testAutoRelease
{
    cxObject *obj = cxObject::Alloc();
    obj->AutoRelease();
    cxAutoPool::Clear();
}

- (void)testObject {
    cxObject *obj = cxObject::Alloc();
    XCTAssertEqual(obj->Refcount(), 1," init refcount = 1");
    obj->Retain();
    XCTAssertEqual(obj->Refcount(), 2," Retain refcount = 1");
    obj->Release();
    XCTAssertEqual(obj->Refcount(), 1," Release refcount = 1");
    obj->Release();
}

- (void)testPerformanceExample {
    // This is an example of a performance test case.
    [self measureBlock:^{
        
    }];
}

@end
