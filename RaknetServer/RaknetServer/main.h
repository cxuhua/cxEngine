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
#include "Config.h"

CX_CPP_BEGIN

class GameServer : public cxServer
{
public:
    CX_DECLARE(GameServer);
protected:
    explicit GameServer();
    virtual ~GameServer();
private:
    uv_timer_t timer;
    uv_loop_t loop;
    //服务器信息
    std::string id;
    std::string host;
    std::string pass;
    cxInt port;
    cxInt max;
    cxInt curr;
    static void updateServerStatus(uv_timer_t* handle);
    //
    MongoDB *db;
public:
    MongoDB *GetDB();
    cchars GetHost();
    cxInt GetPort();
    cchars GetId();
    cxInt GetMax();
    cxInt GetCurr();
public:
    void OnMessage(RakNet::RakNetGUID clientId,const cxStr *message);
    void OnNewConnect(RakNet::RakNetGUID clientId);
    void OnLost(RakNet::RakNetGUID clientId);
    
    void ThreadBegin();
    void ThreadLoop();
    void ThreadExit();
    
    void Stop();
    void Run();
public:
    static int Main(int argc, const char * argv[]);
    //注册服务器
    void Register(cchars id, cchars host, cxInt port,cchars pass,cxInt max);
};

CX_CPP_END

#endif /* main_h */
