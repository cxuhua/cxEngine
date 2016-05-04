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

class cxClient : public cxRaknet
{
public:
    CX_DECLARE(cxClient);
protected:
    explicit cxClient();
    virtual ~cxClient();
private:
public:
    void OnPacket(RakNet::Packet *packet,void *data);
    RakNet::ConnectionAttemptResult Connect(cchars host,cxInt port,cchars pass);
public:
    void OnMessage(RakNet::RakNetGUID clientId, const cxStr *message); 
    virtual void OnConnected();
    virtual void OnLost();
    virtual void OnError(cxInt error);
};

CX_CPP_END

#endif


