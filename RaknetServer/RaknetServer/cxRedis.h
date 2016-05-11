//
//  cxRedis.h
//  RaknetServer
//
//  Created by xuhua on 5/10/16.
//  Copyright © 2016 xuhua. All rights reserved.
//

#ifndef RaknetServer_cxRedis_h
#define RaknetServer_cxRedis_h

#include <core/cxArray.h>
#include <hiredis/hiredis.h>
#include <hiredis/async.h>
#include <core/cxStr.h>

CX_CPP_BEGIN

class cxRedisReply : public cxObject
{
public:
    CX_DECLARE(cxRedisReply);
protected:
    explicit cxRedisReply();
    virtual ~cxRedisReply();
private:
    redisReply *reply;
public:
    cxRedisReply *Init(redisReply *r);
    bool IsNull();
    
    bool IsError();
    const cxStr *Error();
    
    bool IsInt();
    cxInt64 ToInt();
    
    bool IsString();
    const cxStr *ToString();
    
    bool IsArray();
    const cxArray *ToArray();//cxRedisReply
};

class cxRedis : public cxObject
{
public:
    CX_DECLARE(cxRedis);
protected:
    explicit cxRedis();
    virtual ~cxRedis();
private:
    redisAsyncContext *c;
    static void connectCallback(const redisAsyncContext *c, int status);
    static void disconnectCallback(const redisAsyncContext *c, int status);
    static void commandCallback(struct redisAsyncContext *c, void *r, void *data);
public:
    void Close();
    bool Init(cchars host,cxInt port,uv_loop_t *looper);
    void AsyncCommand(std::function<void(const cxRedisReply *reply)> func, cchars cmd,...);
    void AsyncCommand(cchars cmd,...);
public:
    cxEvent<cxRedis> onConnected;
    cxEvent<cxRedis> onDisConnected;
};

CX_CPP_END

#endif


