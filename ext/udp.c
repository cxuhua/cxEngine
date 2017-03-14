//
//  udp.c
//  cxpacket
//
//  Created by xuhua on 10/03/2017.
//  Copyright © 2017 xuhua. All rights reserved.
//

#include "udp.h"

struct udp_data *udp_data_new(void *data,int size,uint seq,uint64_t uid,uint8_t attr)
{
    assert(data != NULL && size > 0 && size <= MAX_UDP_DATA_SIZE);
    struct udp_data *d = malloc(sizeof(struct udp_data) + size);
    d->size = size;
    d->seq = seq;
    d->attr = attr;
    d->uid = uid;
    d->src = 0;
    d->time = time_now();
    d->ack = false;
    d->state = DATA_STATE_INIT;
    d->data = (void *)d + sizeof(struct udp_data);
    d->trynum = TRY_COUNT;
    memcpy(d->data, data, d->size);
    return d;
}

struct udp_data *udp_data_clone(struct udp_data *dv)
{
    return udp_data_new(dv->data, dv->size, dv->seq, dv->uid,dv->attr);
}

void udp_data_try(struct udp_data *dv)
{
    assert(dv != NULL);
    dv->state = DATA_STATE_INIT;
}

int udp_data_send(struct udp_data *dv,struct udp_host *hv)
{
    assert(dv != NULL && hv != NULL && hv->actived);
    assert(dv->state == DATA_STATE_INIT);
    char buffer[1024];
    struct UDP_DATA *d = (struct UDP_DATA *)buffer;
    d->h.opt = UDP_OPT_DATA;
    d->seq = dv->seq;
    d->size = dv->size;
    d->uid = dv->uid;
    d->attr = dv->attr;
    memcpy(d->data, dv->data, dv->size);
    int ret = udp_write_addr(hv->udp, &hv->addr, d, sizeof(struct UDP_DATA) + dv->size);
    if(ret == 0){
        dv->state = DATA_STATE_SEND;
        dv->time = time_now();
        dv->trynum--;
    }
    return ret;
}

void udp_data_free(struct udp_data *d)
{
    free(d);
}

void udp_data_array_init(struct udp_data_array *a)
{
    assert(a != NULL);
    uv_mutex_init(&a->mutex);
    a->items = NULL;
}

void udp_data_array_lock(struct udp_data_array *a)
{
    assert(a != NULL);
    uv_mutex_lock(&a->mutex);
}

void udp_data_array_unlock(struct udp_data_array *a)
{
    assert(a != NULL);
    uv_mutex_unlock(&a->mutex);
}

int udp_data_array_size(struct udp_data_array *a)
{
    assert(a != NULL);
    uv_mutex_lock(&a->mutex);
    int size = HASH_COUNT(a->items);
    uv_mutex_unlock(&a->mutex);
    return size;
}

struct udp_data *udp_data_array_find(struct udp_data_array *a,uint32_t seq)
{
    assert(a != NULL);
    struct udp_data *item = NULL;
    uv_mutex_lock(&a->mutex);
    HASH_FIND(hh, a->items, &seq, sizeof(uint32_t), item);
    uv_mutex_unlock(&a->mutex);
    return item;
}

void udp_data_array_remove(struct udp_data_array *a,uint32_t seq)
{
    assert(a != NULL);
    struct udp_data *item = NULL;
    uv_mutex_lock(&a->mutex);
    HASH_FIND(hh, a->items, &seq, sizeof(uint32_t), item);
    if(item != NULL){
        HASH_DEL(a->items, item);
        udp_data_free(item);
    }
    uv_mutex_unlock(&a->mutex);
}

void udp_data_array_clear(struct udp_data_array *a)
{
    assert(a != NULL);
    uv_mutex_lock(&a->mutex);
    struct udp_data *e = NULL;
    struct udp_data *t = NULL;
    HASH_ITER(hh, a->items, e, t){
        HASH_DEL(a->items, e);
        udp_data_free(e);
    }
    uv_mutex_unlock(&a->mutex);
}

