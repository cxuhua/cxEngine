//
//  packet.c
//  cxpacket
//
//  Created by xuhua on 10/03/2017.
//  Copyright © 2017 xuhua. All rights reserved.
//

#include "packet.h"

void *cx_packet_malloc(size_t size)
{
    return malloc(size);
}

void *cx_packet_realloc(void *ptr,size_t size)
{
    return realloc(ptr, size);
}

void cx_packet_free(void *ptr)
{
    free(ptr);
}

void cx_packet_init()
{
    
}
