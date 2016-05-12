//
//  cxRaknetServer.h
//  RaknetServer
//
//  Created by xuhua on 5/4/16.
//  Copyright © 2016 xuhua. All rights reserved.
//

#ifndef RaknetServer_cxRaknetServer_h
#define RaknetServer_cxRaknetServer_h

#include <core/cxHash.h>
#include "cxRaknet.h"
#include "TCPInterface.h"

CX_CPP_BEGIN

class TcpServer :public cxObject,public RakNet::TCPInterface
{
public:
    CX_DECLARE(TcpServer);
protected:
    explicit TcpServer();
    virtual ~TcpServer();
public:
    bool HasConnection(RakNet::SystemAddress addr);
    cxInt MaxConnection();
};

class cxServer : public cxRaknet
{
public:
    CX_DECLARE(cxServer);
protected:
    explicit cxServer();
    virtual ~cxServer();
private:
    //用于服务器之间数据传输
    TcpServer *tcp;
public:
    virtual void OnTcpPacket(RakNet::Packet *packet);
    //有新连接
    virtual void OnTcpNewConnect(RakNet::SystemAddress addr);
    //连接关闭
    virtual void OnTcpLost(RakNet::SystemAddress addr);
    RakNet::SystemAddress Connect(cchars host,cxInt port);
    //连接成功
    virtual void OnTcpConnected(RakNet::SystemAddress addr);
    //连接失败
    virtual void OnTcpFailed(RakNet::SystemAddress addr);
    //关闭连接
    void CloseTcp(RakNet::SystemAddress addr);
    //发送数据 broadcast == true,addr = UNASSIGNED_SYSTEM_ADDRESS
    void TcpWrite(RakNet::SystemAddress addr,const cxStr *data,bool broadcast=false);
    //是否有此连接
    bool HasConnection(RakNet::SystemAddress addr);
    //连接数量
    cxInt TcpCount();
    //最大连接数
    cxInt TcpMax();
private:
    cxInt thread;
    static void runEntry(void *a);
    cxBool exitFlags;
    uv_thread_t *threads;
    uv_mutex_t mutex;
protected:
    char publicKey[cat::EasyHandshake::PUBLIC_KEY_BYTES];
    char privateKey[cat::EasyHandshake::PRIVATE_KEY_BYTES];
    bool initKey();
public:
    TcpServer *GetTcp();
    //主锁用于读写全局数据
    void Lock();
    void UnLock();
    //
    void Process();
    void OnPacket(RakNet::Packet *packet);
    void OnMessage(RakNet::RakNetGUID clientId,const cxStr *message);
public:
    virtual void Wait();
    virtual void Stop();
    virtual bool Init(cxInt nt,cxInt port,cxInt max,cchars pass);
    virtual void Run();
public:
    virtual void ThreadLoop();
    virtual void OnNewConnect(RakNet::RakNetGUID clientId);
    virtual void OnLost(RakNet::RakNetGUID clientId);
};

CX_CPP_END

#endif