void udp_data_array_append(struct udp_data_array *a,struct udp_data *v)
{
    assert(a != NULL && v != NULL);
    uv_mutex_lock(&a->mutex);
    HASH_ADD(hh, a->items, seq, sizeof(uint32_t), v);
    uv_mutex_unlock(&a->mutex);
}

void udp_data_array_free(struct udp_data_array *a)
{
    assert(a != NULL);
    udp_data_array_clear(a);
    uv_mutex_destroy(&a->mutex);
}

struct udp_host *udp_host_new(struct udp *udp,const struct sockaddr *addr,udp_id uid)
{
    struct udp_host *host = malloc(sizeof(struct udp_host));
    uv_mutex_init(&host->mutex);
    host->addr = *addr;
    host->uid = uid;
    host->udp = udp;
    udp_data_array_init(&host->rb);
    udp_data_array_init(&host->wb);
    udp_host_reset(host);
    return host;
}

bool udp_host_is_active(struct udp_host *host)
{
    uint64_t now = time_now();
    bool active = host->uptime > 0 && now - host->uptime <= PING_TIMEOUT;
    return active;
}

void udp_host_clear(struct udp_host *host)
{
    udp_data_array_clear(&host->wb);
    udp_data_array_clear(&host->rb);
}

uint64_t udp_host_now(struct udp_host *host)
{
    assert(host != NULL);
    return time_now() + host->subtime - host->ping/2;
}

void udp_host_reset(struct udp_host *host)
{
    host->sendnum = 0;
    host->recvnum = 0;
    host->subtime = 0;
    host->closed = true;
    host->actived = false;
    host->ping = 0;
    host->uptime = 0;
    host->prev = 0;
    host->wseq = 0;
    host->pseq = 0;
    udp_host_clear(host);
}

void udp_host_active(struct udp_host *host)
{
    if(!host->actived){
        UDP_LOG("ACTIVE UDP %llu",host->uid);
        host->actived = true;
        host->closed = false;
    }
}

void udp_host_closed(struct udp_host *host)
{
    if(!host->closed){
        UDP_LOG("CLOSED UDP %llu",host->uid);
        udp_host_reset(host);
    }
}

struct udp_data *udp_host_push(struct udp_host *host,struct udp_data *data)
{
    return udp_host_push_write(host, data->data, data->size, data->src, data->attr);
}

struct udp_data *udp_host_push_write(struct udp_host *host,void *data,int size,uint64_t uid,uint8_t attr)
{
    assert(host != NULL);
    assert(data != NULL && size > 0 && size <= MAX_UDP_DATA_SIZE);
    uv_mutex_lock(&host->mutex);
    host->wseq++;
    struct udp_data *v = udp_data_new(data, size, host->wseq, uid, attr);
    uv_mutex_unlock(&host->mutex);
    udp_data_array_append(&host->wb, v);
    return v;
}

void udp_host_push_read(struct udp_host *host,struct udp_data *v)
{
    assert(host != NULL);
    assert(v != NULL);
    udp_data_array_append(&host->rb, v);
}

void udp_host_resend_data(struct udp_host *host,struct UDP_RESEND *d)
{
    assert(false);
}

void udp_host_ack_data(struct udp_host *host,struct UDP_ACK *d)
{
    assert(host != NULL && d != NULL);
    struct udp_data *data = udp_data_array_find(&host->wb, d->seq);
    if(data == NULL){
        return;
    }
    data->state = DATA_STATE_ACK;
}

