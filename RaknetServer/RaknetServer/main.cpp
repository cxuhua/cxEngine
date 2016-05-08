//
//  main.cpp
//  RaknetServer
//
//  Created by xuhua on 5/3/16.
//  Copyright © 2016 xuhua. All rights reserved.
//

#include "main.h"

CX_CPP_BEGIN

CX_IMPLEMENT(cxGameServer);

void cxGameServer::ThreadBegin()
{
    SetKey(Config::Alloc());
}

void cxGameServer::ThreadExit()
{
    GetKey<Config>()->Release();
}

void cxGameServer::OnNewConnect(RakNet::RakNetGUID clientId)
{
//    Config *conf = GetKey<Config>();
}

void cxGameServer::OnLost(RakNet::RakNetGUID clientId)
{
//    Config *conf = GetKey<Config>();
}

void cxGameServer::OnMessage(RakNet::RakNetGUID clientId,const cxStr *message)
{
//    Config *conf = GetKey<Config>();
}

void cxGameServer::Register(cchars aid,cchars ahost,int aport,cchars apass)
{
    try{
        pass = apass;
        id = aid;
        host = ahost;
        port = aport;
        long long time = (long long)cxUtil::Timestamp();
        BSONBinData pb = BSONBinData(publicKey,cat::EasyHandshake::PUBLIC_KEY_BYTES,BinDataGeneral);
        BSONBinData pk = BSONBinData(privateKey,cat::EasyHandshake::PRIVATE_KEY_BYTES,BinDataGeneral);
        BSONObj d = BSON("_id" << id
                         << "host" << host
                         << "port" << port
                         << "pass" << pass
                         << "time" << time
                         << "public" << pb
                         << "private" << pk);
        BSONObj q = BSON("_id" << id);
        conn->update(NS_SERVERS, q , d, true);
    }catch(DBException &e){
        CX_ERROR("Register server error:%s",e.getInfo().toString().c_str());
        exit(1);
    }
}

void cxGameServer::updateServerStatus(uv_timer_t* handle)
{
    cxGameServer *server = (cxGameServer *)handle->data;
    Config *conf = server->GetKey<Config>();
    try{
        long long time = (long long)cxUtil::Timestamp();
        BSONObj q = BSON("_id" << server->id);
        BSONObj d = BSON("time" << time);
        conf->Conn()->update(NS_SERVERS, q, BSON("$set" << d));
    }catch(DBException &e){
        CX_ERROR("update server status error :%s",e.getInfo().toString().c_str());
    }
    uv_timer_again(handle);
}

cxGameServer::cxGameServer()
{
    //每隔10秒更新服务器状态
    uv_timer_init(Looper(), &timer);
    timer.data = this;
    uv_timer_start(&timer, updateServerStatus, UPDATE_SERVER_STATUS_TIME, UPDATE_SERVER_STATUS_TIME);
    //主线程数据库连接
    std::string errmsg;
    ConnectionString cs = ConnectionString::parse(DB_HOST, errmsg);
    if (!cs.isValid()) {
        CX_ERROR("db host error");
        exit(1);
    }
    conn = cs.connect(errmsg);
    if(conn == nullptr){
        CX_ERROR("conn server error :%s",errmsg.c_str());
        exit(1);
    }
}

cxGameServer::~cxGameServer()
{
    delete conn;
}

extern "C" int main(int argc, const char * argv[])
{
    mongo::client::GlobalInstance instance;
    if(!instance.initialized()){
        CX_ERROR("mongo init error");
        exit(1);
    }
    cxGameServer *server = cxGameServer::Alloc();
    if(!server->Init(5, 9000, 512, "123")){
        exit(1);
    }
    server->Register("test", "192.168.199.244", 9000, "123");
    server->Run();
    return 0;
}

CX_CPP_END


