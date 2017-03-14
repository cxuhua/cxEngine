//
//  cxUdpBase.cpp
//  udp
//
//  Created by xuhua on 14/03/2017.
//  Copyright © 2017 xuhua. All rights reserved.
//


#include "cxUdpBase.h"

CX_CPP_BEGIN

CX_IMPLEMENT(cxUdpBase);

cxUdpBase::cxUdpBase()
{
    datakey = cxStr::Alloc("#&&43476_)FV121");
    rQueue = cxList::Alloc();
    uid = 0;
    recvnum = 0;
    framemax = 512;
    bufsiz = 65536;
    buffer = malloc(bufsiz);
    uv_loop_init(&looper);
    uv_udp_init(&looper, &handle);
    handle.data = this;
}

cxUdpBase::~cxUdpBase()
{
    datakey->Release();
    rQueue->Release();
    free(buffer);
    uv_loop_close(&looper);
}

void cxUdpBase::Update()
{
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

void cxUdpBase::OnRecv()
{
    CX_LOGGER("UDP RECV DATA %llu",recvnum);
}

void cxUdpBase::udp_send_cb(uv_udp_send_t* req, int status)
{
    cxStr *data = static_cast<cxStr *>(req->data);
    data->Release();
    delete req;
}

cxInt cxUdpBase::WriteFrame(struct sockaddr *addr,const cxStr *frame)
{
    CX_ASSERT(frame->Size() > 0, "frame error");
    cxInt ret = 0;
    cxStr *data = (cxStr *)frame->TeaEncode(datakey);
    data->Retain();
    uv_udp_send_t *req = new uv_udp_send_t();
    req->data = data;
    uv_buf_t buf = uv_buf_init(data->Buffer(), data->Size());
    mutex.Lock();
    ret = uv_udp_send(req, &handle, &buf, 1, addr, cxUdpBase::udp_send_cb);
    mutex.Unlock();
    if(ret != 0){
        data->Release();
        delete req;
    }
    return ret;
}

void cxUdpBase::udp_udp_recv_cb(uv_udp_t* handle,ssize_t nread,const uv_buf_t *buf,const struct sockaddr* addr,unsigned flags)
{
    cxUdpBase *pudp = static_cast<cxUdpBase *>(handle->data);
    if(nread <= 0){
        return;
    }
    pudp->recvnum++;
    
    pudp->rLock.WLock();
    cxStr *data = cxStr::Alloc(buf->base, (cxInt)nread);
    pudp->rQueue->Append(data);
    data->Release();
    pudp->rLock.WUnlock();

    pudp->OnRecv();
}

cxInt cxUdpBase::Start()
{
    cxInt ret = uv_udp_recv_start(&handle, cxUdpBase::udp_alloc_cb, cxUdpBase::udp_udp_recv_cb);
    if(ret != 0){
        CX_ERROR("uv_udp_recv_start error:%d",ret);
        return ret;
    }
    return ret;
}

cxInt cxUdpBase::Init(cchars host,cxInt port,cxUInt64 id)
{
    cxInt ret = uv_ip4_addr(host, port, &sin);
    if(ret != 0){
        CX_ERROR("UDP ip4 addr error:%d",ret);
        return ret;
    }
    ret = uv_udp_bind(&handle, (struct sockaddr *)&sin, 0);
    if(ret !=0){
        CX_ERROR("UDP bind error:%d",ret);
        return ret;
    }
    uid = id;
    return ret;
}

CX_CPP_END

