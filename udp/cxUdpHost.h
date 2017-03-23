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
#include <vector>

CX_CPP_BEGIN

class DataSegments
{
private:
    struct Item {
        cxUInt32 beg;
        cxUInt32 len;
        Item(cxUInt32 abeg);
    };
    typedef std::vector<Item *> Items;
    Items ds;
public:
    void Merge();
    void Put(cxUInt32 v);
    cxBool Has(cxUInt32 v);
    void Clear();
};

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
    DataSegments rds;
    
    cxRWLock wlocker;
    cxHash *wds;
    cxInt trytime;
    cxInt maxtime;
public:
    void SetTryTime(cxInt v);
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


