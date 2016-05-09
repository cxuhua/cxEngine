//
//  main.cpp
//  RaknetServer
//
//  Created by xuhua on 5/3/16.
//  Copyright © 2016 xuhua. All rights reserved.
//

#include "main.h"

CX_CPP_BEGIN

CX_IMPLEMENT(GameServer);

void GameServer::ThreadBegin()
{
    Config *conf = Config::Alloc();
    conf->SetServer(this);
    SetKey(conf);
}

void GameServer::ThreadExit()
{
    GetKey<Config>()->Release();
}

void GameServer::ThreadLoop()
{
    Config *conf = GetKey<Config>();
    uv_run(conf->Looper(), UV_RUN_NOWAIT);
}

cchars GameServer::GetHost()
{
    return host.c_str();
}

cxInt GameServer::GetPort()
{
    return port;
}

cchars GameServer::GetId()
{
    return id.c_str();
}

cxInt GameServer::GetMax()
{
    return max;
}

cxInt GameServer::GetCurr()
{
    return curr;
}

DB *GameServer::GetDB()
{
    return db;
}

void GameServer::OnNewConnect(RakNet::RakNetGUID clientId)
{
    Config *conf = GetKey<Config>();
    conf->IncCurr(+1);
}

void GameServer::OnLost(RakNet::RakNetGUID clientId)
{
    Config *conf = GetKey<Config>();
    conf->IncCurr(-1);
}

void GameServer::OnMessage(RakNet::RakNetGUID clientId,const cxStr *message)
{
//    Config *conf = GetKey<Config>();
}

void GameServer::Register(cchars aid,cchars ahost,cxInt aport,cchars apass,cxInt amax)
{
    try{
        curr = 0;
        pass = apass;
        id = aid;
        host = ahost;
        port = aport;
        max = amax;
        long long time = (long long)cxUtil::Timestamp();
        BSONBinData pb = BSONBinData(publicKey,cat::EasyHandshake::PUBLIC_KEY_BYTES,BinDataGeneral);
        BSONBinData pk = BSONBinData(privateKey,cat::EasyHandshake::PRIVATE_KEY_BYTES,BinDataGeneral);
        BSONObj d = BSON("_id" << id
                         << "host" << host
                         << "port" << port
                         << "pass" << pass
                         << "max" << max
                         << "curr" << curr
                         << "time" << time
                         << "public" << pb
                         << "private" << pk);
        BSONObj q = BSON("_id" << id);
        GetDB()->Upsert(T_SERVERS, q , d);
    }catch(DBException &e){
        CX_ERROR("Register server error:%s",e.getInfo().toString().c_str());
        exit(1);
    }
}

void GameServer::updateServerStatus(uv_timer_t* handle)
{
    GameServer *server = (GameServer *)handle->data;
    try{
        long long time = (long long)cxUtil::Timestamp();
        BSONObj q = BSON("_id" << server->id);
        BSONObj d = BSON("time" << time);
        server->GetDB()->Update(T_SERVERS, q, BSON("$set" << d));
    }catch(DBException &e){
        CX_ERROR("update server status error :%s",e.getInfo().toString().c_str());
    }
}

void GameServer::Stop()
{
    //停止子线程
    cxServer::Stop();
    //停止主线程
    uv_stop(&loop);
}

void GameServer::Run()
{
    cxServer::Run();
    uv_run(&loop, UV_RUN_DEFAULT);
    //等待子线程结束
    Wait();
    //移除服务器列表
    GetDB()->RemoveId(T_SERVERS, id);
}

GameServer::GameServer()
{
    //主线程loop
    uv_loop_init(&loop);
    //每隔10秒更新服务器状态
    uv_timer_init(&loop, &timer);
    timer.data = this;
    uv_timer_start(&timer, updateServerStatus, UPDATE_STATUS_TIME, UPDATE_STATUS_TIME);
    //主线程数据库连接
    db = DB::Alloc();
}

GameServer::~GameServer()
{
    uv_loop_close(&loop);
    db->Release();
}

int GameServer::Main(int argc, const char * argv[])
{
    cchars host = "192.168.199.244";
    cchars pass = "123";
    cxInt max = 512;
    cxInt port = 9000;
    cchars id = "test";
    cxInt thread = 5;
    mongo::client::GlobalInstance instance;
    if(!instance.initialized()){
        CX_ERROR("mongo client initialized failed");
        return 1;
    }
    GameServer *server = GameServer::Alloc();
    if(!server->Init(thread, port, max, pass)){
        CX_ERROR("game server init failed");
        return 2;
    }
    server->Register(id, host, port, pass, max);
    server->Run();
    return 0;
}

CX_CPP_END

int main(int argc, const char * argv[])
{
    return cxengine::GameServer::Main(argc, argv);
}



