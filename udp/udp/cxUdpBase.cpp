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
    uv_mutex_init(&mutex);
    framemax = 512;
    bufsiz = 65536;
    buffer = malloc(bufsiz);
    uv_loop_init(&looper);
    uv_udp_init(&looper, &handle);
    handle.data = this;
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_protocol = IPPROTO_UDP;
    hints.ai_flags = 0;
}

cxUdpBase::~cxUdpBase()
{
    uv_mutex_destroy(&mutex);
    free(buffer);
    uv_loop_close(&looper);
}

void cxUdpBase::Update()
{
    uv_mutex_lock(&mutex);
    uv_run(&looper, UV_RUN_NOWAIT);
    uv_mutex_unlock(&mutex);
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

void cxUdpBase::OnRecv(const struct sockaddr* addr,cxAny data,cxInt size)
{
    CX_LOGGER("UDP RECV DATA %d",size);
}

void cxUdpBase::udp_send_cb(uv_udp_send_t* req, int status)
{
    cxStr *d = static_cast<cxStr *>(req->data);
    d->Release();
    free(req);
}

cxBool cxUdpBase::WriteFrame(struct sockaddr *addr,cxStr *frame)
{
    uv_udp_send_t *req = (uv_udp_send_t *)malloc(sizeof(uv_udp_send_t));
    frame->Retain();
    req->data = frame;
    uv_buf_t buf = uv_buf_init((char *)frame->Buffer(),frame->Size());
    uv_mutex_lock(&mutex);
    cxInt ret = uv_udp_send(req, &handle, &buf, 1, addr, cxUdpBase::udp_send_cb);
    uv_mutex_unlock(&mutex);
    if(ret != 0){
        frame->Release();
    }
    
    return true;
}

void cxUdpBase::udp_udp_recv_cb(uv_udp_t* handle,ssize_t nread,const uv_buf_t *buf,const struct sockaddr* addr,unsigned flags)
{
    cxUdpBase *pudp = static_cast<cxUdpBase *>(handle->data);
    if(nread <= 0){
        CX_ERROR("%ld",nread);
        return;
    }
    pudp->OnRecv(addr, buf->base, (cxInt)nread);
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

cxInt cxUdpBase::Init(cchars host,cxInt port)
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
    return ret;
}

CX_CPP_END

