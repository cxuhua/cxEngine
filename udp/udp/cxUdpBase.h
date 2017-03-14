//
//  cxUdpBase.h
//  udp
//
//  Created by xuhua on 14/03/2017.
//  Copyright © 2017 xuhua. All rights reserved.
//

#ifndef udp_cxUdpBase_h
#define udp_cxUdpBase_h

#include "cxList.h"
#include "cxStr.h"
#include "cxSync.h"

CX_CPP_BEGIN

class cxUdpBase : public cxObject
{
public:
    CX_DECLARE(cxUdpBase);
protected:
    explicit cxUdpBase();
    virtual ~cxUdpBase();
private:
    
    cxStr *datakey;
    
    cxRWLock  rLock;
    cxList *rQueue;
    
    cxUInt64 uid;
    uint64_t recvnum;
    cxMutex mutex;
    cxInt framemax;
    uv_loop_t looper;
    uv_udp_t handle;
    struct sockaddr_in sin;
    cxAny buffer;
    cxInt bufsiz;
    static void udp_alloc_cb(uv_handle_t* handle,size_t suggested,uv_buf_t* buf);
    static void udp_udp_recv_cb(uv_udp_t* handle,ssize_t nread,const uv_buf_t* buf,const struct sockaddr* addr,unsigned flags);
    static void udp_send_cb(uv_udp_send_t* req, int status);
public:
    virtual void OnRecv();
public:
    void Update();
    cxInt Init(cchars host,cxInt port,cxUInt64 uid);
    cxInt Start();
    cxInt WriteFrame(struct sockaddr *addr,const cxStr *frame);
};

CX_CPP_END

#endif


