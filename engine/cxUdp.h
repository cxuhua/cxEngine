//
//  cxUdp.h
//  cxEngineCore
//
//  Created by xuhua on 1/12/16.
//  Copyright © 2016 xuhua. All rights reserved.
//

#ifndef cxEngineCore_cxUdp_h
#define cxEngineCore_cxUdp_h

#include <uv/uv.h>
#include <core/cxStr.h>
#include "cxAction.h"

CX_CPP_BEGIN

#pragma pack(1)
#define PACKET_MAX_BYTES    512
struct cxPacketInfo
{
    cxUInt16 len;                       // packet total length
    cxUInt32 seq;                       // 0->n
    cxUInt8  opt;                       // option
    cxByte  data[PACKET_MAX_BYTES];     // max packet bytes
};
#pragma pack()

class cxPacket : public cxObject
{
public:
    CX_DECLARE(cxPacket);
protected:
    explicit cxPacket();
    virtual ~cxPacket();
};

class cxUdp : public cxAction
{
public:
    CX_DECLARE(cxUdp);
protected:
    explicit cxUdp();
    virtual ~cxUdp();
private:
    cxArray *packets;
    cxInt port;
    cxStr *host;
    uv_loop_t looper;
    uv_udp_t handle;
    struct addrinfo hints;
protected:
    void OnStep(cxFloat dt);
public:
    cxBool Init(cchars ahost,cxInt aport);
    static cxUdp *Create(cchars host,cxInt port);
};

CX_CPP_END

#endif


