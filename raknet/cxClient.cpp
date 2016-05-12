//
//  cxClient.cpp
//  RaknetServer
//
//  Created by xuhua on 5/4/16.
//  Copyright © 2016 xuhua. All rights reserved.
//


#include "cxClient.h"
#include "ListServers.h"

CX_CPP_BEGIN

CX_IMPLEMENT(cxClient);

cxClient::cxClient()
{
    memset(&publicKey, 0, sizeof(publicKey));
    peer->Startup(1, &socket, 1);
}

cxClient::~cxClient()
{
    
}

void cxClient::SetPublicKey(const cxStr *data)
{
    CX_ASSERT(data != nullptr && data->Size() == cat::EasyHandshake::PUBLIC_KEY_BYTES, "public key data error");
    memcpy(publicKeyData, data->Data(), cat::EasyHandshake::PUBLIC_KEY_BYTES);
    publicKey.publicKeyMode = RakNet::PKM_ACCEPT_ANY_PUBLIC_KEY;
    publicKey.myPublicKey = publicKeyData;
}

void cxClient::OnPacket(RakNet::Packet *packet)
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
        default:{
            cxRaknet::OnPacket(packet);
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
}

void cxClient::OnLost()
{
    CX_LOGGER("OnLost");
}

void cxClient::OnMessage(RakNet::RakNetGUID clientId, const cxStr *message)
{
    CX_LOGGER("OnMessage %s",message->ToString());
}

void cxClient::Connect(const ServerInfo *info)
{
    CX_ASSERT(info->Public != nullptr, "public key null");
    SetPublicKey(info->Public);
    CX_ASSERT(info->Host != nullptr, "host null");
    CX_ASSERT(info->Port >0, "port error");
    CX_ASSERT(info->Pass!= nullptr, "pass null");
    Connect(info->Host->ToString(), info->Port, info->Pass->ToString());
}

void cxClient::Connect(cchars host,cxInt port,cchars pass)
{
    CX_ASSERT(publicKey.myPublicKey != NULL, "public key miss");
    peer->SetOccasionalPing(true);
    peer->Connect(host, port, pass, (int)strlen(pass),&publicKey);
}

CX_CPP_END

