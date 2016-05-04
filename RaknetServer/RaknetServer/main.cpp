//
//  main.cpp
//  RaknetServer
//
//  Created by xuhua on 5/3/16.
//  Copyright © 2016 xuhua. All rights reserved.
//

#include <raknet/cxServer.h>
using namespace cxengine;

class cxGameServer : public cxServer
{
public:
    CX_DECLARE(cxGameServer);
protected:
    explicit cxGameServer();
    virtual ~cxGameServer();
};

CX_IMPLEMENT(cxGameServer);

cxGameServer::cxGameServer()
{
    
}

cxGameServer::~cxGameServer()
{
    
}

int main(int argc, const char * argv[]) {
    
    cxGameServer *server = cxGameServer::Alloc();
    server->Init(5, 10020, 512, "123");
    server->Run();
    return 0;
}
