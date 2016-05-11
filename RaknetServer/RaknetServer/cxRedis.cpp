//
//  cxRedis.cpp
//  RaknetServer
//
//  Created by xuhua on 5/10/16.
//  Copyright © 2016 xuhua. All rights reserved.
//


#include "cxRedis.h"
#include <hiredis/adapters/libuv.h>

CX_CPP_BEGIN

/*
 #define REDIS_REPLY_STRING 1
 #define REDIS_REPLY_ARRAY 2
 #define REDIS_REPLY_INTEGER 3
 #define REDIS_REPLY_NIL 4
 #define REDIS_REPLY_STATUS 5
 #define REDIS_REPLY_ERROR 6
 */

CX_IMPLEMENT(cxRedisReply)

cxRedisReply::cxRedisReply()
{
    reply = nullptr;
}

cxRedisReply::~cxRedisReply()
{
    
}

bool cxRedisReply::IsNull()
{
    if(reply == nullptr){
        return true;
    }
    return reply->type == REDIS_REPLY_NIL;
}

bool cxRedisReply::IsError()
{
    return reply->type == REDIS_REPLY_ERROR;
}

bool cxRedisReply::IsInt()
{
    return reply->type == REDIS_REPLY_INTEGER;
}

bool cxRedisReply::IsString()
{
    return reply->type == REDIS_REPLY_STRING;
}

bool cxRedisReply::IsArray()
{
    return reply->type == REDIS_REPLY_ARRAY;
}

const cxStr *cxRedisReply::Error()
{
    return cxStr::Create()->Init(reply->str, reply->len);
}

cxInt64 cxRedisReply::ToInt()
{
    CX_ASSERT(IsInt(), "type error");
    return reply->integer;
}

const cxStr *cxRedisReply::ToString()
{
    CX_ASSERT(IsString(), "type error");
    return cxStr::Create()->Init(reply->str, reply->len);
}

const cxArray *cxRedisReply::ToArray()
{
    CX_ASSERT(IsArray(), "type error");
    cxArray *rets = cxArray::Create();
    for(int i=0;i<reply->elements;i++){
        cxRedisReply *r = cxRedisReply::Alloc();
        r->Init(reply->element[i]);
        rets->Append(r);
        r->Release();
    }
    return rets;
}

cxRedisReply *cxRedisReply::Init(redisReply *r)
{
    reply = r;
    return this;
}

CX_IMPLEMENT(cxRedis);

cxRedis::cxRedis()
{
    c = nullptr;
}

cxRedis::~cxRedis()
{
    if(c != nullptr){
        redisAsyncFree(c);
    }
}

void cxRedis::Close()
{
    CX_ASSERT(c != nullptr, "redis not init");
    redisAsyncDisconnect(c);
}

struct CommandInfo {
    void *data;
    std::function<void(const cxRedisReply *reply)> func;
};

void cxRedis::commandCallback(struct redisAsyncContext *c, void *r, void *data){
    CommandInfo *info = (CommandInfo *)data;
    cxRedisReply *rep = cxRedisReply::Alloc()->Init((redisReply *)r);
    info->func(rep);
    rep->Release();
    delete info;
}

void cxRedis::AsyncCommand(std::function<void(const cxRedisReply *reply)> func, cchars cmd,...){
    CX_ASSERT(c != nullptr, "redis not init");
    CommandInfo *info = new CommandInfo();
    info->data = this;
    info->func = func;
    va_list ap;
    va_start(ap, cmd);
    redisvAsyncCommand(c, cxRedis::commandCallback, info, cmd, ap);
    va_end(ap);
}

void cxRedis::AsyncCommand(cchars cmd,...){
    CX_ASSERT(c != nullptr, "redis not init");
    va_list ap;
    va_start(ap, cmd);
    redisvAsyncCommand(c, NULL, NULL, cmd, ap);
    va_end(ap);
}

void cxRedis::connectCallback(const redisAsyncContext *c, int status) {
    if (status != REDIS_OK) {
        CX_ERROR("connect error: %s", c->errstr);
        return;
    }
    cxRedis *self = (cxRedis *)c->data;
    self->onConnected.Fire(self);
}

void cxRedis::disconnectCallback(const redisAsyncContext *c, int status) {
    if (status != REDIS_OK) {
        CX_ERROR("disconnect error: %s", c->errstr);
        return;
    }
    cxRedis *self = (cxRedis *)c->data;
    self->onDisConnected.Fire(self);
}

bool cxRedis::Init(cchars host,cxInt port,uv_loop_t *looper)
{
    CX_ASSERT(looper != nullptr, "looper null");
    c = redisAsyncConnect(host, port);
    if(c->err){
        CX_ERROR("redis async connect error:%s",c->errstr);
        return false;
    }
    c->data = this;
    redisLibuvAttach(c, looper);
    redisAsyncSetConnectCallback(c,cxRedis::connectCallback);
    redisAsyncSetDisconnectCallback(c,cxRedis::disconnectCallback);
    return true;
}

CX_CPP_END

