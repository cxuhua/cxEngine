//
//  packet.c
//  cxpacket
//
//  Created by xuhua on 10/03/2017.
//  Copyright © 2017 xuhua. All rights reserved.
//

#include <sys/time.h>
#include "packet.h"

#define MD5_KEY "&3847^^dfdfdKNKN"

uint64_t time_now()
{
    struct timeval val = {0};
    gettimeofday(&val, NULL);
    return (uint64_t)val.tv_sec * 1000000UL + (uint64_t)val.tv_usec;
}

void packet_attach_sum(void *data,int size)
{
    assert(data != NULL && size > sizeof(UDP_SUM));
    int len = size - sizeof(UDP_SUM);
    void *ptr = data + len;
    mongo_md5_state_t state;
    mongo_md5_init(&state);
    mongo_md5_append(&state, data, len);
    mongo_md5_append(&state, (mongo_md5_byte_t *)MD5_KEY, 16);
    mongo_md5_finish(&state, ptr);
}

bool packet_check_sum(void *data,int size)
{
    assert(data != NULL);
    UDP_SUM sum;
    if(size <= sizeof(UDP_SUM)){
        return false;
    }
    int len = size - sizeof(UDP_SUM);
    void *ptr = data + len;
    mongo_md5_state_t state;
    mongo_md5_init(&state);
    mongo_md5_append(&state, data, len);
    mongo_md5_append(&state, (mongo_md5_byte_t *)MD5_KEY, 16);
    mongo_md5_finish(&state, &sum[0]);
    return memcmp(&sum[0], ptr, sizeof(UDP_SUM)) == 0;
}