void udp_host_recv_data(struct udp_host *host,struct UDP_DATA *d)
{
    assert(host != NULL && d != NULL);
    struct udp_data *data = udp_data_array_find(&host->rb, d->seq);
    if(data != NULL && data->state == DATA_STATE_DROP){
        return;
    }
    //send ack data
    int ret = 0;
    struct UDP_ACK ack;
    ack.h.opt = UDP_OPT_ACK;
    ack.seq = d->seq;
    ack.time = d->h.time;
    ret = udp_write_addr(host->udp, &host->addr, &ack, sizeof(struct UDP_ACK));
    //read data push read buf
    data = udp_data_new(d->data, d->size, d->seq, d->uid,d->attr);
    data->time = time_now();
    data->state = DATA_STATE_RECV;
    data->ack = (ret == 0);
    data->src = d->h.uid;
    udp_host_push_read(host, data);
}

static bool is_try_send(struct udp_data *d,struct udp_host *h)
{
    if(h->ping == 0){
        return false;
    }
    uint64_t now = time_now();
    float t = (float)(now - d->time)/1000.0f;//ms
    float p = (float)h->ping/1000.0f;//ms
    return t > p * 2.0f;
}

static bool is_del_data(struct udp_data *d,struct udp_host *h)
{
    uint64_t now = time_now();
    return (now - d->time) > PING_TIMEOUT;
}

static int udp_data_sort(struct udp_data *d1,struct udp_data *d2)
{
    return d1->seq - d2->seq;
}

void udp_host_sync(struct udp_host *host)
{
    assert(host != NULL);
    //write buffer
    struct udp_data *e = NULL,*t = NULL;
    udp_data_array_lock(&host->wb);
    HASH_ITER(hh, host->wb.items, e, t){
        if(is_del_data(e,host)){
            e->state = DATA_STATE_DROP;
            UDP_LOG("DATA TIMEOUT DROP %d",e->seq);
        }
        if(e->state == DATA_STATE_INIT && host->actived){
            if(e->trynum > 0){
                udp_data_send(e, host);
            }else{
                e->state = DATA_STATE_DROP;
                UDP_LOG("DATA TRY COUNT DROP %d",e->seq);
            }
        }else if(e->state == DATA_STATE_SEND){
            if(is_try_send(e,host)){
                UDP_LOG("TRY SEND SEQ=%u",e->seq);
                udp_data_try(e);
            }
        }else if(e->state == DATA_STATE_ACK){
            host->sendnum++;
            e->state = DATA_STATE_DROP;
        }
        if(e->state == DATA_STATE_DROP){
            HASH_DEL(host->wb.items, e);
            udp_data_free(e);
        }
    }
    udp_data_array_unlock(&host->wb);
    e = NULL;t = NULL;
    udp_data_array_lock(&host->rb);
    HASH_SORT(host->rb.items, udp_data_sort);
    HASH_ITER(hh, host->rb.items, e, t){
        if(is_del_data(e,host)){
            HASH_DEL(host->rb.items, e);
            udp_data_free(e);
            continue;
        }
        if(e->state != DATA_STATE_RECV){
            continue;
        }
        bool ret = true;
        int sub = host->pseq + 1 - e->seq;
        if(sub != 0){
            UDP_LOG("DATA NOT CONTINUE %d",sub);
        }
        host->pseq = e->seq;
        if(host->udp->on_data != NULL){
            ret = host->udp->on_data(host,e);
        }
        if(ret){
            host->recvnum++;
            e->state = DATA_STATE_DROP;
        }
    }
    udp_data_array_unlock(&host->rb);
}


void udp_host_free(struct udp_host *host)
{
    uv_mutex_destroy(&host->mutex);
    udp_data_array_free(&host->rb);
    udp_data_array_free(&host->wb);
    free(host);
}

void udp_run_default(struct udp *p)
{
    uv_run(&p->looper, UV_RUN_DEFAULT);
}

void udp_run_nowait(struct udp *p)
{
    uv_run(&p->looper, UV_RUN_NOWAIT);
}

static void udp_run_entry(void *arg)
{
    struct udp *p = arg;
    udp_run_default(p);
}

