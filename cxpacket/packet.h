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
    
void *cx_packet_malloc(size_t size);
void *cx_packet_realloc(void *ptr,size_t size);
void cx_packet_free(void *ptr);

typedef struct cx_packet_piece cx_packet_piece;
struct cx_packet_piece {
    
};

void cx_packet_init();

#endif /* packet_h */
