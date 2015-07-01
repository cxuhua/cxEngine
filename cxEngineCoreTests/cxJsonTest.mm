//
//  cxJsonTest.m
//  cxEngineCore
//
//  Created by xuhua on 5/29/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#include <core/cxJson.h>
#import <UIKit/UIKit.h>
#import <XCTest/XCTest.h>

using namespace cxengine;

@interface cxJsonTest : XCTestCase

@end

@implementation cxJsonTest

- (void)setUp {
    [super setUp];
    // Put setup code here. This method is called before the invocation of each test method in the class.
}

- (void)tearDown {
    // Put teardown code here. This method is called after the invocation of each test method in the class.
    [super tearDown];
}

-(void)testArrayEach
{
    cxJson *json = cxJson::Alloc()->Array();
    json->Append(false);
    json->Append(1200);
    json->Append(124.0f);
    json->Append("aaa");
    cxInt i = 0;
    for(cxJson::Iter it=json->Begin();it != json->End();it++){
        CX_LOGGER("%d",it.Index());
        if(i == 0){
            XCTAssertTrue(it.Value()->IsBool(),"0 error");
        }else if(i == 1){
            XCTAssertTrue(it.Value()->IsInt(),"1 error");
        }else if(i == 2){
            XCTAssertTrue(it.Value()->IsFloat(),"1 error");
        }else if(i == 3){
            XCTAssertTrue(it.Value()->IsString(),"1 error");
        }
        i++;
    }
    XCTAssertEqual(4, i,"test object json each error");
    json->Release();
}

-(void)testOperation
{
    cxJson *json = cxJson::Alloc()->Array();
    *json << cxJson::Key("a");
    *json << false;
    *json << 1200;
    *json << 124.f;
    *json << "aaa";
    cxInt i = 0;
    for(cxJson::Iter it=json->Begin();it != json->End();it++){
        CX_LOGGER("%d",it.Index());
        if(i == 0){
            XCTAssertTrue(it.Value()->IsBool(),"0 error");
        }else if(i == 1){
            XCTAssertTrue(it.Value()->IsInt(),"1 error");
        }else if(i == 2){
            XCTAssertTrue(it.Value()->IsFloat(),"1 error");
        }else if(i == 3){
            XCTAssertTrue(it.Value()->IsString(),"1 error");
        }
        i++;
    }
    XCTAssertEqual(4, i,"test object json each error");
    json->Release();
}

-(void)testObjectEach
{
    cxJson *json = cxJson::Alloc()->Object();
    json->Set("a", false);
    json->Set("b", 1200);
    json->Set("c", 123.4983492432f);
    json->Set("s", "string");
    
    cxInt i = 0;
    for(cxJson::Iter it=json->Begin();it != json->End();it++){
        if(cxStr::IsEqu(it.Key(), "a")){
            XCTAssertTrue(it.Value()->IsBool(),"a error");
        }else if(cxStr::IsEqu(it.Key(), "b")){
            XCTAssertTrue(it.Value()->IsInt(),"b error");
        }else if(cxStr::IsEqu(it.Key(), "c")){
            XCTAssertTrue(it.Value()->IsFloat(),"c error");
        }else if(cxStr::IsEqu(it.Key(), "s")){
            XCTAssertTrue(it.Value()->IsString(),"s error");
        }
        i++;
    }
    XCTAssertEqual(4, i,"test object json each error");
    json->Release();
}

- (void)testDumps {
    cxJson *json = cxJson::Alloc()->Object();
    json->Set("a", false);
    json->Set("b", 1200);
    json->Set("c", 123.4983492432f);
    json->Set("s", "string");
    XCTAssertTrue(json->Select("a")->IsBool(),"json select error");
    const cxStr *txt = json->Dumps();
    XCTAssertTrue(cxStr::IsOK(txt),"json dumps test failed");
    cxJson *j2 = cxJson::Alloc()->From(txt);
    XCTAssertTrue(j2->At("a")->IsBool(),"json dumps test failed");
    XCTAssertTrue(j2->At("b")->ToInt() == 1200,"json dumps test failed");
    j2->Release();
    json->Release();
}

- (void)testPerformanceExample {
    // This is an example of a performance test case.
    [self measureBlock:^{
        // Put the code you want to measure the time of here.
    }];
}

@end
