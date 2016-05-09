//
//  Config.cpp
//  RaknetServer
//
//  Created by xuhua on 5/8/16.
//  Copyright © 2016 xuhua. All rights reserved.
//


#include "Config.h"
#include "main.h"

CX_CPP_BEGIN

CX_IMPLEMENT(Config);

Config::Config()
{
    uv_loop_init(&loop);
    db = DB::Alloc();
}

Config::~Config()
{
    db->Release();
    uv_loop_close(&loop);
}

uv_loop_t *Config::Looper()
{
    return &loop;
}

DB *Config::GetDB()
{
    return db;
}

void Config::SetServer(void *ptr)
{
    server = ptr;
}

void Config::IncCurr(cxInt c)
{
    GameServer *s = (GameServer *)server;
    BSONObj d = BSON("$inc" << BSON("curr" << c));
    db->UpdateId(T_SERVERS, s->GetId(), d);
}

CX_CPP_END

