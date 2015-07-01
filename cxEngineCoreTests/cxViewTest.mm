//
//  cxViewTest.m
//  cxEngineCore
//
//  Created by xuhua on 7/1/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#include <engine/cxView.h>
#import <UIKit/UIKit.h>
#import <XCTest/XCTest.h>

using namespace cxengine;

@interface cxViewTest : XCTestCase

@end

@implementation cxViewTest

- (void)setUp {
    [super setUp];
    // Put setup code here. This method is called before the invocation of each test method in the class.
}

- (void)tearDown {
    // Put teardown code here. This method is called after the invocation of each test method in the class.
    [super tearDown];
}

- (void)testBind {
    cxView *v1 = cxView::Alloc();
    cxView *v2 = cxView::Alloc();
    v2->SetTag(10010);
    
    v1->Bind(v2);
    
    //v1 bind v2
    XCTAssertTrue(v1->HasBindes(v2),"bind error");
    
    //v2 被 v1 bind
    XCTAssertTrue(v2->HasBinded(v1),"bind error");
    
    v1->EachBindes([v2,self](cxObject *obj){
        XCTAssertTrue(v2 == obj,"bind error");
    });
    
    v2->EachBinded([v1,self](cxObject *obj){
        XCTAssertTrue(v1 == obj,"bind error");
    });
    
    v1->Release();
    v2->Release();
}

- (void)testPerformanceExample {
    // This is an example of a performance test case.
    [self measureBlock:^{
        // Put the code you want to measure the time of here.
    }];
}

@end
