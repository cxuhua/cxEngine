//
//  cxArray.m
//  cxEngineCore
//
//  Created by xuhua on 5/23/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#include <core/cxArray.h>
#include <core/cxAutoPool.h>
#include <core/cxHash.h>
#include <core/cxQueue.h>
#include <core/cxList.h>
#include <core/cxUnique.h>
#import <XCTest/XCTest.h>

using namespace cxengine;

@interface cxArrayTest : XCTestCase
{
    
}
@end

class Test :public cxObject
{
public:
    CX_DECLARE(Test);
protected:
    explicit Test(){};
    virtual ~Test(){};
};

static cxInt sortFunc(const void *lp,const void *rp)
{
    cxObject *lv = *(cxObject **)lp;
    cxObject *rv = *(cxObject **)rp;
    return rv->Tag() - lv->Tag();
}

@implementation cxArrayTest

- (void)setUp {
    [super setUp];
}

- (void)tearDown {
    [super tearDown];
}

-(void)testUnique
{
    cxUnique *u = cxUnique::Alloc();
    Test *t = Test::Alloc();
    
    u->Append(t);
    u->Append(t);
    
    XCTAssertEqual(u->Size(), 1,"count test");
    
    t->Release();
    u->Release();
}

-(void)testList
{
    cxList *l = cxList::Alloc();
    cxObject *obj = cxObject::Alloc();
    l->PushBack(obj);
    XCTAssertEqual(l->Size(), 1,"count test");
    XCTAssertEqual(obj->Refcount(), 2,"refcount test");
    l->Remove(obj);
    XCTAssertEqual(l->Size(), 0,"count test");
    XCTAssertEqual(obj->Refcount(), 1,"refcount test");
    
    obj->Release();
    l->Release();
}

-(void)testSort{
    cxArray *list = cxArray::Alloc();
    for(int i=0; i < 100; i++){
        cxObject *obj = cxObject::Alloc();
        obj->SetTag(i);
        list->Append(obj);
        obj->Release();
    }
    list->Sort(sortFunc);
    for(int i=0; i < 100; i++){
        cxObject *obj = list->At<cxObject>(i);
        XCTAssertEqual(obj->Tag(), 99-i,"test replace");
    }
    list->Release();
}

-(void)testEach{
    cxArray *list = cxArray::Alloc();
    for(int i=0; i < 100; i++){
        cxObject *obj = cxObject::Alloc();
        list->Append(obj);
        obj->Release();
    }
    list->Clear();
}

-(void)testReplace{
    cxArray *list = cxArray::Alloc();
    for(int i=0; i < 100; i++){
        cxObject *obj = cxObject::Alloc();
        obj->SetTag(i);
        list->Append(obj);
        obj->Release();
    }
    cxObject *obj = cxObject::Create();
    obj->SetTag(1024);
    list->Replace(50, obj);
    XCTAssertEqual(list->At<cxObject>(50)->Tag(), 1024,"test replace");
    list->Clear();
    list->Release();
}

-(void)testHashEach
{
    cxHash *hash = cxHash::Alloc();
    cxObject *obj = cxObject::Alloc();
    hash->Set("0", obj);
    hash->Set("1", obj);
    hash->Set("2", obj);
    hash->Set("3", obj);
    
    cxInt i = 0;
    for(cxHash::Iter iter=hash->Begin();iter != hash->End();iter++){
        i++;
    }
    XCTAssertEqual(4, i,"hash iter failed");
    
    for(cxHash::Iter iter=hash->Begin();iter != hash->End();){
        iter = hash->Remove(iter);
    }
    
    XCTAssertEqual(0, hash->Size(),"remove iter  error");
    
    cxObject::release(&hash);
    cxObject::release(&obj);
}


-(void)testQueue
{
    cxObject *obj = cxObject::Alloc();
    cxQueue *q = cxQueue::Alloc();
    q->Push(obj);
    XCTAssertEqual(obj->Refcount(), 2,"refcount test");
    XCTAssertEqual(q->Size(), 1,"count test");
    
    XCTAssertEqual(q->Front(), obj,"object test");
    XCTAssertEqual(q->Back(), obj,"object test");
    
    q->Pop();
    XCTAssertEqual(obj->Refcount(), 1,"refcount test");
    XCTAssertEqual(q->Size(), 0,"count test");
    
    cxObject *obj1 = cxObject::Alloc();
    q->Push(obj1);
    cxObject *obj2 = cxObject::Alloc();
    q->Push(obj2);
    XCTAssertEqual(q->Size(), 2,"count test");
    XCTAssertEqual(q->Front(), obj1,"object test");
    XCTAssertEqual(q->Back(), obj2,"object test");
    q->Pop();
    XCTAssertEqual(q->Front(), obj2,"object test");
    
    q->Release();
    obj->Release();
    obj1->Release();
    obj2->Release();
}

-(void)testHash{
    cxHash *hash = cxHash::Alloc();
    cxObject *obj = cxObject::Alloc();
    hash->Set("0", obj);
    XCTAssertEqual(obj, hash->Get("0"),"int key error");
    XCTAssertEqual(nullptr, hash->Get("1"),"not exists test");
    XCTAssertEqual(obj->Refcount(), 2,"refcount test");
    
    XCTAssertEqual(1, hash->Size(),"not exists test");
    
    XCTAssertEqual(obj->Refcount(), 2,"refcount test");
    hash->Del("0");
    XCTAssertEqual(obj->Refcount(), 1,"refcount test");
    
    XCTAssertEqual(nullptr, hash->Get("0"),"not exists test");
    XCTAssertEqual(0, hash->Size(),"not exists test");
    
    hash->Set(11, obj);
    XCTAssertEqual(obj, hash->Get(11),"not exists test");
    hash->Set(22, obj);
    XCTAssertEqual(obj, hash->Get(22),"not exists test");
    hash->Set(33, obj);
    XCTAssertEqual(obj, hash->Get(33),"not exists test");
    XCTAssertEqual(obj->Refcount(), 4,"refcount test");
    
    cxObject::release(&hash);
    cxObject::release(&obj);
}

-(void)testPush {
    cxStack *list = cxStack::Alloc();
    cxObject *obj = cxObject::Alloc();
    list->Push(obj);
    XCTAssertEqual(obj, list->Top(),"Push errror");
    list->Pop();
    XCTAssertEqual(nullptr, list->Top(),"Push errror");
    obj->Release();
}

@end
