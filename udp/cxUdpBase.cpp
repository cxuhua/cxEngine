//
//  cxUdpBase.cpp
//  udp
//
//  Created by xuhua on 14/03/2017.
//  Copyright © 2017 xuhua. All rights reserved.
//

#include <core/cxUtil.h>
#include "cxUdpBase.h"
#include "cxUdpData.h"

CX_CPP_BEGIN

CX_IMPLEMENT(cxUdpBase);

cxUdpBase::cxUdpBase()
{
    wqueue = cxArray::Alloc();
    hosts = cxHash::Alloc();
    datakey = cxStr::Alloc("#&&43476_)FV121");
    
    uid = 0;
    recvnum = 0;
    framemax = 512;
    bufsiz = 65536;
    buffer = malloc(bufsiz);
    uv_loop_init(&looper);
    
    uv_udp_init(&looper, &handle);
    handle.data = this;
    
    uv_timer_init(&looper, &timer);
    timer.data = this;
}

cxUdpBase::~cxUdpBase()
{
    wqueue->Release();
    hosts->Release();
    datakey->Release();
    free(buffer);
    uv_loop_close(&looper);
}

cxUdpHost *cxUdpBase::FindHost(cxUInt64 id,const UdpAddr *addr)
{
    hlocker.RLock();
    cxUdpHost *h = hosts->Get(id)->To<cxUdpHost>();
    hlocker.RUnlock();
    if(h != nullptr){
        return h;
    }
    return ConnectHost(id,addr);
}

cxUdpHost *cxUdpBase::FindHost(cxUInt64 id)
{
    hlocker.RLock();
    cxUdpHost *h = hosts->Get(id)->To<cxUdpHost>();
    hlocker.RUnlock();
    return h;
}

cxUdpHost *cxUdpBase::ConnectHost(cxUInt64 id,const UdpAddr *addr)
{
    cxUdpHost *host = cxUdpHost::Create();
    if(!host->Init(this, addr, id)){
        return  nullptr;
    }
    hlocker.WLock();
    hosts->Set(id, host);
    hlocker.WUnlock();
    return host;
}

cxUdpHost *cxUdpBase::ConnectHost(cchars ip,cxInt port,cxUInt64 id)
{
    cxUdpHost *host = cxUdpHost::Create();
    if(!host->Init(this, ip, port, id)){
        return  nullptr;
    }
    hlocker.WLock();
    hosts->Set(id, host);
    hlocker.WUnlock();
    return host;
}

void cxUdpBase::Update()
{
    hlocker.RLock();
    for(cxHash::Iter it=hosts->Begin();it!=hosts->End();it++){
        cxUdpHost *h = it->second->To<cxUdpHost>();
        h->Update();
    }
    hlocker.RUnlock();
    
    wlocker.WLock();
    cxArray::FIter it = wqueue->FBegin();
    while(it != wqueue->FEnd()){
        cxUdpData *data = (*it)->To<cxUdpData>();
        if(WriteFrame(data) == 0){
            it = wqueue->Remove(it);
        }else{
            it++;
        }
    }
    wlocker.WUnlock();
    
    mutex.Lock();
    uv_run(&looper, UV_RUN_NOWAIT);
    mutex.Unlock();
}

void cxUdpBase::udp_alloc_cb(uv_handle_t* handle,size_t suggested,uv_buf_t *buf)
{
    cxUdpBase *pudp = static_cast<cxUdpBase *>(handle->data);
    if(suggested > pudp->bufsiz){
        pudp->buffer = (char *)realloc(pudp->buffer,suggested);
        pudp->bufsiz = (cxInt)suggested;
    }
    buf->base = (char *)pudp->buffer;
    buf->len = pudp->bufsiz;
}

void cxUdpBase::udp_send_cb(uv_udp_send_t* req, int status)
{
    cxStr *data = static_cast<cxStr *>(req->data);
    data->Release();
    delete req;
}

void cxUdpBase::WriteData(const UdpAddr *addr,cxUInt32 seq,cxUInt64 dst,const cxStr *data)
{
    udp_data_t dptr;
    dptr.opt = UDP_OPT_DATA;
    dptr.seq = seq;
    dptr.src = uid;
    dptr.dst = dst;
    cxStr *d = cxStr::Alloc()->Init(&dptr, sizeof(udp_data_t));
    d->Append(data);
    WriteFrame(addr, d);
    d->Release();
}

cxInt cxUdpBase::WriteFrame(const cxUdpData *ud)
{
    cxStr *data = ud->Data();
    data->Retain();
    uv_udp_send_t *req = new uv_udp_send_t();
    req->data = data;
    uv_buf_t buf = uv_buf_init(data->Buffer(), data->Size());
    mutex.Lock();
    cxInt ret = uv_udp_send(req, &handle, &buf, 1, ud->Addr(), cxUdpBase::udp_send_cb);
    mutex.Unlock();
    if(ret != 0){
        CX_LOGGER("write frame error:%s",uv_err_name(ret));
        data->Release();
        delete req;
    }
    return ret;
}

void cxUdpBase::WriteFrame(const UdpAddr *addr,const cxStr *frame)
{
    CX_ASSERT(frame->Size() > 0, "frame error");
    cxStr *data = EncodeData(frame);
    cxUdpData *d = cxUdpData::Alloc();
    if(d->Init(addr, data)){
        wlocker.WLock();
        wqueue->Append(d);
        wlocker.WUnlock();
    }
    d->Release();
}

void cxUdpBase::WorkRun()
{
    CX_ASSERT(false, "not imp");
}

cxStr *cxUdpBase::EncodeData(const cxStr *data)
{
    return data->TeaEncode(datakey);
}

