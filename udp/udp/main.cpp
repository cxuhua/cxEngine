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
#include "cxUdpBase.h"

using namespace cxengine;

cxUdpBase *udp = nullptr;

cxStr *x = nullptr;

static void work_cb(void *arg)
{
    struct sockaddr_in in;
    uv_ip4_addr("0.0.0.0", 9988, &in);
    while(true){
        cxAutoPool::Start();
        cxStr *data = cxStr::Create("1234");
        udp->WriteFrame((struct sockaddr *)&in, data);
        cxAutoPool::Update();
        usleep(50);
    }
}


int main(int argc, const char * argv[])
{
    udp = cxUdpBase::Alloc();
    udp->Init("0.0.0.0", 9988, 1);
    udp->Start();
    
    x = cxStr::Alloc()->Init("sssss");
    uv_thread_t pid[20];
    for(int i=0; i < 20 ; i++){
        uv_thread_create(&pid[i], work_cb, &i);
    }
    while (true) {
        udp->Update();
        usleep(50);
    }
    return 0;
}








