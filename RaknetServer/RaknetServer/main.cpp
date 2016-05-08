//
//  main.cpp
//  RaknetServer
//
//  Created by xuhua on 5/3/16.
//  Copyright © 2016 xuhua. All rights reserved.
//

#include <core/cxUtil.h>
#include <raknet/cxServer.h>
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
public:
    void *ThreadData();
};

CX_IMPLEMENT(cxGameServer);

void *cxGameServer::ThreadData()
{
    return nullptr;
}

void timer_cb(uv_timer_t* handle)
{
    CX_LOGGER("timer %f at %p",cxUtil::Timestamp(),uv_thread_self());
    uv_timer_again(handle);
}

cxGameServer::cxGameServer()
{
    uv_timer_init(Looper(), &timer);
    uv_timer_start(&timer, timer_cb, 1000, 1000);
}

cxGameServer::~cxGameServer()
{
    
}

int main(int argc, const char * argv[])
{
    cxGameServer *server = cxGameServer::Alloc();
    if(!server->Init(5, 10020, 512, "123")){
        exit(1);
    }
    server->Run();
    return 0;
}
