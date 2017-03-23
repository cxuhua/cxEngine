//
//  main.cpp
//  TextureTool
//
//  Created by xuhua on 7/28/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#include <unistd.h>
#include <uv/uv.h>
#include <core/cxAutoPool.h>
#include <core/cxStr.h>
#include <core/cxUtil.h>
#include <udp/cxUdpServer.h>
#include <udp/cxUdpClient.h>
using namespace cxengine;

cxUdpServer *server = nullptr;
cxUdpClient *client = nullptr;

static void pull_work_data(void *arg)
{
    while(true){
        cxAutoPool::Start();
        
        server->WorkRun();
        
        cxAutoPool::Update();
        usleep(1);
    }
}

cxUdpHost *chost = nullptr;

static void update_client(void *arg)
{
    while (true) {
        cxAutoPool::Start();
        client->Update();
        cxAutoPool::Update();
        usleep(1);
    }
}

static void client_write(void *arg)
{
    while (true) {
        cxAutoPool::Start();
        if(chost->IsActived()){
            cxStr *data = cxStr::Alloc("1234");
            chost->WriteData(data,2);
            data->Release();
        }
        cxAutoPool::Update();
        usleep(100000);
    }
}

#include <iostream>

int main(int argc, const char * argv[])
{
    server = cxUdpServer::Alloc();
    server->Init("0.0.0.0", 9988, 1);
    server->Start();
    
    uv_thread_t pid2[5];
    for(int i=0; i < 5 ; i++){
        uv_thread_create(&pid2[i], pull_work_data, &i);
    }
    client = cxUdpClient::Alloc();
    client->Init("0.0.0.0", 9977, 2);
    
    chost = client->ConnectHost("0.0.0.0", 9988, 1);
    client->Start();
    client->onData +=[](cxUdpBase *udp,cxUdpHost *shost,const cxUdpData *data){
        CX_LOGGER("%llu -> %llu",data->Src(),data->Dst());
    };
    
    uv_thread_t cid;
    uv_thread_create(&cid, update_client, client);
    
    uv_thread_t wid;
    uv_thread_create(&wid, client_write, client);
    
    cxUdpClient *client2 = cxUdpClient::Alloc();
    client2->Init("0.0.0.0", 9976, 3);
    client2->ConnectHost("0.0.0.0", 9988, 1);
    client2->onData +=[](cxUdpBase *udp,cxUdpHost *shost,const cxUdpData *data){
        CX_LOGGER("%llu -> %llu",data->Src(),data->Dst());
    };
    client2->Start();
    
    while (true) {
        cxAutoPool::Start();
        client2->Update();
        server->Update();
        cxAutoPool::Update();
        usleep(1);
    }
    return 0;
}








