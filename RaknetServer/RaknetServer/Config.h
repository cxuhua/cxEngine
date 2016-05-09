//
//  config.h
//  RaknetServer
//
//  Created by xuhua on 5/8/16.
//  Copyright © 2016 xuhua. All rights reserved.
//

#ifndef config_h
#define config_h

#include "DB.h"

//每隔多少秒更新服务器状态
#define UPDATE_STATUS_TIME   10000

CX_CPP_BEGIN

class Config : public cxObject
{
public:
    CX_DECLARE(Config);
protected:
    explicit Config();
    virtual ~Config();
private:
    DB *db;
    uv_loop_t loop;
    void *server;
public:
    void SetServer(void *ptr);
    uv_loop_t *Looper();
    DB *GetDB();
public:
    //更新当前在线人数
    void IncCurr(cxInt c);
};

CX_CPP_END

#endif /* config_h */
