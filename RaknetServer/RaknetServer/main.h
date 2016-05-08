//
//  main.h
//  RaknetServer
//
//  Created by xuhua on 5/8/16.
//  Copyright © 2016 xuhua. All rights reserved.
//

#ifndef main_h
#define main_h

#include <core/cxUtil.h>
#include <raknet/cxServer.h>
#include "config.h"

using namespace cxengine;

class cxGameServer : public cxServer
{
public:
    CX_DECLARE(cxGameServer);
protected:
    explicit cxGameServer();
    virtual ~cxGameServer();
private:
    uv_timer_t timer;
    //服务器信息
    std::string id;
    std::string host;
    int port;
    
    static void updateServerStatus(uv_timer_t* handle);
    
public:
    void ThreadBegin();
    void ThreadExit();
public:
    //注册服务器
    void Register(cchars id, cchars host, int port);
};

#endif /* main_h */
