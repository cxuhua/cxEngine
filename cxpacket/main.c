//
//  main.c
//  cxpacket
//
//  Created by xuhua on 10/03/2017.
//  Copyright © 2017 xuhua. All rights reserved.
//

#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#ifdef REMOTE
#include "udp.h"
#else
#include <ext/udp.h>
#endif

#define SERVER

static bool client_on_data_cb(struct udp_host *h,struct udp_data *d)
{
    UDP_LOG("UID=%llu RECV=%llu",h->udp->uid,d->uid);
    return true;
}
struct udp s,c1,c2;
int main(int argc, const char * argv[])
{
#ifdef SERVER
    // server
    udp_init(&s,1);
    udp_bind(&s, "0.0.0.0", 9988);
    udp_run(&s);
#endif
    
#ifdef CLIENT
    // client
    udp_init(&c1,2);
    c1.on_data = client_on_data_cb;
    udp_bind(&c1, "0.0.0.0", 9977);
    udp_run(&c1);
//    struct udp_host * h1 = udp_add_host(&c1, "139.196.203.217", 9988, 1);
    struct udp_host * h1 = udp_add_host(&c1, "0.0.0.0", 9988, 1);
    
    
    udp_init(&c2, 3);
    c2.on_data = client_on_data_cb;
    udp_bind(&c2, "0.0.0.0", 9978);
    udp_run(&c2);
//    struct udp_host * h2 = udp_add_host(&c2, "139.196.203.217", 9988, 1);
    struct udp_host * h2 = udp_add_host(&c2, "0.0.0.0", 9988, 1);
    
    while(true){
        usleep(10000);
        udp_host_push_write(h2, "1111", 5, 0, DATA_ATTR_BROADCAST);
//        usleep(1000000);
//        udp_host_push_write(h2, "2222", 5, 0);
    }
#endif
    
//    udp_free(&s);
//    udp_free(&c);
    
    while(true){
        usleep(200000000);
    }
}