void udp_run(struct udp *p)
{
    uv_thread_create(&p->pid, udp_run_entry, p);
}

static void udp_alloc_cb(uv_handle_t* handle,size_t suggested,uv_buf_t* buf)
{
    struct udp *p = handle->data;
    if(suggested > p->bufsiz){
        p->buffer = realloc(p->buffer,suggested);
        p->bufsiz = (int)suggested;
    }
    buf->base = p->buffer;
    buf->len = p->bufsiz;
}

int udp_ping_host(struct udp *p,struct udp_host *h)
{
    assert(p != NULL);
    struct UDP_PING d;
    d.h.opt = UDP_OPT_PING;
    d.ping = h->ping;
    return udp_write_addr(p, &h->addr, &d, sizeof(struct UDP_PING));
}

int udp_cnt_host(struct udp *p)
{
    assert(p != NULL);
    int count = 0;
    count = HASH_COUNT(p->hosts);
    return count;
}

void udp_clear_host(struct udp *p)
{
    assert(p != NULL);
    struct udp_host *e = NULL;
    struct udp_host *t = NULL;
    HASH_ITER(hh, p->hosts, e, t){
        udp_host_free(e);
    }
    HASH_CLEAR(hh, p->hosts);
}

struct udp_host *udp_add_host_with_addr(struct udp *p,const struct sockaddr *addr,udp_id uid)
{
    assert(p != NULL && uid > 0);
    struct udp_host *h = udp_find_host(p, uid);
    if(h != NULL){
        h->addr = *addr;
        return h;
    }
    h = udp_host_new(p, addr, uid);
    assert(!udp_equ_host(p, h));
    HASH_ADD(hh, p->hosts, uid, sizeof(udp_id), h);
    return h;
}

struct udp_host *udp_add_host(struct udp *p,char *ip,int port,udp_id uid)
{
    assert(p != NULL && uid > 0);
    struct sockaddr_in in;
    uv_ip4_addr(ip, port, &in);
    struct sockaddr *addr = (struct sockaddr *)&in;
    struct udp_host *h = udp_find_host(p, uid);
    if(h != NULL){
        h->addr = *addr;
        return h;
    }
    h = udp_host_new(p, addr, uid);
    assert(!udp_equ_host(p, h));
    HASH_ADD(hh, p->hosts, uid, sizeof(udp_id), h);
    return h;
}

struct udp_host *udp_find_host(struct udp *p,udp_id uid)
{
    assert(p != NULL && uid > 0);
    struct udp_host *h = NULL;
    HASH_FIND(hh, p->hosts, &uid, sizeof(udp_id), h);
    return h;
}

int udp_del_host(struct udp *p,udp_id uid)
{
    struct udp_host *h = udp_find_host(p, uid);
    if(h == NULL){
        return udp_cnt_host(p);
    }
    HASH_DEL(p->hosts, h);
    udp_host_free(h);
    return HASH_COUNT(p->hosts);
}

#define DEFINE_VALUE(_t_) struct _t_ *v = (struct _t_ *)data

