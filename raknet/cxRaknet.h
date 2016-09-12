//
//  cxRaknet.h
//  RaknetServer
//
//  Created by xuhua on 5/4/16.
//  Copyright © 2016 xuhua. All rights reserved.
//
//peer->Send((char*)message, strlen(message)+1, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_RAKNET_GUID, true);
//1、字节流 2、有多少字节要发送 3、数据包的优先级 4、获取数据的序列和子串 5、使用哪个有序流 6、要发送到的远端系统（UNASSIGNED_RAKNET_GUID） 7、表明是否广播到所有的连接系统或不广播
/*
 其中连接消息作为Packet::data结构的第一个字节返回，如下：
 连接关闭:
 ID_DISCONNECTION_NOTIFICATION  丢失通知
 ID_CONNECTION_LOST  连接关闭
 
 新的连接:
 ID_NEW_INCOMING_CONNECTION 新的连接
 ID_CONNECTION_REQUEST_ACCEPTED  请求接受
 
 连接尝试失败:
 ID_CONNECTION_ATTEMPT_FAILED 连接失败
 ID_REMOTE_SYSTEM_REQUIRES_PUBLIC_KEY  公钥
 ID_OUR_SYSTEM_REQUIRES_SECURITY  安全请求
 ID_PUBLIC_KEY_MISMATCH
 ID_ALREADY_CONNECTED 已经存在
 ID_NO_FREE_INCOMING_CONNECTIONS  未释放连接
 ID_CONNECTION_BANNED
 ID_INVALID_PASSWORD  无效密码
 ID_INCOMPATIBLE_PROTOCOL_VERSION  无效协议
 ID_IP_RECENTLY_CONNECTED  已连接
 */

#ifndef RaknetServer_cxRaknet_h
#define RaknetServer_cxRaknet_h

#include <core/cxStr.h>
#include "RakPeerInterface.h"
#include "RakSleep.h"
#include "MessageIdentifiers.h"
#include "BitStream.h"
#include "SecureHandshake.h"
#include "GetTime.h"
#include "cxPackage.h"

CX_CPP_BEGIN

const RakNet::MessageID ID_MESSAGE_PACKET = ID_USER_PACKET_ENUM + 1;

class cxRaknet : public cxObject
{
public:
    CX_DECLARE(cxRaknet);
protected:
    explicit cxRaknet();
    virtual ~cxRaknet();
private:
    uv_key_t key;
    void InitBitStream(RakNet::BitStream *bs,const cxStr *data);
protected:
    //
    cxUInt32 UDPWrite(const cxStr *message,RakNet::AddressOrGUID clientId,
                   PacketPriority priority, PacketReliability reliability, char channel,
                   bool broadcast, uint32_t receipt);
    //
    cxUInt32 UDPWrite(cxPackHeader *pack,RakNet::AddressOrGUID clientId,
                      PacketPriority priority, PacketReliability reliability, char channel,
                      bool broadcast, uint32_t receipt);
    RakNet::RakNetGUID remote;//client use
    RakNet::SocketDescriptor socket;
    RakNet::RakPeerInterface *peer;
    //
    void ReadMessage(RakNet::Packet *packet);
public:
    void SetKey(void *data)
    {
        uv_key_set(&key, data);
    }
    template<class T>
    T*GetKey()
    {
        return (T *)uv_key_get(&key);
    }
public:
    cxInt GetPing(const RakNet::AddressOrGUID guid);
    cxUInt64 GUID();
    
    void Shutdown(cxInt wait);
    cxInt UdpCount();
    cxInt UdpMax();
    void SetOccasionalPing(bool ping);
    
    virtual void OnUDPPackage(RakNet::Packet *packet,cchars data,cxInt size);
    
    virtual void ThreadBegin();
    virtual void ThreadExit();
    
    virtual void Update();
    virtual void OnPacket(RakNet::Packet *packet);
    
    //unconnect ping pong
    void Ping(cchars host,cxInt port);
    //time＝send time
    virtual void OnPong(RakNet::SystemAddress addr,RakNet::TimeMS ping);
    virtual void OnPing(RakNet::SystemAddress addr);
public:
    cxUInt32 UDPWrite(cxPackHeader *pack,RakNet::AddressOrGUID clientId,bool broadcast=false);
public:
    cxUInt32 UDPWrite(cxPackHeader *pack,bool broadcast=false);
    cxUInt32 UDPWrite(const cxStr *message,bool broadcast=false);
    cxUInt32 UDPWrite(cxPackHeader *pack,PacketPriority priority, PacketReliability reliability, char channel, bool broadcast);
    cxUInt32 UDPWrite(const cxStr *message,PacketPriority priority, PacketReliability reliability, char channel,bool broadcast=false);
public:
    cxEvent<cxRaknet,RakNet::SystemAddress,RakNet::TimeMS> onPong;
};

CX_CPP_END

#endif


