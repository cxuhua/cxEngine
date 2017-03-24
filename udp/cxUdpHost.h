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
        cxUInt64 beg;
        cxUInt64 len;
        Item(cxUInt64 abeg);
    };
    typedef std::vector<Item *> Items;
    Items ds;
public:
    void Merge();
    void Put(cxUInt64 v);
    cxBool Has(cxUInt64 v);
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
    cxUInt64 seq;
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
    cxUInt32 group;
    cxHash::Iter isMissRemove(cxUdpData *data,cxHash::Iter it);
public:
    cxEvent<cxUdpHost, const cxUdpData *> onData;
    cxEvent<cxUdpHost, const cxUdpData *> onMiss;
public:
    cxUInt32 Group();
    void SetTryTime(cxInt v);
    void Update();
    // ack send data
    void AckSendData(cxUInt64 seq);
    void SaveSendData(cxUInt64 seq,const cxStr *data);
    cxBool SaveRecvData(cxUdpData *data);
    cxUInt64 SeqInc();
    cxBool IsClosed();
    cxBool IsActived();
    cxBool CheckClosed(cxUInt64 time);
    cxBool CheckActived(cxUInt64 time);
    void Reset();
    void SetGroup(cxUInt32 v);
    void UpdatePing(cxUInt64 time,cxUInt64 value);
    void UpdateTime(cxUInt64 time);
    void UpdateGroup(cxUInt64 tv,cxUInt32 gv);
    cxUInt64 UID();
    UdpAddr *Addr();
    cxBool Init(cxUdpBase *pb,cchars ip,cxInt port,cxUInt64 id);
    cxBool Init(cxUdpBase *pb,const UdpAddr *paddr,cxUInt64 id);
    void WriteData(const cxStr *data);
    void WriteData(const cxUdpData *data);
};

CX_CPP_END

#endif


