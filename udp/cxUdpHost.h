//
//  cxUdpHost.h
//  udp
//
//  Created by xuhua on 17/03/2017.
//  Copyright © 2017 xuhua. All rights reserved.
//

#ifndef udp_cxUdpHost_h
#define udp_cxUdpHost_h

#include <core/cxStr.h>
#include <core/cxSync.h>

CX_CPP_BEGIN

typedef struct sockaddr UdpAddr;

class cxUdpBase;
class cxUdpHost : public cxObject
{
public:
    CX_DECLARE(cxUdpHost);
protected:
    explicit cxUdpHost();
    virtual ~cxUdpHost();
private:
    cxMutex mutex;
    cxUInt32 seq;
    cxBool isactived;
    cxBool isclosed;
    cxUdpBase *base;
    cxUInt64 uid;
    UdpAddr addr;
    cxInt ping;
    cxUInt64 uptime;
public:
    cxUInt32 SeqInc();
    cxBool IsClosed();
    cxBool IsActived();
    cxBool CheckClosed(cxUInt64 time);
    cxBool CheckActived(cxUInt64 time);
    void Reset();
    void UpdatePing(cxUInt64 time,cxUInt64 value);
    void UpdateTime(cxUInt64 time);
    cxUInt64 UID();
    UdpAddr *Addr();
    cxBool Init(cxUdpBase *pb,cchars ip,cxInt port,cxUInt64 id);
    cxBool Init(cxUdpBase *pb,const UdpAddr *paddr,cxUInt64 id);
    cxInt WriteData(const cxStr *data);
};

CX_CPP_END

#endif


