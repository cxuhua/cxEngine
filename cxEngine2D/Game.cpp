//
//  IOSEngine.cpp
//  cxEngine2D
//
//  Created by xuhua on 6/17/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#include <engine/cxSprite.h>
#include <engine/cxTexture.h>
#include <engine/cxAtlas.h>
#include <engine/cxLabel.h>
#include <core/cxUtil.h>
#include <engine/cxMove.h>
#include <engine/cxRotate.h>
#include <engine/cxScale.h>
#include <engine/cxButton.h>
#include <engine/cxContainer.h>
#include <engine/cxEmitter.h>
#include <engine/cxTimeLine.h>
#include <engine/cxTimer.h>
#include <engine/cxAnimate.h>
#include <engine/cxTcp.h>
#include <engine/cxHttp.h>
#include <engine/cxSpline.h>
#include <engine/cxFollow.h>
#include <engine/cxBezier.h>
#include <engine/cxFade.h>
#include <engine/cxTint.h>
#include <engine/cxSequence.h>
#include <engine/cxJump.h>
#include <engine/cxResize.h>
#include <engine/cxProgress.h>
#include <engine/cxTable.h>
#include <engine/cxPath.h>
#include <engine/cxTriangles.h>
#include <engine/cxMusic.h>
#include "Game.h"
#include <core/cxKDTree.h>
#include "Sea.h"
#include <engine/cxBox2D.h>
#include <engine/cxSpine.h>
#include <core/cxSqlite.h>
#include <engine/cxScript.h>

#include <raknet/MessageIdentifiers.h>
#include <raknet/RakPeerInterface.h>
#include <raknet/BitStream.h>

CX_CPP_BEGIN

CX_IMPLEMENT(Game);

void Game::OnUpdate(cxFloat dt)
{
    RakNet::Packet *packet = peer->Receive();
    if(packet != nullptr){
        enum DefaultMessageIDTypes type = (enum DefaultMessageIDTypes)packet->data[0];
        switch (type) {
            case ID_CONNECTION_REQUEST_ACCEPTED:{
                //连接成功
                CX_LOGGER("ID_CONNECTION_REQUEST_ACCEPTED %s",peer->GetMyGUID().ToString());
                const RakNet::MessageID ID_CREATE_SOLDIER = ID_USER_PACKET_ENUM + 1;
                RakNet::BitStream bsOut;
                bsOut.Write(ID_CREATE_SOLDIER);
                char buf[1024*4 + 1]={0};
                for(int i =0; i < 1024*4;i++){
                    buf[i] = 'a';
                }
                bsOut.Write(buf);
                //UNASSIGNED_SYSTEM_ADDRESS true 广播
                peer->Send(&bsOut,IMMEDIATE_PRIORITY, RELIABLE_ORDERED, 0, packet->guid, false);
                break;
            }
            case ID_CONNECTION_LOST:{
                //连接断开
                CX_LOGGER("ID_CONNECTION_LOST %s",peer->GetMyGUID().ToString());
                break;
            }
            case ID_CONNECTION_ATTEMPT_FAILED:{
                //连接失败
                CX_LOGGER("ID_CONNECTION_ATTEMPT_FAILED %s",peer->GetMyGUID().ToString());
                peer->Connect("192.168.199.244", 10020, "123", 3);
                break;
            }
            case ID_DISCONNECTION_NOTIFICATION:{
                //收到断开通知，服务器端调用CloseConnection(guid,true)时
                CX_LOGGER("ID_DISCONNECTION_NOTIFICATION %s",peer->GetMyGUID().ToString());
                break;
            }
            default:{
                CX_LOGGER("%d",type);
                break;
            }
        }
        peer->DeallocatePacket(packet);
    }
    cxEngine::OnUpdate(dt);
}

Game::Game()
{
    RakNet::SocketDescriptor socket;
    peer = RakNet::RakPeerInterface::GetInstance();
    peer->Startup(1, &socket, 1);
    peer->Connect("192.168.199.244", 10020, "123", 3);
}

Game::~Game()
{
    
}

void Game::OnMain()
{
    SetPlanSize(cxSize2F(2048, 1536));
    
    cxSprite *sp = cxSprite::Create();
    sp->SetSize(cxSize2F(250, 250));
    sp->SetTexture(cxTexture::Create()->From("grid.png"));
    Window()->Append(sp);
    
    cxRotateBy *by = cxRotateBy::Create(1.0f, 1000.0f);
    sp->Append(by);
}

CX_CPP_END

