//
//  cxTcp.h
//  cxEngineCore
//
//  Created by xuhua on 7/3/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#ifndef cxEngineCore_cxTcp_h
#define cxEngineCore_cxTcp_h

#include <uv/uv.h>
#include <core/cxStr.h>

CX_CPP_BEGIN

class cxTcp : public cxObject
{
public:
    CX_DECLARE(cxTcp);
protected:
    explicit cxTcp();
    virtual ~cxTcp();
private:
    cxInt error;
    uv_tcp_t handle;
    uv_connect_t connreq;
    uv_shutdown_t shutreq;
    uv_getaddrinfo_t resolver;
    struct addrinfo hints;
    char *buffer;
    cxInt bufsiz;
    cxBool shutdown;
    cxBool connected;
    static void shutdown_cb(uv_shutdown_t* req, int status);
    static void close_cb(uv_handle_t* handle);
    static void alloc_cb(uv_handle_t* handle,size_t suggested,uv_buf_t* buf);
    static void read_cb(uv_stream_t* tcp, ssize_t nread, const uv_buf_t* buf);
    static void connect_cb(uv_connect_t* req, int status);
    static void resolved_cb(uv_getaddrinfo_t *resolver, int status, struct addrinfo *res);
    static void write_cb(uv_write_t* req, int status);
protected:
    virtual void OnData(char *buffer,cxInt size);
    virtual void OnConnected();
    virtual void OnClose();
    virtual void OnIpAddr(cchars ip);
public:
    cxBool Connect(cchars host,cxInt port);
    void Close(cxInt err);
    cxBool Write(cxStr *data);
    cxEvent<cxTcp> onConnected;
    cxEvent<cxTcp> onClose;
};

CX_CPP_END

#endif