static void udp_recv_cb(uv_udp_t* handle,ssize_t nread,const uv_buf_t* buf,const struct sockaddr* addr,unsigned flags)
{
    struct udp *p = handle->data;
    if(nread <= 0){
        return;
    }
    int datasiz = (int)nread;
    uint8_t *data = (uint8_t *)buf->base;
    if(!packet_check_sum(data, datasiz)){
        return;
    }
    uint64_t now = time_now();
    switch (data[0]) {
        case UDP_OPT_PING:{
            DEFINE_VALUE(UDP_PING);
            struct udp_host *h = udp_add_host_with_addr(p, addr, v->h.uid);
            assert(h != NULL);
            struct UDP_PONG d;
            d.h.opt = UDP_OPT_PONG;
            d.time = v->h.time;
            d.ping = h->ping;
            udp_write_addr(p, addr, &d, sizeof(struct UDP_PONG));
            h->uptime = now;
            if(v->ping > 0){
                h->subtime = (int)(v->h.time - now + v->ping/2);
            }
            break;
        }
        case UDP_OPT_PONG:{
            DEFINE_VALUE(UDP_PONG);
            struct udp_host *h = udp_add_host_with_addr(p, addr,v->h.uid);
            assert(h != NULL);
            int ping = (int)(time_now() - v->time);
            h->uptime = now;
            if(v->h.time > 0 && v->h.time > h->prev){
                h->ping = (int)ping;
                h->prev = v->h.time;
            }
            if(v->ping > 0){
                h->subtime = (int)(v->h.time - now + v->ping/2);
            }
            break;
        }
        case UDP_OPT_DATA:{
            DEFINE_VALUE(UDP_DATA);
            struct udp_host *h = udp_find_host(p, v->h.uid);
            if(h != NULL){
                h->uptime = now;
                udp_host_recv_data(h, v);
            }
            break;
        }
        case UDP_OPT_ACK:{
            DEFINE_VALUE(UDP_ACK);
            struct udp_host *h = udp_find_host(p, v->h.uid);
            if(h != NULL){
                h->uptime = now;
                udp_host_ack_data(h, v);
            }
            break;
        }
        case UDP_OPT_RESEND:{
            DEFINE_VALUE(UDP_RESEND);
            struct udp_host *h = udp_find_host(p, v->h.uid);
            if(h != NULL){
                h->uptime = now;
                udp_host_resend_data(h, v);
            }
            break;
        }
        default:{
            assert(false);
            break;
        }
    }
}

static void udp_send_cb(uv_udp_send_t* req, int status)
{
    free(req);
}

int udp_write_addr(struct udp *p,const struct sockaddr *addr,void *data, int size)
{
    struct UDP_HEAD *head = data;
    assert(p->uid > 0 && head->opt > 0);
    head->time = time_now();
    head->uid = p->uid;
    int dsiz = size + sizeof(UDP_SUM);
    int reqsiz = sizeof(uv_udp_send_t);
    uv_udp_send_t *req = malloc(reqsiz + dsiz);
    void *pdata = (void *)req + reqsiz;
    memcpy(pdata, data, size);
    //fill data sum data
    packet_attach_sum(pdata, dsiz);
    uv_buf_t buf = uv_buf_init(pdata,dsiz);
    int ret = uv_udp_send(req, &p->handle, &buf, 1, addr, udp_send_cb);
    if(ret != 0){
        free(req);
    }
    return ret;
}

int udp_write(struct udp *p,char *ip,int port,void *data,int size)
{
    struct sockaddr_in addr;
    uv_ip4_addr(ip, port, &addr);
    return udp_write_addr(p, ADDR(addr), data, size);
}


static void uv_opt_work_ping(struct udp *p)
{
    struct udp_host *e = NULL;
    struct udp_host *t = NULL;
    HASH_ITER(hh, p->hosts, e, t){
        udp_ping_host(p, e);
        if(udp_host_is_active(e)){
            udp_host_active(e);
        }else{
            udp_host_closed(e);
        }
    }
}

static void uv_opt_work_write(struct udp *p)
{
    struct udp_host *e = NULL;
    struct udp_host *t = NULL;
    HASH_ITER(hh, p->hosts, e, t){
        udp_host_sync(e);
    }
}

void udp_run_opt_work(struct udp *p,uint8_t opt)
{
    switch (opt) {
        case WORK_OPT_PING:{
            uv_opt_work_ping(p);
            break;
        }
        case WORK_OPT_WRITE:{
            uv_opt_work_write(p);
            break;
        }
        default:{
            assert(false);
            break;
        }
    }
}

static void udp_idle_cb(uv_idle_t* handle)
{
    struct udp *p = handle->data;
    udp_run_opt_work(p, WORK_OPT_WRITE);
    usleep(100);
}

