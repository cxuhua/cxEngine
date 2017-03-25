//
//  cxUdpServer.h
//  udp
//
//  Created by xuhua on 16/03/2017.
//  Copyright © 2017 xuhua. All rights reserved.
//

#ifndef udp_cxUdpServer_h
#define udp_cxUdpServer_h

#include "cxUdpBase.h"

CX_CPP_BEGIN

class UdpFrame : public cxObject
{
public:
    CX_DECLARE(UdpFrame);
protected:
    explicit UdpFrame();
    virtual ~UdpFrame();
private:
    cxStr *buffer;
    UdpAddr addr;
public:
    cxStr *Buffer();
    const UdpAddr *Addr();
public:
    static UdpFrame *Alloc(UdpAddr *addr,cxAny data,cxInt size);
};

class cxUdpServer : public cxUdpBase
{
public:
    CX_DECLARE(cxUdpServer);
protected:
    explicit cxUdpServer(cxInt anum=10);
    virtual ~cxUdpServer();
protected:
    void OnRecvData(cxUdpHost *h,const cxUdpData *d);
    void OnRecvFrame(UdpAddr *addr,cxAny data,cxInt size);
private:
    cxBool isexit;
    cxMutex dMutex;
    cxCond  dCond;
    cxList *rQueue;
    
    cxBarrier bar;
    
    cxInt num;
    static void workRunFunc(void *arg);
    uv_thread_t *pid;
    
    static void workUpdateFunc(void *arg);
    uv_thread_t mpid;
public:
    void Stop();
    void WorkRun();
    void Start();
};

CX_CPP_END

#endif


