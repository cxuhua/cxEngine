//
//  cxAstarTest.m
//  cxEngineCore
//
//  Created by xuhua on 7/4/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#include <engine/cxAstar.h>
#import <UIKit/UIKit.h>
#import <XCTest/XCTest.h>

using namespace cxengine;

const int MAP_WIDTH = 20;
const int MAP_HEIGHT = 20;

int world_map[ MAP_WIDTH * MAP_HEIGHT ] =
{
    
    // 0001020304050607080910111213141516171819
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,   // 00
    1,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,1,   // 01
    1,9,9,1,1,9,9,9,1,9,1,9,1,9,1,9,9,9,1,1,   // 02
    1,9,9,1,1,9,9,9,1,9,1,9,1,9,1,9,9,9,1,1,   // 03
    1,9,1,1,1,1,9,9,1,9,1,9,1,1,1,1,9,9,1,1,   // 04
    1,9,1,1,9,1,1,1,1,9,1,1,1,1,9,1,1,1,1,1,   // 05
    1,9,9,9,9,1,1,1,1,1,1,9,9,9,9,1,1,1,1,1,   // 06
    1,9,9,9,9,9,9,9,9,1,1,1,9,9,9,9,9,9,9,1,   // 07
    1,9,1,1,1,1,1,1,1,1,1,9,1,1,1,1,1,1,1,1,   // 08
    1,9,1,9,9,9,9,9,9,9,1,1,9,9,9,9,9,9,9,1,   // 09
    1,9,1,1,1,1,9,1,1,9,1,1,1,1,1,1,1,1,1,1,   // 10
    1,9,9,9,9,9,1,9,1,9,1,9,9,9,9,9,1,1,1,1,   // 11
    1,9,1,9,1,9,9,9,1,9,1,9,1,9,1,9,9,9,1,1,   // 12
    1,9,1,9,1,9,9,9,1,9,1,9,1,9,1,9,9,9,1,1,   // 13
    1,9,1,1,1,1,9,9,1,9,1,9,1,1,1,1,9,9,1,1,   // 14
    1,9,1,1,9,1,1,1,1,9,1,1,1,1,9,1,1,1,1,1,   // 15
    1,9,9,9,9,1,1,1,1,1,1,9,9,9,9,1,1,1,1,1,   // 16
    1,1,9,9,9,9,9,9,9,1,1,1,9,9,9,1,9,9,9,9,   // 17
    1,9,1,1,1,1,1,1,1,1,1,9,1,1,1,1,1,1,1,1,   // 18
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,   // 19
    
};

// map helper functions

int GetMap( int x, int y )
{
    if( x < 0 ||
       x >= MAP_WIDTH ||
       y < 0 ||
       y >= MAP_HEIGHT
       )
    {
        return 9;	 
    }
    
    return world_map[(y*MAP_WIDTH)+x];
}

class cxAstarMap : public cxAstar
{
public:
    CX_DECLARE(cxAstarMap);
protected:
    explicit cxAstarMap()
    {
        
    }
    virtual ~cxAstarMap()
    {
        
    }
    cxBool IsAppend(const cxPoint2I &point)
    {
        return GetMap(point.x,point.y) == 1;
    }
};

CX_IMPLEMENT(cxAstarMap);

@interface cxAstarTest : XCTestCase

@end

@implementation cxAstarTest

- (void)setUp {
    [super setUp];
    // Put setup code here. This method is called before the invocation of each test method in the class.
}

- (void)tearDown {
    // Put teardown code here. This method is called after the invocation of each test method in the class.
    [super tearDown];
}

- (void)testAstar {
    cxAstarMap *s = cxAstarMap::Alloc();
    s->Init(cxPoint2I(0, 0), cxPoint2I(16,2));
    s->Step();
    XCTAssertTrue(s->Points().Size() == 0);
    s->Init(cxPoint2I(0, 0), cxPoint2I(12,2));
    s->Step();
    XCTAssertTrue(s->Points().Size() == 30);
    s->Release();
}

- (void)testPerformanceExample {
    // This is an example of a performance test case.
    [self measureBlock:^{
        // Put the code you want to measure the time of here.
    }];
}

@end