static void udp_status(struct udp *p)
{
    struct udp_host *e=NULL,*t=NULL;
    HASH_ITER(hh, p->hosts, e, t){
        UDP_LOG("UID=%llu PING=%d SUBTIME=%d NOW=%llu WB=%d RB=%d SEND=%d RECV=%d",
                e->uid,
                e->ping,
                e->subtime,
                udp_host_now(e),
                udp_data_array_size(&e->wb),
                udp_data_array_size(&e->rb),
                e->sendnum,
                e->recvnum);
    }
}

static void udp_ping_cb(uv_timer_t* handle)
{
    struct udp *p = handle->data;
    udp_run_opt_work(p, WORK_OPT_PING);
    
//    udp_status(p);
}



int udp_bind(struct udp *p,char *ip,int port)
{
    int ret = 0;
    struct sockaddr_in addr;
    ret = uv_ip4_addr(ip, port, &addr);
    if(ret != 0)return ret;
    ret = uv_udp_bind(&p->handle, (struct sockaddr *)&addr, 0);
    if(ret != 0)return ret;
    p->handle.data = p;
    ret = uv_udp_recv_start(&p->handle, udp_alloc_cb, udp_recv_cb);
    if(ret != 0)return ret;
    ret = uv_idle_start(&p->idle, udp_idle_cb);
    if(ret != 0)return ret;
    p->port = port;
    strcpy(p->ip, ip);
    ret = uv_timer_start(&p->timer, udp_ping_cb, 0, PING_TIME);
    if(ret != 0)return ret;
    return ret;
}

bool udp_equ_host(struct udp *p1,struct udp_host *p2)
{
    assert(p1 != NULL && p2 != NULL);
    return p1->uid == p2->uid;
}

static bool server_on_data_cb(struct udp_host *h,struct udp_data *d)
{
    if(d->attr == DATA_ATTR_TO_UID){
        struct udp_host *c = udp_find_host(h->udp, d->uid);
        if(c != NULL){
            udp_host_push(c, d);
        }
        return true;
    }
    if(d->attr == DATA_ATTR_BROADCAST){
        struct udp_host *e=NULL,*t=NULL;
        HASH_ITER(hh, h->udp->hosts, e, t){
            udp_host_push(e, d);
        }
        return true;
    }
    if(d->attr == DATA_ATTR_NOT_SENDER){
        struct udp_host *e=NULL,*t=NULL;
        HASH_ITER(hh, h->udp->hosts, e, t){
            if(e->uid == h->uid){
                continue;
            }
            udp_host_push(e, d);
        }
        return true;
    }
    return true;
}

void udp_init(struct udp *p,udp_id uid)
{
    assert(uid > 0);
    int ret = 0;
    p->on_data = server_on_data_cb;
    p->closed = false;
    p->uid = uid;
    p->ip[0] = 0;
    p->port = 0;
    p->hosts = NULL;
    p->bufsiz = 1024;
    p->buffer = malloc(p->bufsiz);
    ret = uv_loop_init(&p->looper);
    assert(ret == 0);
    ret = uv_udp_init(&p->looper, &p->handle);
    assert(ret == 0);
    ret = uv_idle_init(&p->looper, &p->idle);
    assert(ret == 0);
    p->idle.data = p;
    ret = uv_timer_init(&p->looper, &p->timer);
    assert(ret == 0);
    p->timer.data = p;
    p->hints.ai_family = AF_INET;
    p->hints.ai_socktype = SOCK_DGRAM;
    p->hints.ai_protocol = IPPROTO_UDP;
    p->hints.ai_flags = 0;
}

void udp_close(struct udp *p)
{
    p->closed = true;
}

void udp_free(struct udp *p)
{
    uv_timer_stop(&p->timer);
    uv_idle_stop(&p->idle);
    uv_udp_recv_stop(&p->handle);
    uv_loop_close(&p->looper);
    free(p->buffer);
}
