//
//  config.h
//  RaknetServer
//
//  Created by xuhua on 5/8/16.
//  Copyright © 2016 xuhua. All rights reserved.
//

#ifndef config_h
#define config_h

#include "MongoDB.h"

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
    MongoDB *db;
    uv_loop_t loop;
    void *server;
public:
    void SetServer(void *ptr);
    uv_loop_t *Looper();
    MongoDB *GetDB();
};

CX_CPP_END

#endif /* config_h */
