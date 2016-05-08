//
//  Config.cpp
//  RaknetServer
//
//  Created by xuhua on 5/8/16.
//  Copyright © 2016 xuhua. All rights reserved.
//


#include "Config.h"

CX_CPP_BEGIN

CX_IMPLEMENT(Config);

Config::Config()
{
    std::string errmsg;
    ConnectionString cs = ConnectionString::parse(DB_HOST, errmsg);
    if (!cs.isValid()) {
        CX_ERROR("db host error");
        exit(1);
    }
    conn = cs.connect(errmsg);
}

Config::~Config()
{
    delete conn;
}

DBClientBase *Config::Conn()
{
    if(conn->isStillConnected()){
        return conn;
    }
    throw new DBException("db connect miss", 1000);
}

CX_CPP_END

