//
//  cxUdpHost.h
//  udp
//
//  Created by xuhua on 17/03/2017.
//  Copyright © 2017 xuhua. All rights reserved.
//

#ifndef udp_cxUdpHost_h
#define udp_cxUdpHost_h

#include <core/cxHash.h>
#include <core/cxStr.h>
#include <core/cxSync.h>
#include <bitset>

CX_CPP_BEGIN

#define MAX_SEQ         1024*1024

typedef struct sockaddr UdpAddr;

class cxUdpData;
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
    
    cxRWLock rlocker;
    std::bitset<MAX_SEQ> rds;
    cxUInt32 maxseq;
    
    cxRWLock wlocker;
    cxHash *wds;
public:
    cxEvent<cxUdpHost> onActived;
    cxEvent<cxUdpHost> onClosed;
public:
    void Update();
    // ack send data
    void AckSendData(cxUInt32 seq);
    void SaveSendData(cxUInt32 seq,const cxStr *data);
    cxBool SaveRecvData(cxUdpData *data);
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
    void WriteData(const cxStr *data);
    void WriteData(const cxUdpData *data);
};

CX_CPP_END

#endif


