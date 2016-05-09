//
//  cxRaknetServer.h
//  RaknetServer
//
//  Created by xuhua on 5/4/16.
//  Copyright © 2016 xuhua. All rights reserved.
//

#ifndef RaknetServer_cxRaknetServer_h
#define RaknetServer_cxRaknetServer_h

#include <core/cxHash.h>
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
    uv_mutex_t mutex;
protected:
    char publicKey[cat::EasyHandshake::PUBLIC_KEY_BYTES];
    char privateKey[cat::EasyHandshake::PRIVATE_KEY_BYTES];
    bool initKey();
    cxHash *clients;
public:
    //主锁用于读写全局数据
    void Lock();
    void UnLock();
    //
    void OnPacket(RakNet::Packet *packet);
    void OnMessage(RakNet::RakNetGUID clientId,const cxStr *message);
public:
    virtual void Wait();
    virtual void Stop();
    virtual bool Init(cxInt nt,cxInt port,cxInt max,cchars pass);
    virtual void Run();
public:
    virtual void ThreadLoop();
    virtual void OnNewConnect(RakNet::RakNetGUID clientId);
    virtual void OnLost(RakNet::RakNetGUID clientId);
};

CX_CPP_END

#endif


