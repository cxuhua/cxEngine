//
//  cxUdpBase.h
//  udp
//
//  Created by xuhua on 14/03/2017.
//  Copyright © 2017 xuhua. All rights reserved.
//

#ifndef udp_cxUdpBase_h
#define udp_cxUdpBase_h

#include <core/cxList.h>
#include <core/cxStr.h>
#include <core/cxSync.h>
#include <core/cxHash.h>
#include "cxUdpData.h"

CX_CPP_BEGIN

class cxUdpData;
class cxUdpBase : public cxObject
{
public:
    CX_DECLARE(cxUdpBase);
protected:
    explicit cxUdpBase();
    virtual ~cxUdpBase();
private:
    cxRWLock wlocker;
    cxArray *wqueue;
    cxHash *hosts;
    cxRWLock hlocker;
    cxStr *datakey;
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
    void hostRecvData(cxUdpHost *h,cxUdpData *d);
    void recvData(const UdpAddr *addr,const udp_data_t *data,cxInt size);
private:
    uv_timer_t timer;
    static void udp_timer_cb(uv_timer_t* handle);
    void sendPing();
protected:
    void DecodeData(const UdpAddr *addr,cxAny data,cxInt size);
    void DecodeData(const UdpAddr *addr,const cxStr *data);
    cxStr *EncodeData(const cxStr *data);
protected:
    virtual void OnRecvFrame(UdpAddr *addr,cxAny data,cxInt size);
    virtual void OnRecvData(cxUdpHost *h,const cxUdpData *d);
    virtual void OnHostActived(cxUdpHost *h);
    virtual void OnHostClosed(cxUdpHost *h);
public:
    cxEvent<cxUdpBase, cxUdpHost *, const cxUdpData *> onData;
    cxEvent<cxUdpBase, cxUdpHost *, const cxUdpData *> onMiss;
    cxEvent<cxUdpBase, cxUdpHost *> onActived;
    cxEvent<cxUdpBase, cxUdpHost *> onClosed;
public:
    cxUInt64 Now();
    cxUInt64 UID();
    cxUdpHost *FindHost(cxUInt64 uid,const UdpAddr *addr);
    cxUdpHost *FindHost(cxUInt64 uid);
    cxUdpHost *ConnectHost(cchars ip,cxInt port,cxUInt64 uid);
    cxUdpHost *ConnectHost(cxUInt64 uid,const UdpAddr *addr);
    virtual void WorkRun();
    void Update();
    cxInt Init(cchars host,cxInt port,cxUInt64 uid);
    cxInt Start();
    void WriteFrame(const UdpAddr *addr,const cxStr *frame);
    cxInt WriteFrame(const cxUdpData *data);
    void WriteData(const UdpAddr *addr,cxUInt32 seq, cxUInt64 dst,const cxStr *data);
    void WriteData(const UdpAddr *addr,cxUInt32 seq,cxUInt64 src,cxUInt64 dst,const cxStr *data);
};

CX_CPP_END

#endif


