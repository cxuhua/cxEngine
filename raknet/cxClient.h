//
//  cxClient.h
//  RaknetServer
//
//  Created by xuhua on 5/4/16.
//  Copyright © 2016 xuhua. All rights reserved.
//

#ifndef RaknetServer_cxClient_h
#define RaknetServer_cxClient_h

#include "cxRaknet.h"

CX_CPP_BEGIN

class ServerInfo;
class cxClient : public cxRaknet
{
public:
    CX_DECLARE(cxClient);
protected:
    explicit cxClient();
    virtual ~cxClient();
private:
    ServerInfo *serverInfo;
    char publicKeyData[cat::EasyHandshake::PUBLIC_KEY_BYTES];
    RakNet::PublicKey publicKey;
public:
    const ServerInfo *GetServerInfo();
    void SetPublicKey(const cxStr *data);
    void OnPacket(RakNet::Packet *packet);
    
    void Connect(const ServerInfo *info);
    void Connect(cchars host,cxInt port,cchars pass);
public:
    void OnMessage(RakNet::RakNetGUID clientId, const cxStr *message);
    virtual void OnConnected();
    virtual void OnLost();
    virtual void OnError(cxInt error);
public:
    cxEvent<cxClient> onConnected;
    cxEvent<cxClient> onLost;
    cxEvent<cxClient,cxInt> onError;
    cxEvent<cxClient,RakNet::RakNetGUID, const cxStr *> onMessage;
};

CX_CPP_END

#endif


