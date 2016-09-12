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

void cxRaknet::OnUDPPackage(RakNet::Packet *packet,cchars data,cxInt size)
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
    OnUDPPackage(packet,buf,l);
}

cxUInt64 cxRaknet::GUID()
{
    return peer->GetMyGUID().g;
}

cxInt cxRaknet::GetPing(const RakNet::AddressOrGUID guid)
{
    return peer->GetAveragePing(guid);
}

void cxRaknet::Ping(cchars host,cxInt port)
{
    peer->Ping(host, port, false);
}

void cxRaknet::OnPong(RakNet::SystemAddress addr,RakNet::TimeMS ping)
{
    onPong.Fire(this, addr, ping);
}

void cxRaknet::OnPing(RakNet::SystemAddress addr)
{
    //CX_LOGGER("PING %s",addr.ToString());
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
        case ID_UNCONNECTED_PONG:{
            RakNet::BitStream bs(packet->data, packet->length, false );
            bs.IgnoreBytes(1);
            RakNet::TimeMS sendTime;
            bs.Read(sendTime);
            RakNet::TimeMS ping = RakNet::GetTimeMS() - sendTime;
            OnPong(packet->systemAddress,ping);
            break;
        }
        case ID_UNCONNECTED_PING:{
            OnPing(packet->systemAddress);
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

void cxRaknet::Shutdown(cxInt wait)
{
    peer->Shutdown(wait);
}

cxInt cxRaknet::UdpCount()
{
    unsigned short num = 0;
    peer->GetConnectionList(nullptr, &num);
    return num;
}

cxInt cxRaknet::UdpMax()
{
    return peer->GetMaximumIncomingConnections();
}

void cxRaknet::Update()
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

cxUInt32 cxRaknet::UDPWrite(cxPackHeader *pack,RakNet::AddressOrGUID clientId,bool broadcast)
{
    if(broadcast){
        return UDPWrite(pack, RakNet::UNASSIGNED_RAKNET_GUID, IMMEDIATE_PRIORITY, RELIABLE_ORDERED, 0,broadcast ,0);
    }else{
        return UDPWrite(pack, clientId, IMMEDIATE_PRIORITY, RELIABLE_ORDERED, 0,broadcast ,0);
    }
}

cxUInt32 cxRaknet::UDPWrite(const cxStr *message, bool broadcast)
{
    if(broadcast){
        return UDPWrite(message, RakNet::UNASSIGNED_RAKNET_GUID, IMMEDIATE_PRIORITY, RELIABLE_ORDERED, 0,broadcast ,0);
    }else{
        return UDPWrite(message, remote, IMMEDIATE_PRIORITY, RELIABLE_ORDERED, 0,broadcast ,0);
    }
}

cxUInt32 cxRaknet::UDPWrite(cxPackHeader *pack, bool broadcast)
{
    if(broadcast){
        return UDPWrite(pack, RakNet::UNASSIGNED_RAKNET_GUID, IMMEDIATE_PRIORITY, RELIABLE_ORDERED, 0,broadcast ,0);
    }else{
        return UDPWrite(pack, remote, IMMEDIATE_PRIORITY, RELIABLE_ORDERED, 0,broadcast ,0);
    }
}

cxUInt32 cxRaknet::UDPWrite(const cxStr *message,PacketPriority priority, PacketReliability reliability, char channel, bool broadcast)
{
    return UDPWrite(message, remote, priority, reliability, channel, broadcast, 0);
}

cxUInt32 cxRaknet::UDPWrite(cxPackHeader *pack,PacketPriority priority, PacketReliability reliability, char channel, bool broadcast)
{
    return UDPWrite(pack, remote, priority, reliability, channel, broadcast, 0);
}

cxUInt32 cxRaknet::UDPWrite(const cxStr *message,RakNet::AddressOrGUID clientId,PacketPriority priority, PacketReliability reliability, char channel,bool broadcast, uint32_t receipt)
{
    RakNet::BitStream bsOut;
    InitBitStream(&bsOut, message);
    return peer->Send(&bsOut, priority, reliability, channel, clientId, broadcast,receipt);
}

cxUInt32 cxRaknet::UDPWrite(cxPackHeader *pack,RakNet::AddressOrGUID clientId,PacketPriority priority, PacketReliability reliability, char channel,bool broadcast, uint32_t receipt)
{
    pack->Ping = GetPing(clientId);
    SetHashHeader(pack);
    RakNet::BitStream bs;
    unsigned short l = pack->Size;
    bs.Write(ID_MESSAGE_PACKET);
    bs.Write(l);
    bs.WriteAlignedBytes((const unsigned char *)pack, (const unsigned int)l);
    return peer->Send(&bs, priority, reliability, channel, clientId, broadcast,receipt);
}


CX_CPP_END

