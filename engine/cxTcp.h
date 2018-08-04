//
//  cxTcp.h
//  cxEngineCore
//  tcp net
//  Created by xuhua on 7/3/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#ifndef cxEngineCore_cxTcp_h
#define cxEngineCore_cxTcp_h

#include <uv/uv.h>
#include <core/cxStr.h>
#include "cxAction.h"

CX_CPP_BEGIN

#define CX_TCP_BUFFER_SIZE  4096

class cxTcp : public cxAction
{
public:
    CX_DECLARE(cxTcp);
protected:
    explicit cxTcp();
    virtual ~cxTcp();
private:
    uv_loop_t looper;
    cxInt error;
    uv_tcp_t handle;
    uv_connect_t connreq;
    uv_shutdown_t shutreq;
    uv_getaddrinfo_t resolver;
    
    struct addrinfo hints;
    
    char *buffer;
    cxInt bufsiz;
    cxBool connected;
    
    static void close_cb(uv_handle_t* handle);
    static void alloc_cb(uv_handle_t* handle,size_t suggested,uv_buf_t* buf);
    static void read_cb(uv_stream_t* tcp, ssize_t nread, const uv_buf_t* buf);
    static void connect_cb(uv_connect_t* req, int status);
    static void resolved_cb(uv_getaddrinfo_t *resolver, int status, struct addrinfo *res);
    static void write_cb(uv_write_t* req, int status);
protected:
    //
    virtual void OnData(char *buffer,cxInt size);
    //
    virtual void OnConnected();
    
    virtual void OnClose();
    
    virtual void OnIpAddr(cchars ip);
    
    virtual void OnStep(cxFloat dt);
public:
    cxInt Error();
    cxBool Connect(cchars host,cxInt port);
    void Close(cxInt err);
    cxBool Write(cxStr *data);
    cxBool IsConnected();
public:
    cxEvent<cxTcp, cchars, cxInt> onData;
    cxEvent<cxTcp, cchars> onIpAddr;
    cxEvent<cxTcp> onConnected;
    cxEvent<cxTcp> onClose;
public:
    static cxTcp *Create(cchars host,cxInt port);
};

CX_CPP_END

#endif


