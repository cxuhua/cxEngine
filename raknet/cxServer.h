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
    uv_loop_t loop;
    uv_timer_t timer;
    uv_mutex_t mutex;
    char publicKey[cat::EasyHandshake::PUBLIC_KEY_BYTES];
    char privateKey[cat::EasyHandshake::PRIVATE_KEY_BYTES];
    bool initKey();
public:
    uv_loop_t *Looper();
    void Stop();
    virtual bool Init(cxInt nt,cxInt port,cxInt max,cchars pass);
    void Run();
    void OnPacket(RakNet::Packet *packet,void *data);
    void OnMessage(RakNet::RakNetGUID clientId,const cxStr *message,void *data);
public:
    virtual void Loop(void *data);
    virtual void OnNewConnect(RakNet::RakNetGUID clientId,void *data);
    virtual void OnLost(RakNet::RakNetGUID clientId,void *data);
};

CX_CPP_END

#endif


