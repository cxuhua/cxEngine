//
//  main.cpp
//  RaknetServer
//
//  Created by xuhua on 5/3/16.
//  Copyright © 2016 xuhua. All rights reserved.
//

#include "main.h"

CX_IMPLEMENT(cxGameServer);

void cxGameServer::ThreadBegin()
{
    SetKey(Config::Alloc());
}

void cxGameServer::ThreadExit()
{
    GetKey<Config>()->Release();
}

void cxGameServer::Register(cchars aid,cchars ahost,int aport)
{
    try{
        id = aid;
        host = ahost;
        port = aport;
        cxInt64 time = (cxInt64)cxUtil::Timestamp();
        std::string errmsg;
        ConnectionString cs = ConnectionString::parse(DB_HOST, errmsg);
        if (!cs.isValid()) {
            CX_ERROR("db host error");
            exit(1);
        }
        boost::scoped_ptr<DBClientBase> conn(cs.connect(errmsg));
        
        BSONObj d = BSONObjBuilder().append("_id",id)
        .append("host",host)
        .append("port",port)
        .append("time",time)
        .append("public",BSONBinData(publicKey,cat::EasyHandshake::PUBLIC_KEY_BYTES,BinDataGeneral))
        .append("private",BSONBinData(privateKey,cat::EasyHandshake::PRIVATE_KEY_BYTES,BinDataGeneral))
        .obj();
        
        BSONObj q = BSONObjBuilder().append("_id",id).obj();
        
        conn->update(NS_SERVERS, q , d, true);
    }catch(DBException &e){
        CX_ERROR("mongo initialized error:%s",e.getInfo().toString().c_str());
        exit(1);
    }
}

void cxGameServer::updateServerStatus(uv_timer_t* handle)
{
    cxGameServer *server = (cxGameServer *)handle->data;
    Config *conf = server->GetKey<Config>();
    try{
        cxInt64 time = (cxInt64)cxUtil::Timestamp();
        BSONObj q = BSON("_id" << server->id);
        BSONObj d = BSON("time" << time);
        conf->Conn()->update(NS_SERVERS, q, BSON("$set" << d));
    }catch(DBException &e){
        CX_ERROR("update server status error %s",e.getInfo().toString().c_str());
    }
    uv_timer_again(handle);
}

cxGameServer::cxGameServer()
{
    //每隔10秒更新服务器状态
    uv_timer_init(Looper(), &timer);
    timer.data = this;
    uv_timer_start(&timer, updateServerStatus, UPDATE_SERVER_STATUS_TIME, UPDATE_SERVER_STATUS_TIME);
}

cxGameServer::~cxGameServer()
{
    
}

int main(int argc, const char * argv[])
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
    server->Register("test", "192.168.199.244", 9000);
    server->Run();
    return 0;
}