void cxUdpBase::DecodeData(const UdpAddr *addr,cxAny data,cxInt size)
{
    DecodeData(addr,cxStr::Create(data, size));
}

void cxUdpBase::OnHostActived(cxUdpHost *h)
{
    h->onActived.Fire(h);
}

void cxUdpBase::OnHostClosed(cxUdpHost *h)
{
    h->onClosed.Fire(h);
}

void cxUdpBase::sendPing()
{
    cxUInt64 now = Now();
    hlocker.RLock();
    for(cxHash::Iter it=hosts->Begin();it!=hosts->End();it++){
        cxUdpHost *h = it->second->To<cxUdpHost>();
        if(h->CheckClosed(now)){
            OnHostClosed(h);
        }
        udp_ping_t p;
        p.opt = UDP_OPT_PING;
        p.src = uid;
        p.time = now;
        p.ptime = p.time;
        cxStr *d = cxStr::Alloc(&p, sizeof(udp_ping_t));
        WriteFrame(h->Addr(),d);
        d->Release();
    }
    hlocker.RUnlock();
}

void cxUdpBase::DecodeData(const UdpAddr *addr,const cxStr *data)
{
    cxStr *d = data->TeaDecode(datakey);
    if(!cxStr::IsOK(d)){
        return;
    }
    cxUInt64 now = Now();
    char opt = d->At(0);
    switch (opt) {
        case UDP_OPT_PING:{
            udp_ping_t *p = (udp_ping_t *)d->Buffer();
            cxUdpHost *h = FindHost(p->src,addr);
            udp_pong_t pong;
            pong.opt = UDP_OPT_PONG;
            pong.src = uid;
            pong.time = Now();
            pong.ptime = p->ptime;
            cxStr *d = cxStr::Alloc(&pong, sizeof(udp_ping_t));
            WriteFrame(addr,d);
            d->Release();
            h->UpdateTime(now);
            break;
        }
        case UDP_OPT_PONG:{
            udp_pong_t *p = (udp_pong_t *)d->Buffer();
            cxUInt64 ping = now - p->ptime;
            cxUdpHost *h = FindHost(p->src,addr);
            h->UpdatePing(now,ping);
            if(h->CheckActived(now)){
                OnHostActived(h);
            }
            break;
        }
        case UDP_OPT_DATA:{
            udp_data_t *p = (udp_data_t *)d->Buffer();
            recvData(addr, p, data->Size());
            break;
        }
        case UDP_OPT_ACKD:{
            udp_ack_t *p = (udp_ack_t *)d->Buffer();
            cxUdpHost *h = FindHost(p->uid,addr);
            if(h == nullptr){
                break;
            }
            h->AckSendData(p->seq);
            h->UpdateTime(now);
            break;
        }
        default:{
            CX_ASSERT(false, "opt error");
            break;
        }
    }
}

cxUInt64 cxUdpBase::UID()
{
    return uid;
}

void cxUdpBase::RecvFrame(UdpAddr *addr,cxAny data,cxInt size)
{
    
}

void cxUdpBase::RecvData(cxUdpHost *h,const cxUdpData *d)
{
    udp_ack_t ack;
    ack.opt = UDP_OPT_ACKD;
    ack.uid = uid;
    ack.seq = d->Seq();
    cxStr *data = cxStr::Alloc(&ack, sizeof(udp_ack_t));
    WriteFrame(h->Addr(), data);
    data->Release();
}

void cxUdpBase::udp_udp_recv_cb(uv_udp_t* handle,ssize_t nread,const uv_buf_t *buf,const struct sockaddr* addr,unsigned flags)
{
    cxUdpBase *pudp = static_cast<cxUdpBase *>(handle->data);
    if(nread <= 0){
        return;
    }
    pudp->recvnum++;
    pudp->RecvFrame((UdpAddr *)addr, buf->base, (cxInt)nread);
}

void cxUdpBase::recvData(const UdpAddr *addr,const udp_data_t *data,cxInt size)
{
    cxUdpHost *h = FindHost(data->src,addr);
    if(h == nullptr){
        return;
    }
    cxUdpData *ud = cxUdpData::Alloc();
    if(ud->Init(data,size) && !h->SaveRecvData(ud)){
        RecvData(h,ud);
    }
    ud->Release();
    h->UpdateTime(Now());
}

cxUInt64 cxUdpBase::Now()
{
    return cxUtil::Now();
}

void cxUdpBase::udp_timer_cb(uv_timer_t* handle)
{
    cxUdpBase *pudp = static_cast<cxUdpBase *>(handle->data);
    pudp->sendPing();
}

cxInt cxUdpBase::Start()
{
    cxInt ret = uv_udp_recv_start(&handle, cxUdpBase::udp_alloc_cb, cxUdpBase::udp_udp_recv_cb);
    if(ret != 0){
        CX_ERROR("uv_udp_recv_start error:%s",uv_err_name(ret));
        return ret;
    }
    ret = uv_timer_start(&timer, cxUdpBase::udp_timer_cb, 1000, 1000);
    if(ret != 0){
        CX_ERROR("udp timer start error:%s",uv_err_name(ret));
        return ret;
    }
    return ret;
}

cxInt cxUdpBase::Init(cchars host,cxInt port,cxUInt64 id)
{
    cxInt ret = uv_ip4_addr(host, port, &sin);
    if(ret != 0){
        CX_ERROR("UDP ip4 addr error:%s",uv_err_name(ret));
        return ret;
    }
    ret = uv_udp_bind(&handle, (struct sockaddr *)&sin, 0);
    if(ret != 0){
        CX_ERROR("UDP bind error:%s",uv_err_name(ret));
        return ret;
    }
    uid = id;
    return ret;
}

CX_CPP_END

