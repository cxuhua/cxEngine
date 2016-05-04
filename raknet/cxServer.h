//
//  cxRaknetServer.h
//  RaknetServer
//
//  Created by xuhua on 5/4/16.
//  Copyright © 2016 xuhua. All rights reserved.
//

#ifndef RaknetServer_cxRaknetServer_h
#define RaknetServer_cxRaknetServer_h

#include "cxRaknet.h"

CX_CPP_BEGIN

class cxServer : public cxRaknet
{
public:
    CX_DECLARE(cxServer);
protected:
    explicit cxServer();
    virtual ~cxServer();
private:
    cxInt thread;
    static void runEntry(void *a);
    cxBool exitFlags;
    uv_thread_t *threads;
public:
    void Stop();
    virtual void Init(cxInt nt,cxInt port,cxInt max,cchars pass);
    void Run();
    void OnPacket(RakNet::Packet *packet,void *data);
    void OnMessage(RakNet::RakNetGUID clientId,const cxStr *message);
public:
    virtual void OnNewConnect(RakNet::RakNetGUID clientId);
    virtual void OnLost(RakNet::RakNetGUID clientId);
};

CX_CPP_END

#endif


