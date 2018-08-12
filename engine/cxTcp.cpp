//
//  cxTcp.cpp
//  cxEngineCore
//
//  Created by xuhua on 7/3/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//


#include "cxEngine.h"
#include "cxTcp.h"

CX_CPP_BEGIN

CX_IMPLEMENT(cxTcp);

cxTcp::cxTcp()
{
    memset(&resolver,0, sizeof(uv_getaddrinfo_t));
    memset(&hints,0, sizeof(struct addrinfo));
    error = 0;
    uv_loop_init(&looper);
    connected = false;
    uv_tcp_init(&looper, &handle);
    handle.data = this;
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = 0;
    bufsiz = CX_TCP_BUFFER_SIZE;
    buffer = (char *)malloc(bufsiz);
    Forever();
}

cxTcp::~cxTcp()
{
    uv_loop_close(&looper);
    free(buffer);
}

cxInt cxTcp::Error()
{
    return error;
}

cxTcp *cxTcp::Create(cchars host,cxInt port)
{
    cxTcp *rv = cxTcp::Create();
    rv->Connect(host, port);
    return rv;
}

void cxTcp::OnStep(cxFloat dt)
{
    uv_run(&looper, UV_RUN_NOWAIT);
}

void cxTcp::OnClose()
{
    Exit(true);
    onClose.Fire(this);
}

void cxTcp::OnData(char *buffer,cxInt size)
{
    onData.Fire(this, buffer, size);
}

void cxTcp::OnConnected()
{
    onConnected.Fire(this);
}

void cxTcp::OnIpAddr(cchars ip)
{
    onIpAddr.Fire(this, ip);
}

void cxTcp::close_cb(uv_handle_t* handle)
{
    cxTcp *tcp = static_cast<cxTcp *>(handle->data);
    tcp->connected = false;
    tcp->OnClose();
}

void cxTcp::alloc_cb(uv_handle_t* handle,size_t suggested,uv_buf_t* buf)
{
    cxTcp *tcp = static_cast<cxTcp *>(handle->data);
    if(suggested > tcp->bufsiz){
        tcp->buffer = (char *)realloc(tcp->buffer,suggested);
        tcp->bufsiz = (cxInt)suggested;
    }
    buf->base = tcp->buffer;
    buf->len = tcp->bufsiz;
}

void cxTcp::Close(cxInt err)
{
    error = err;
    if(!uv_is_closing((uv_handle_t *)&handle)){
        uv_close((uv_handle_t *)&handle, close_cb);
    }
}

void cxTcp::read_cb(uv_stream_t* handle, ssize_t nread, const uv_buf_t* buf)
{
    cxTcp *tcp = static_cast<cxTcp *>(handle->data);
    if (nread >= 0) {
        tcp->OnData(buf->base, (cxInt)nread);
    }else{
        tcp->Close((cxInt)nread);
    }
}

void cxTcp::write_cb(uv_write_t* req, int status)
{
    cxStr *data = static_cast<cxStr *>(req->data);
    data->Release();
    free(req);
}

cxBool cxTcp::Write(cxStr *data)
{
    if(!connected){
        CX_WARN("write data failed,tcp not connect");
        return false;
    }
    if(!uv_is_writable((uv_stream_t *)&handle)){
        CX_WARN("write data failed,tcp not writable");
        return false;
    }
    uv_write_t *wreq = (uv_write_t *)malloc(sizeof(uv_write_t));
    data->Retain();
    wreq->data = data;
    uv_buf_t buf = uv_buf_init(data->Buffer(),data->Size());
    cxInt ret = uv_write(wreq, (uv_stream_t *)&handle, &buf, 1, write_cb);
    if(ret == 0){
        return true;
    }
    Close(ret);
    free(wreq);
    data->Release();
    return false;
}

void cxTcp::connect_cb(uv_connect_t* req, int status)
{
    cxTcp *tcp = static_cast<cxTcp *>(req->data);
    if(status != 0){
        tcp->Close(status);
        return;
    }
    tcp->connected = true;
    tcp->OnConnected();
    cxInt ret = uv_read_start(req->handle, alloc_cb, read_cb);
    if(ret != 0){
        tcp->Close(ret);
    }
}

void cxTcp::resolved_cb(uv_getaddrinfo_t *resolver, int status, struct addrinfo *res)
{
    cxTcp *tcp = static_cast<cxTcp *>(resolver->data);
    cxBool flags = false;
    if(status == 0){
        if(res->ai_protocol == IPPROTO_IPV6){
            char addrs[65]={0};
            uv_ip6_name((struct sockaddr_in6*)res->ai_addr, addrs, 64);
            tcp->OnIpAddr(addrs);
        }else{
            char addrs[17]={0};
            uv_ip4_name((struct sockaddr_in*)res->ai_addr, addrs, 16);
            tcp->OnIpAddr(addrs);
        }
        tcp->connreq.data = tcp;
        flags=(uv_tcp_connect(&tcp->connreq, &tcp->handle, (struct sockaddr *)res->ai_addr, connect_cb) == 0);
    }
    if(!flags){
        CX_ERROR("resolved_cb failed=%s errno=%s",hstrerror(status),strerror(errno));
        tcp->Close(status);
    }
    uv_freeaddrinfo(res);
}

cxBool cxTcp::IsConnected()
{
    return connected;
}

cxBool cxTcp::Connect(cchars host,cxInt port)
{
    resolver.data = this;
    char ports[16]={0};
    snprintf(ports, 16, "%d",port);
    cxInt ret = uv_getaddrinfo(&looper,&resolver,resolved_cb,host,ports,&hints);
    if(ret != 0){
        Close(ret);
    }
    return ret == 0;
}

CX_CPP_END

