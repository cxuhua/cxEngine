//
//  main.c
//  cxpacket
//
//  Created by xuhua on 10/03/2017.
//  Copyright © 2017 xuhua. All rights reserved.
//

#include <unistd.h>
#include <stdio.h>
#include <ext/udp.h>
#include <errno.h>

void test_udp_data_array()
{
    // 查看内存是否一直增加
    while(true){
        struct udp_data_array a;
        udp_data_array_init(&a);
        
        struct udp_data *d1 = udp_data_new("123456", 6, 1);
        udp_data_array_append(&a, d1);
        
        struct udp_data *d3 = udp_data_new("654321", 6, 3);
        udp_data_array_append(&a, d3);
        
        struct udp_data *d2 = udp_data_new("654321", 6, 2);
        udp_data_array_append(&a, d2);
        
        udp_data_array_sort(&a);
        
        udp_data_array_free(&a);
        
        usleep(100);
    }
}

void test_host_hash()
{
    // 查看内存是否一直增加
    struct udp p;
    udp_init(&p,1);
    uint64_t uid = 1;
    while(true){
        uid++;
        udp_add_host(&p, "0.0.0.0", 9000, uid);
        udp_clear_host(&p);
        usleep(100);
    }
}

#define SERVER

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
    udp_bind(&c1, "0.0.0.0", 9977);
    udp_run(&c1);
    struct udp_host * h1 = udp_add_host(&c1, "0.0.0.0", 9988, 1);
    
    
//    udp_init(&c2, 3);
//    udp_bind(&c2, "0.0.0.0", 9978);
//    udp_run(&c2);
//    struct udp_host * h2 = udp_add_host(&c2, "0.0.0.0", 9988, 1);
    
    while(true){
        udp_host_push_write(h1, "1234567", 7);
        usleep(1000);
    }
#endif
    
//    udp_free(&s);
//    udp_free(&c);
    
    while(true){
        usleep(200000000);
    }
}
