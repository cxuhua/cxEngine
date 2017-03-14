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
#include "cxUdpBase.h"

using namespace cxengine;

cxUdpBase *udp = nullptr;

cxStr *x = nullptr;

static void work_cb(void *arg)
{
    cxInt i = 100000;
    struct sockaddr_in in;
    uv_ip4_addr("0.0.0.0", 9988, &in);
    while(i > 0){
        cxAutoPool::Start();
        cxStr *data = cxStr::Create("1234567");
//        udp->WriteFrame((struct sockaddr *)&in, data);
        x->Retain();
        cxInt t = CX_RAND_01f() * 50;
        usleep(t);
        x->Release();
        x->Retain();
        t = CX_RAND_01f() * 50;
        usleep(t);
        x->Release();
        cxAutoPool::Update();
        i--;
    }
}


int main(int argc, const char * argv[]) {
    
    udp = cxUdpBase::Alloc();
    udp->Init("0.0.0.0", 9988);
    udp->Start();
    
    x = cxStr::Alloc()->Init("sssss");
    uv_thread_t pid[80];
    for(int i=0; i < 80 ; i++){
        uv_thread_create(&pid[i], work_cb, &i);
    }
    for(int i=0; i < 80 ; i++){
        uv_thread_join(&pid[i]);
    }
    while (true) {
        udp->Update();
        usleep(1000);
    }
    return 0;
}








