//
//  udp.h
//  cxpacket
//
//  Created by xuhua on 10/03/2017.
//  Copyright © 2017 xuhua. All rights reserved.
//

#ifndef udp_h
#define udp_h

#include <unistd.h>
#include <uv/uv.h>
#include "xxhash.h"
#include "uthash.h"
#include "utarray.h"
#include "packet.h"

struct udp_host;

#define UDP_LOG(fmt,...)    printf(fmt"\n",##__VA_ARGS__)

#define MAX_SEND_REQ_SIZE   (MAX_PIECE_SIZE + 128)

#define ADDR(s) ((struct sockaddr *)&s)

#define HOST_KEY_SIZE   32

#define PING_TIMEOUT    5000000        //5000000 us

#define PING_TIME       1000           //1000 ms

#define WRITER_TIME     1              // 5ms

enum udp_data_state {
    DATA_STATE_INIT,    //new data
    DATA_STATE_SEND,    //send to remote,wait ack
    DATA_STATE_ACK,     //ack data,send success
    DATA_STATE_RECV,    //recv data
    DATA_STATE_DROP,    //process ok
    DATA_STATE_TIMEOUT, //not ack timeout
};

struct udp_data {
    uint32_t seq;
    void *data;
    int size;
    uint8_t state;
    uint64_t time;
    bool ack;   //if sended ack
    int ping;
};

struct udp_data *udp_data_new(void *data,int size,uint seq);
int udp_data_send(struct udp_data *dv,struct udp_host *hv);
void udp_data_free(struct udp_data *d);

struct udp_data_array {
    uv_rwlock_t mutex;
    UT_array *ds;
};

#define EACH_DATA_ARRAY(_v_,_e_)    struct udp_data **_e_=NULL;while((_e_ = udp_data_array_next(_e_, &(_v_))) != NULL)

void udp_data_array_init(struct udp_data_array *a);
//sort by time
void udp_data_array_sort(struct udp_data_array *a);

int udp_data_array_size(struct udp_data_array *a);

struct udp_data **udp_data_array_next(struct udp_data **e,struct udp_data_array *a);

void udp_data_array_clear(struct udp_data_array *a);

void udp_data_array_append(struct udp_data_array *a,struct udp_data *v);

void udp_data_array_free(struct udp_data_array *a);

void udp_data_array_rdlock(struct udp_data_array *a);

void udp_data_array_rdunlock(struct udp_data_array *a);

void udp_data_array_wrlock(struct udp_data_array *a);

void udp_data_array_wrunlock(struct udp_data_array *a);

struct udp_host {
    udp_id uid;
    struct udp *udp;
    struct sockaddr addr;
    uint64_t uptime;
    int ping;
    uint64_t prev;  //prev ping time
    bool actived;
    bool closed;
    uv_mutex_t mutex;
    struct udp_data_array rb;       //read buf
    uint wseq;
    struct udp_data_array wb;       //write buf
    uint send_num;
    uint init_num;
    uint ack_num;
    UT_hash_handle hh;
};

struct udp_host *udp_host_new(struct udp *udp,const struct sockaddr *addr,udp_id uid);
void udp_host_uptime(struct udp_host *host,uint64_t time,int ping);
void udp_host_reset(struct udp_host *host);
bool udp_host_is_active(struct udp_host *host);
void udp_host_closed(struct udp_host *host);
void udp_host_active(struct udp_host *host);
void udp_host_push_write(struct udp_host *host,void *data,int size);
void udp_host_push_read(struct udp_host *host,struct udp_data *v);
void udp_host_free(struct udp_host *host);
void udp_host_sync(struct udp_host *host);
void udp_host_recv_data(struct udp_host *host,struct UDP_DATA *d);
void udp_host_write_data(struct udp_host *host,struct udp_data *d);
void udp_host_ack_data(struct udp_host *host,struct UDP_ACK *d);

struct upd_work_req {
    uv_work_t req;
    struct sockaddr addr;
    int size;
};

enum work_opt {
    WORK_OPT_NONE = 0,
    WORK_OPT_PING,
    WORK_OPT_WRITE,
};

struct udp {
    udp_id uid;               //udp id
    uv_thread_t pid;            //use thread run
    uv_mutex_t mutex;         //socket write mutex
    struct udp_host *hosts;
    uv_rwlock_t hmutex;         //hosts add del mutext
    uv_rwlock_t dmutex;         //data buffer mutex
    uv_loop_t looper;
    uv_udp_t handle;
    struct addrinfo hints;
    void *buffer;
    int bufsiz;
    char ip[HOST_KEY_SIZE];
    int port;
    bool closed;
    uv_timer_t timer;
    uv_timer_t writer;
};

int udp_cnt_host(struct udp *p);
void udp_clear_host(struct udp *p);
struct udp_host *udp_add_host(struct udp *p,char *ip,int port,udp_id uid);
struct udp_host *udp_add_host_with_addr(struct udp *p,const struct sockaddr *addr,udp_id uid);
int udp_del_host(struct udp *p,udp_id uid);
struct udp_host *udp_find_host(struct udp *p,udp_id uid);

void udp_run_opt_work(struct udp *p,uint8_t opt);

int udp_ping_addr(struct udp *p,const struct sockaddr *addr);

bool udp_equ_host(struct udp *p1,struct udp_host *p2);

void udp_run_default(struct udp *p);

void udp_run_nowait(struct udp *p);

void udp_run(struct udp *p);

void udp_init(struct udp *p,udp_id uid);

int udp_bind(struct udp *p,char *ip,int port);

void udp_free(struct udp *p);

void udp_close(struct udp *p);

int udp_write_addr(struct udp *p,const struct sockaddr *addr,void *data, int size);

int udp_write(struct udp *p,char *ip,int port,void *data,int size);


#endif /* udp_h */
