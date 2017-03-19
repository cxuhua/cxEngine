//
//  cxUdpData.h
//  udp
//
//  Created by xuhua on 18/03/2017.
//  Copyright © 2017 xuhua. All rights reserved.
//

#ifndef udp_cxUdpData_h
#define udp_cxUdpData_h

#include <core/cxStr.h>
#include "cxUdpBase.h"

CX_CPP_BEGIN

#define MAX_TRY_SEND    5

enum udp_opt_t {
    UDP_OPT_NONE,
    UDP_OPT_PING,
    UDP_OPT_PONG,
    UDP_OPT_DATA,
    UDP_OPT_ACKD,
};

#pragma pack(1)
//opt == UDP_OPT_PING
struct udp_ping_t {
    cxUInt8 opt;
    cxUInt64 src;
    cxUInt64 time;
    cxUInt64 ptime;
};
//opt == UDP_OPT_PONG
struct udp_pong_t {
    cxUInt8 opt;
    cxUInt64 src;
    cxUInt64 time;
    cxUInt64 ptime;
};
//opt == UDP_OPT_DATA
struct udp_data_t {
    cxUInt8     opt;
    cxUInt32    seq;
    cxUInt64    src;
    cxUInt64    time;
    cxUInt64    dst;
    cxUInt16    size;
    cxInt8      data[0];
};
//opt == UDP_OPT_ACKD
struct udp_ack_t {
    cxUInt8     opt;
    cxUInt64    uid;
    cxUInt64    time;
    cxUInt32    seq;
};
#pragma pack()

class cxUdpData : public cxObject
{
public:
    CX_DECLARE(cxUdpData);
protected:
    explicit cxUdpData();
    virtual ~cxUdpData();
private:
    cxStr *buffer;
private:
    UdpAddr     addr;
    cxUInt8     maxtry;
    cxUInt32    seq;
    cxUInt64    src;
    cxUInt64    time;
    cxUInt64    dst;
public:
    // init for recv data
    cxBool Init(const udp_data_t *data);
    // init for send data
    cxBool Init(cxUInt32 aseq, const cxStr *data,cxUInt64 adst,cxUInt64 atime);
    // init raw data for send
    cxBool Init(const UdpAddr *paddr,const cxStr *data);
public:
    const UdpAddr *Addr() const;
    cxUInt8 DecMaxTry();
    cxUInt64 Time();
    void SetTime(cxUInt64 v);
    cxUInt32 Seq() const;
    cxUInt64 Src() const;
    cxUInt64 Dst() const;
    cxStr *Data() const;
};

CX_CPP_END

#endif


