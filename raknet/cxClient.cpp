//
//  cxClient.cpp
//  RaknetServer
//
//  Created by xuhua on 5/4/16.
//  Copyright © 2016 xuhua. All rights reserved.
//


#include "cxClient.h"

CX_CPP_BEGIN

CX_IMPLEMENT(cxClient);

cxClient::cxClient()
{
    peer->Startup(1, &socket, 1);
}

cxClient::~cxClient()
{
    
}

void cxClient::OnPacket(RakNet::Packet *packet,void *data)
{
    RakNet::MessageID type = packet->data[0];
    switch (type) {
        case ID_CONNECTION_REQUEST_ACCEPTED:{
            remote = packet->guid;
            OnConnected();
            break;
        }
        case ID_CONNECTION_LOST:{
            OnLost();
            break;
        }
        case ID_CONNECTION_ATTEMPT_FAILED:{
            OnError(1);
            break;
        }
        case ID_DISCONNECTION_NOTIFICATION:{
            OnLost();
            break;
        }
        case ID_MESSAGE_PACKET:{
            ReadMessage(packet);
            break;
        }
        default:{
            CX_LOGGER("onMessageType %d not process",type);
            break;
        }
    }
}

void cxClient::OnError(cxInt error)
{
    CX_LOGGER("OnError:%d",error);
}

void cxClient::OnConnected()
{
    CX_LOGGER("OnConnected");
    Write(cxStr::UTF8("123456"));
}

void cxClient::OnLost()
{
    CX_LOGGER("OnLost");
}

void cxClient::OnMessage(RakNet::RakNetGUID clientId, const cxStr *message)
{
    CX_LOGGER("OnMessage %s",message->ToString());
}

RakNet::ConnectionAttemptResult cxClient::Connect(cchars host,cxInt port,cchars pass)
{
    CX_ASSERT(peer != nullptr, "not init");
    return peer->Connect(host, port, pass, (int)strlen(pass));
}

CX_CPP_END

