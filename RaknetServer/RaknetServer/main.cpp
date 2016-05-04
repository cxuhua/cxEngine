//
//  main.cpp
//  RaknetServer
//
//  Created by xuhua on 5/3/16.
//  Copyright © 2016 xuhua. All rights reserved.
//

#include <raknet/RakPeerInterface.h>
#include <raknet/RakSleep.h>
#include <raknet/MessageIdentifiers.h>
#include <raknet/BitStream.h>
#include <iostream>
#include <core/cxAutoPool.h>
#include <core/cxStr.h>

using namespace cxengine;

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

RakNet::RakPeerInterface* peer;

void a(void *b){
    cxAutoPool::Start();
    printf("%p start\n",pthread_self());
    int i = 1;
    while(i > 0){
        RakNet::Packet *packet;
        const RakNet::MessageID ID_CREATE_SOLDIER = ID_USER_PACKET_ENUM + 1;
        for(packet=peer->Receive(); packet; peer->DeallocatePacket(packet), packet=peer->Receive()) {
            RakNet::MessageID type = packet->data[0];
            switch (type) {
                case ID_NEW_INCOMING_CONNECTION:{
                    //新连接
                    printf("ID_NEW_INCOMING_CONNECTION %s\n",packet->guid.ToString());
                    //                    peer->CloseConnection(packet->guid, true);
                    break;
                }
                case ID_CONNECTION_LOST:{
                    //client断开
                    printf("ID_CONNECTION_LOST %s\n",packet->guid.ToString());
                    break;
                }
                case ID_DISCONNECTION_NOTIFICATION:{
                    //client断开通知
                    printf("ID_DISCONNECTION_NOTIFICATION %s\n",packet->guid.ToString());
                    break;
                }
                case ID_CREATE_SOLDIER:{
                    printf("ID_CREATE_SOLDIER %s\n",packet->guid.ToString());
                    RakNet::BitStream bsIn(packet->data, packet->length, false);
                    bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
                    unsigned short l;
                    bsIn.Read(l);
                    printf("%d\n",l);
                    char buf[l+1];
                    bsIn.Read(buf, l);
                    buf[l] = 0;
                    printf("%s %d\n",buf,(int)strlen(buf));
                    break;
                }
                default:{
                    printf("%d\n",type);
                    break;
                }
            }
        }
        
        cxAutoPool::Update();
        RakSleep(10);
    }
    cxAutoPool::Stop();
}

int main(int argc, const char * argv[]) {
    
    
//    MAX_OFFLINE_DATA_LENGTH
    
    RakNet::SocketDescriptor socket(10020,0);
    peer = RakNet::RakPeerInterface::GetInstance();
    peer->Startup(512, &socket, 1);
    peer->SetMaximumIncomingConnections(512);
    peer->SetIncomingPassword("123", 3);
    
    cxAutoPool::Start();
    cxAutoPool::Update();
    
    uv_thread_t p1,p2;
    
    uv_thread_create(&p1, a, nullptr);
    uv_thread_create(&p2, a, nullptr);

//    RakNet::Packet *packet;//通常要在一个循环中调用这个函数
//    const RakNet::MessageID ID_CREATE_SOLDIER = ID_USER_PACKET_ENUM + 1;
//    while(true){
//        for(packet=peer->Receive(); packet; peer->DeallocatePacket(packet), packet=peer->Receive()) {
//            RakNet::MessageID type = packet->data[0];
//            switch (type) {
//                case ID_NEW_INCOMING_CONNECTION:{
//                    //新连接
//                    printf("ID_NEW_INCOMING_CONNECTION %s\n",packet->guid.ToString());
////                    peer->CloseConnection(packet->guid, true);
//                    break;
//                }
//                case ID_CONNECTION_LOST:{
//                    //client断开
//                    printf("ID_CONNECTION_LOST %s\n",packet->guid.ToString());
//                    break;
//                }
//                case ID_DISCONNECTION_NOTIFICATION:{
//                    //client断开通知
//                    printf("ID_DISCONNECTION_NOTIFICATION %s\n",packet->guid.ToString());
//                    break;
//                }
//                case ID_CREATE_SOLDIER:{
//                    printf("ID_CREATE_SOLDIER %s\n",packet->guid.ToString());
//                    RakNet::BitStream bsIn(packet->data, packet->length, false);
//                    bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
//                    unsigned short l;
//                    bsIn.Read(l);
//                    printf("%d\n",l);
//                    char buf[l];
//                    bsIn.Read(buf, l);
//                    printf("%s\n",buf);
//                    break;
//                }
//                default:{
//                    printf("%d\n",type);
//                    break;
//                }
//            }
//        }
//        RakSleep(10);
//    }
//
    while(true){
        RakSleep(1000);
    }
    RakNet::RakPeerInterface::DestroyInstance(peer);
    return 0;
}
