//
//  cxRaknet.cpp
//  RaknetServer
//
//  Created by xuhua on 5/4/16.
//  Copyright © 2016 xuhua. All rights reserved.
//


#include "cxRaknet.h"

CX_CPP_BEGIN

CX_IMPLEMENT(cxRaknet);

cxRaknet::cxRaknet()
{
    peer = RakNet::RakPeerInterface::GetInstance();
    uv_key_create(&key);
}

cxRaknet::~cxRaknet()
{
    uv_key_delete(&key);
    RakNet::RakPeerInterface::DestroyInstance(peer);
}

void cxRaknet::SetOccasionalPing(bool ping)
{
    peer->SetOccasionalPing(ping);
}

void cxRaknet::OnMessage(RakNet::RakNetGUID clientId, const cxStr *message)
{
    
}

void cxRaknet::ReadMessage(RakNet::Packet *packet)
{
    RakNet::BitStream bsIn(packet->data, packet->length, false);
    bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
    unsigned short l;
    if(!bsIn.Read(l)){
        return;
    }
    char buf[l+1];
    if(!bsIn.Read(buf, l)){
        return;
    }
    buf[l] = 0;
    OnMessage(packet->guid, cxStr::Create()->Init(buf, l));
}

void cxRaknet::OnPacket(RakNet::Packet *packet)
{
    RakNet::MessageID type = packet->data[0];
    switch (type) {
        case ID_MESSAGE_PACKET:{
            ReadMessage(packet);
            break;
        }
        case ID_PUBLIC_KEY_MISMATCH:{
            CX_LOGGER("public key mismatch");
            break;
        }
        default:{
            CX_LOGGER("onMessageType %d not process",type);
            break;
        }
    }
}

void cxRaknet::ThreadBegin()
{
    
}

void cxRaknet::ThreadExit()
{
    
}

void cxRaknet::Process()
{
    RakNet::Packet *packet = nullptr;
    for(packet=peer->Receive(); packet; peer->DeallocatePacket(packet), packet=peer->Receive()) {
        OnPacket(packet);
    }
}

void cxRaknet::InitBitStream(RakNet::BitStream *bs,const cxStr *data)
{
    unsigned short l = data->Size();
    bs->Write(ID_MESSAGE_PACKET);
    bs->Write(l);
    bs->WriteAlignedBytes((const unsigned char *)data->Data(), (const unsigned int)l);
}

cxUInt32 cxRaknet::Write(const cxStr *message)
{
    return Write(message, remote, IMMEDIATE_PRIORITY, RELIABLE_ORDERED, 0,false ,0);
}

cxUInt32 cxRaknet::Write(const cxStr *message,PacketPriority priority, PacketReliability reliability, char channel)
{
    return Write(message, remote, priority, reliability, channel, false, 0);
}

cxUInt32 cxRaknet::Broadcast(const cxStr *message)
{
    return Broadcast(message, IMMEDIATE_PRIORITY, RELIABLE_ORDERED, 0);
}

cxUInt32 cxRaknet::Broadcast(const cxStr *message,PacketPriority priority, PacketReliability reliability, char channel)
{
    return Write(message, RakNet::UNASSIGNED_SYSTEM_ADDRESS, priority, reliability, channel, true, 0);
}

cxUInt32 cxRaknet::Write(const cxStr *message,RakNet::AddressOrGUID clientId,PacketPriority priority, PacketReliability reliability, char channel,bool broadcast, uint32_t receipt)
{
    RakNet::BitStream bsOut;
    InitBitStream(&bsOut, message);
    return peer->Send(&bsOut, priority, reliability, channel, clientId, broadcast,receipt);
}


CX_CPP_END

