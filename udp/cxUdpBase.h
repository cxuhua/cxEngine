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
#include "cxUdpHost.h"

CX_CPP_BEGIN

#pragma pack(1)
//opt == 1
struct udp_ping_t {
    cxUInt8 opt;
    cxUInt64 src;
    cxUInt64 time;
    cxUInt64 ptime;
};
//opt == 2
struct udp_pong_t {
    cxUInt8 opt;
    cxUInt64 src;
    cxUInt64 time;
    cxUInt64 ptime;
};
//opt == 3
struct udp_data_t {
    cxUInt8     opt;
    cxUInt32    seq;
    cxUInt64    src;
    cxUInt64    time;
    cxUInt64    dst;
    cxUInt16    size;
    cxInt8      data[0];
};
#pragma pack()

class cxUdpData;
class cxUdpBase : public cxObject
{
public:
    CX_DECLARE(cxUdpBase);
protected:
    explicit cxUdpBase();
    virtual ~cxUdpBase();
private:
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
private:
    uv_timer_t timer;
    static void udp_timer_cb(uv_timer_t* handle);
    void sendPing();
protected:
    virtual void RecvFrame(UdpAddr *addr,cxAny data,cxInt size);
    virtual void RecvData(cxUdpHost *h,const cxUdpData *d);
    void DecodeData(const UdpAddr *addr,cxAny data,cxInt size);
    void DecodeData(const UdpAddr *addr,const cxStr *data);
    cxStr *EncodeData(const cxStr *data);
    cxUInt64 Now();
    virtual void OnHostActived(cxUdpHost *h);
    virtual void OnHostClosed(cxUdpHost *h);
public:
    cxUInt64 UID();
    cxUdpHost *FindHost(cxUInt64 id,const UdpAddr *addr);
    cxUdpHost *FindHost(cxUInt64 id);
    cxUdpHost *AppendHost(cchars ip,cxInt port,cxUInt64 id);
    cxUdpHost *AppendHost(cxUInt64 id,const UdpAddr *addr);
    virtual void WorkData();
    void Update();
    cxInt Init(cchars host,cxInt port,cxUInt64 uid);
    cxInt Start();
    cxInt WriteFrame(const UdpAddr *addr,const cxStr *frame);
    cxInt WriteData(const UdpAddr *addr,cxUInt32 seq, cxUInt64 dst,const cxStr *data);
};

CX_CPP_END

#endif


