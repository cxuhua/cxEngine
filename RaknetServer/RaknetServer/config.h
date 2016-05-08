//
//  config.h
//  RaknetServer
//
//  Created by xuhua on 5/8/16.
//  Copyright © 2016 xuhua. All rights reserved.
//

#ifndef config_h
#define config_h

#include <mongo/client/dbclient.h>
#include <core/cxObject.h>

//数据库连接
#define DB_HOST     "mongodb://127.0.0.1:27017"

//每隔多少秒更新服务器状态
#define UPDATE_SERVER_STATUS_TIME   10000

//服务器表
#define DB_NAME         "test"
#define NS_SERVERS      DB_NAME".servers"

//

using namespace mongo;

CX_CPP_BEGIN

class Config : public cxObject
{
public:
    CX_DECLARE(Config);
protected:
    explicit Config();
    virtual ~Config();
private:
    DBClientBase *conn;
public:
    DBClientBase *Conn();
};

CX_CPP_END

#endif /* config_h */
