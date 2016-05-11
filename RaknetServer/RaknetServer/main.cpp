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

MongoDB *GameServer::GetDB()
{
    return db;
}

void GameServer::OnNewConnect(RakNet::RakNetGUID clientId)
{
//    Config *conf = GetKey<Config>();
}

void GameServer::OnLost(RakNet::RakNetGUID clientId)
{
//    Config *conf = GetKey<Config>();
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
        BSONBinData pb = BSONBinData(publicKey,cat::EasyHandshake::PUBLIC_KEY_BYTES,BinDataGeneral);
        BSONBinData pk = BSONBinData(privateKey,cat::EasyHandshake::PRIVATE_KEY_BYTES,BinDataGeneral);
        BSONObj d = BSON("_id" << id
                         << "host" << host
                         << "port" << port
                         << "pass" << pass
                         << "max" << max
                         << "curr" << curr
                         << "time" << (long long)0
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
    //更新当前服务器状态
    GameServer *server = (GameServer *)handle->data;
    unsigned short num = server->UdpCount();
    try{
        long long time = (long long)cxUtil::Timestamp();
        BSONObj q = BSON("_id" << server->id);
        BSONObj d = BSON("time" << time << "curr" << num);
        server->GetDB()->Update(T_SERVERS, q, BSON("$set" << d));
    }catch(DBException &e){
        CX_ERROR("update server status error :%s",e.getInfo().toString().c_str());
    }
    //维护到其它服务器的连接状态
    try{
        long long time = (long long)cxUtil::Timestamp() - UPDATE_STATUS_TIME ;
        Query q = MONGO_QUERY("time" << GTE << time);
        std::auto_ptr<DBClientCursor> iter = server->GetDB()->Find(T_SERVERS, q);
        while(iter->more()){
            BSONObj obj = iter->next();
            //自己不能连接自己
            if(obj["_id"].String() == server->id){
                continue;
            }
            //如果连接丢失重新连接到其它服务器
            cchars host = obj["host"].String().c_str();
            cxInt port = obj["port"].Int();
            RakNet::SystemAddress addr(host,port);
            if(server->HasConnection(addr)){
                continue;
            }
            server->Connect(host, port);
        }
    }catch(DBException &e){
        CX_ERROR("list server error :%s",e.getInfo().toString().c_str());
    }
    CX_LOGGER("STATS TCP=%d/%d UDP=%d/%d",server->TcpCount(),server->TcpMax(),num,server->UdpMax());
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

void GameServer::signalExit(uv_signal_t* handle, int signum)
{
    CX_LOGGER("Server recv signal %d",signum);
    GameServer *server = (GameServer *)handle->data;
    server->Stop();
}

GameServer::GameServer()
{
    //主线程loop
    uv_loop_init(&loop);
    //每隔10秒更新服务器状态
    uv_timer_init(&loop, &timer);
    timer.data = this;
    uv_timer_start(&timer, updateServerStatus, UPDATE_STATUS_TIME, UPDATE_STATUS_TIME);
    //kill信号处理
    uv_signal_init(&loop, &sigkill);
    sigkill.data = this;
    uv_signal_start(&sigkill, GameServer::signalExit, SIGQUIT);
    //主线程数据库连接
    db = MongoDB::Alloc();
}

GameServer::~GameServer()
{
    uv_loop_close(&loop);
    db->Release();
}


int GameServer::Main(int argc, const char * argv[])
{
    signal(SIGPIPE, SIG_IGN);
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



