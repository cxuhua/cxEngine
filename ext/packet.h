//
//  packet.h
//  cxpacket
//  数据分片协议
//  Created by xuhua on 10/03/2017.
//  Copyright © 2017 xuhua. All rights reserved.
//

#ifndef packet_h
#define packet_h

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <limits.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include "md5.h"

typedef uint64_t udp_id;

#define MAX_UDP_DATA_SIZE   512

#pragma pack(1)

enum udp_opt {
    UDP_OPT_NONE,
    UDP_OPT_PING,
    UDP_OPT_PONG,
    UDP_OPT_DATA,
    UDP_OPT_ACK,
    UDP_OPT_MAX,
};

struct UDP_HEAD {
    uint8_t opt;        // send opt
    uint64_t uid;       // send uid
    uint64_t time;      // send time
};

typedef uint8_t UDP_SUM[MD5_DIGEST_LENGTH];

struct UDP_PING {
    struct UDP_HEAD h;
};

struct UDP_PONG {
    struct UDP_HEAD h;
    uint64_t time;      //sender local time
};

struct UDP_DATA {
    struct UDP_HEAD h;
    uint32_t seq;
    uint16_t size;
    uint8_t data[0];
};

struct UDP_ACK {
    struct UDP_HEAD h;
    uint32_t seq;
    uint64_t time;
};

#pragma pack()

void packet_attach_sum(void *data,int size);
bool packet_check_sum(void *data,int size);

uint64_t time_now();

#endif /* packet_h */
