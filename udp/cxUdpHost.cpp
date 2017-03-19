//
//  cxUdpHost.cpp
//  udp
//
//  Created by xuhua on 17/03/2017.
//  Copyright © 2017 xuhua. All rights reserved.
//


#include "cxUdpBase.h"
#include "cxUdpHost.h"
#include "cxUdpData.h"

CX_CPP_BEGIN

CX_IMPLEMENT(cxUdpHost);

cxUdpHost::cxUdpHost()
{
    rds.reset();
    wds = cxHash::Alloc();
    Reset();
    base = nullptr;
}

cxUdpHost::~cxUdpHost()
{
    wds->Release();
}

cxUInt32 cxUdpHost::SeqInc()
{
    mutex.Lock();
    seq ++;
    CX_ASSERT(seq < MAX_SEQ, "seq too big");
    mutex.Unlock();
    return seq;
}

void cxUdpHost::Reset()
{
    rds.reset();
    wds->Clear();
    seq = 0;
    isactived = false;
    isclosed = true;
    uptime = 0;
    ping = 0;
    maxseq = 0;
}

cxBool cxUdpHost::SaveRecvData(cxUdpData *data)
{
    cxBool ret = false;
    cxUInt32 dseq = data->Seq();
    rlocker.WLock();
    ret = rds[dseq];
    if(!ret){
        rds[dseq] = true;
    }
    if(dseq > maxseq){
        maxseq = dseq;
    }
    rlocker.WUnlock();
    return ret;
}

void cxUdpHost::AckSendData(cxUInt32 seq)
{
    wlocker.WLock();
    wds->Del(seq);
    wlocker.WUnlock();
}

void cxUdpHost::SaveSendData(cxUInt32 seq,const cxStr *data)
{
    wlocker.WLock();
    cxUdpData *d = cxUdpData::Alloc();
    if(d->Init(seq, data, uid, base->Now())){
        wds->Set(seq, d);
    }
    d->Release();
    wlocker.WUnlock();
}

void cxUdpHost::Update()
{
    CX_ASSERT(base != nullptr, "base nullptr");
    if(!IsActived()){
        return;
    }
    cxUInt64 now = base->Now();
    // > 2*ping repeat send
    wlocker.WLock();
    cxHash::Iter it = wds->Begin();
    while(it != wds->End()){
        cxUdpData *data = it->second->To<cxUdpData>();
        cxInt v = (cxInt)(now - data->Time());
        if(v >= 1000000){
            it = wds->Remove(it);
            continue;
        }
        if(v < ping * 2){
            it++;
            continue;
        }
        if(data->DecMaxTry() == 0){
            it = wds->Remove(it);
            continue;
        }
        WriteData(data);
        data->SetTime(now);
        it++;
    }
    wlocker.WUnlock();
}

void cxUdpHost::WriteData(const cxUdpData *data)
{
    CX_ASSERT(base != nullptr, "base not set");
    if(!IsActived()){
        CX_ERROR("Write data error,upd not actived");
        return;
    }
    base->WriteData(Addr(), data->Seq(), data->Dst(), data->Data());
}

void  cxUdpHost::WriteData(const cxStr *data)
{
    CX_ASSERT(base != nullptr, "base not set");
    if(!IsActived()){
        CX_ERROR("Write data error,upd not actived");
        return;
    }
    cxUInt32 seq = SeqInc();
    base->WriteData(Addr(), seq, uid, data);
    SaveSendData(seq, data);
}

cxBool cxUdpHost::IsClosed()
{
    mutex.Lock();
    cxBool ret = isclosed;
    mutex.Unlock();
    return ret;
}

cxBool cxUdpHost::IsActived()
{
    mutex.Lock();
    cxBool ret = isactived;
    mutex.Unlock();
    return ret;
}

cxBool cxUdpHost::CheckClosed(cxUInt64 time)
{
    cxBool ret = false;
    mutex.Lock();
    if(uptime > 0 && !isclosed && time - uptime > 3000000L){
        isclosed = true;
        isactived = false;
        uptime = 0;
        ret = true;
    }
    mutex.Unlock();
    return ret;
}

cxBool cxUdpHost::CheckActived(cxUInt64 time)
{
    cxBool ret = false;
    mutex.Lock();
    if(!isactived){
        isactived = true;
        isclosed = false;
        ret = true;
    }
    mutex.Unlock();
    return ret;
}

void cxUdpHost::UpdatePing(cxUInt64 time,cxUInt64 value)
{
    uptime = time;
    ping =  (cxInt)value;
}

void cxUdpHost::UpdateTime(cxUInt64 time)
{
    uptime = time;
}

UdpAddr *cxUdpHost::Addr()
{
    return &addr;
}

cxUInt64 cxUdpHost::UID()
{
    return uid;
}

cxBool cxUdpHost::Init(cxUdpBase *pb,cchars ip,cxInt port,cxUInt64 id)
{
    struct sockaddr_in in;
    cxInt ret = uv_ip4_addr(ip, port, &in);
    CX_ASSERT(ret == 0, "iv ip4 addr error:%s",uv_err_name(ret));
    if(ret != 0){
        return false;
    }
    return Init(pb, (struct sockaddr *)&in, id);
}

cxBool cxUdpHost::Init(cxUdpBase *pb,const UdpAddr *paddr,cxUInt64 id)
{
    uid = id;
    base = pb;
    addr = *paddr;
    return true;
}

CX_CPP_END

