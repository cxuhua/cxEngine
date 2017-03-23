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

DataSegments::Item::Item(cxUInt64 abeg)
{
    beg = abeg;
    len = 1;
}

void DataSegments::Merge()
{
    Items::iterator it = ds.begin();
    Items::iterator pt = it;
    while(it != ds.end() && pt != ds.end()){
        Item *i1 = *pt;
        Item *i2 = *it;
        if(pt == it){
            pt = it++;
        }else if(i1->beg + i1->len == i2->beg){
            //i1 -> i2
            i1->len += i2->len;
            delete i2;
            it = ds.erase(it);
        }else if(i2->beg + i2->len == i1->beg){
            //i2 <- i1
            i1->beg -= i2->len;
            i1->len += i2->len;
            delete i2;
            it = ds.erase(it);
        }else{
            pt = it++;
        }
    }
}

void DataSegments::Put(cxUInt64 v)
{
    Items::iterator it = ds.begin();
    while(it != ds.end()){
        Item *i = *it;
        //middle
        if(v >= i->beg && v < i->beg + i->len){
            Merge();
            return;
        }
        //after
        if(i->beg + i->len == v){
            i->len ++;
            Merge();
            return;
        }
        //before
        if(i->beg - 1 == v){
            i->beg --;
            i->len ++;
            Merge();
            return;
        }
        it ++;
    }
    ds.push_back(new Item(v));
}

void DataSegments::Clear()
{
    Items::iterator it = ds.begin();
    while(it != ds.end()){
        delete *it;
        it++;
    }
    ds.clear();
}

cxBool DataSegments::Has(cxUInt64 v)
{
    for(Items::iterator it=ds.begin();it!=ds.end();it++){
        Item *i = *it;
        if(v >= i->beg && v < i->beg + i->len){
            return true;
        }
    }
    return false;
}

CX_IMPLEMENT(cxUdpHost);

cxUdpHost::cxUdpHost()
{
    group = 0;
    wds = cxHash::Alloc();
    Reset();
    base = nullptr;
    trytime = 300000L;//0.3s
    maxtime = 3000000L;//3s
}

cxUdpHost::~cxUdpHost()
{
    wds->Release();
}

cxUInt64 cxUdpHost::SeqInc()
{
    mutex.Lock();
    seq ++;
    mutex.Unlock();
    return seq;
}

void cxUdpHost::Reset()
{
    rlocker.WLock();
    rds.Clear();
    rlocker.WUnlock();
    
    wlocker.WLock();
    wds->Clear();
    wlocker.WUnlock();
    
    seq = 0;
    isactived = false;
    isclosed = true;
    uptime = 0;
    ping = 0;
}

void cxUdpHost::SetGroup(cxUInt32 v)
{
    group = v;
}

cxBool cxUdpHost::SaveRecvData(cxUdpData *data)
{
    cxBool ret = false;
    cxUInt64 dseq = data->Seq();
    rlocker.WLock();
    ret = rds.Has(dseq);
    if(!ret){
        rds.Put(dseq);
    }
    rlocker.WUnlock();
    return ret;
}

void cxUdpHost::AckSendData(cxUInt64 seq)
{
    wlocker.WLock();
    wds->Del(seq);
    wlocker.WUnlock();
}

void cxUdpHost::SaveSendData(cxUInt64 seq,const cxStr *data)
{
    cxUdpData *d = cxUdpData::Alloc();
    if(d->Init(seq, data, base->Now())){
        wlocker.WLock();
        wds->Set(seq, d);
        wlocker.WUnlock();
    }
    d->Release();
}

void cxUdpHost::SetTryTime(cxInt v)
{
    trytime = v;
}

cxUInt32 cxUdpHost::Group()
{
    return group;
}

void cxUdpHost::Update()
{
    CX_ASSERT(base != nullptr, "base nullptr");
    if(!IsActived()){
        return;
    }
    cxUInt64 now = base->Now();
    wlocker.WLock();
    cxHash::Iter it = wds->Begin();
    while(it != wds->End()){
        cxUdpData *data = it->second->To<cxUdpData>();
        cxInt v = (cxInt)(now - data->Time());
        if(v >= maxtime){
            onMiss.Fire(this, data);
            base->onMiss.Fire(base, this, data);
            it = wds->Remove(it);
            continue;
        }
        if(v < trytime){
            it++;
            continue;
        }
        if(data->DecMaxTry() == 0){
            onMiss.Fire(this, data);
            base->onMiss.Fire(base, this, data);
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
    base->WriteData(Addr(), data->Seq(), data->Data());
}

void  cxUdpHost::WriteData(const cxStr *data)
{
    CX_ASSERT(base != nullptr, "base not set");
    if(!IsActived()){
        CX_ERROR("Write data error,upd not actived");
        return;
    }
    cxUInt64 seq = SeqInc();
    SaveSendData(seq, data);
    base->WriteData(Addr(), seq, data);
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

void cxUdpHost::UpdateGroup(cxUInt64 tv,cxUInt32 gv)
{
    uptime = tv;
    group = gv;
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

