//
//  udp.c
//  cxpacket
//
//  Created by xuhua on 10/03/2017.
//  Copyright © 2017 xuhua. All rights reserved.
//

#include "udp.h"

struct udp_data *udp_data_new(void *data,int size,uint seq)
{
    assert(data != NULL && size > 0 && size <= MAX_UDP_DATA_SIZE);
    struct udp_data *d = malloc(sizeof(struct udp_data) + size);
    d->size = size;
    d->seq = seq;
    d->time = 0;
    d->ack = false;
    d->ping = 0;
    d->state = DATA_STATE_INIT;
    d->data = (void *)d + sizeof(struct udp_data);
    memcpy(d->data, data, d->size);
    return d;
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
    memcpy(d->data, dv->data, dv->size);
    int ret = udp_write_addr(hv->udp, &hv->addr, d, sizeof(struct UDP_DATA) + dv->size);
    if(ret == 0){
        dv->state = DATA_STATE_SEND;
        dv->time = time_now();
    }
    return ret;
}

void udp_data_free(struct udp_data *d)
{
    free(d);
}

static void utarray_udp_data_dtor(void *elt)
{
    struct udp_data **eltc = (struct udp_data **)elt;
    udp_data_free(*eltc);
}

static const UT_icd ut_udp_data_icd = {sizeof(char*),NULL,NULL,utarray_udp_data_dtor};

void udp_data_array_init(struct udp_data_array *a)
{
    assert(a != NULL);
    a->ds = NULL;
    utarray_new(a->ds, &ut_udp_data_icd);
    uv_rwlock_init(&a->mutex);
}

static int udp_data_sort_func(const void *src,const void *dst)
{
    struct udp_data **d1 = (struct udp_data **)src;
    struct udp_data **d2 = (struct udp_data **)dst;
    return (*d1)->seq - (*d2)->seq;
}

void udp_data_array_sort(struct udp_data_array *a)
{
    assert(a != NULL);
    uv_rwlock_wrlock(&a->mutex);
    utarray_sort(a->ds, udp_data_sort_func);
    uv_rwlock_wrunlock(&a->mutex);
}

int udp_data_array_size(struct udp_data_array *a)
{
    assert(a != NULL);
    uv_rwlock_rdlock(&a->mutex);
    int size = utarray_len(a->ds);
    uv_rwlock_rdunlock(&a->mutex);
    return size;
}

void udp_data_array_rdlock(struct udp_data_array *a)
{
    assert(a != NULL);
    uv_rwlock_rdlock(&a->mutex);
}

void udp_data_array_rdunlock(struct udp_data_array *a)
{
    assert(a != NULL);
    uv_rwlock_rdunlock(&a->mutex);
}

void udp_data_array_wrlock(struct udp_data_array *a)
{
    assert(a != NULL);
    uv_rwlock_wrlock(&a->mutex);
}

void udp_data_array_wrunlock(struct udp_data_array *a)
{
    assert(a != NULL);
    uv_rwlock_wrunlock(&a->mutex);
}

struct udp_data **udp_data_array_next(struct udp_data **e,struct udp_data_array *a)
{
    assert(a != NULL);
    return (struct udp_data **)utarray_next(a->ds, e);
}

void udp_data_array_clear(struct udp_data_array *a)
{
    assert(a != NULL);
    uv_rwlock_wrlock(&a->mutex);
    utarray_clear(a->ds);
    uv_rwlock_wrunlock(&a->mutex);
}

void udp_data_array_append(struct udp_data_array *a,struct udp_data *v)
{
    assert(a != NULL && v != NULL);
    uv_rwlock_wrlock(&a->mutex);
    utarray_push_back(a->ds, &v);
    uv_rwlock_wrunlock(&a->mutex);
}

void udp_data_array_free(struct udp_data_array *a)
{
    assert(a != NULL);
    uv_rwlock_destroy(&a->mutex);
    utarray_free(a->ds);
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
    uv_mutex_lock(&host->mutex);
    bool active = host->uptime > 0 && now - host->uptime <= PING_TIMEOUT;
    uv_mutex_unlock(&host->mutex);
    return active;
}

void udp_host_uptime(struct udp_host *host,uint64_t time, int ping)
{
    uv_mutex_lock(&host->mutex);
    host->uptime = time_now();
    if(time > 0 && time > host->prev){
        host->ping = (int)ping;
        host->prev = time;
    }
    uv_mutex_unlock(&host->mutex);
}

void udp_host_reset(struct udp_host *host)
{
    host->closed = true;
    host->actived = false;
    host->ping = 0;
    host->uptime = 0;
    host->prev = 0;
    host->wseq = 0;
    host->send_num = 0;
    host->init_num = 0;
    host->ack_num = 0;
    udp_data_array_clear(&host->rb);
    udp_data_array_clear(&host->wb);
}

void udp_host_active(struct udp_host *host)
{
    uv_mutex_lock(&host->mutex);
    if(!host->actived){
        UDP_LOG("ACTIVE UDP %llu",host->uid);
        host->actived = true;
        host->closed = false;
    }
    uv_mutex_unlock(&host->mutex);
}

void udp_host_closed(struct udp_host *host)
{
    uv_mutex_lock(&host->mutex);
    if(!host->closed){
        UDP_LOG("CLOSED UDP %llu",host->uid);
        udp_host_reset(host);
    }
    uv_mutex_unlock(&host->mutex);
}

void udp_host_push_write(struct udp_host *host,void *data,int size)
{
    assert(host != NULL);
    assert(data != NULL && size > 0 && size <= MAX_UDP_DATA_SIZE);
    bool active = false;
    uv_mutex_lock(&host->mutex);
    host->wseq++;
    active = host->actived;
    uv_mutex_unlock(&host->mutex);
    struct udp_data *v = udp_data_new(data, size, host->wseq);
    if(active){
        udp_host_write_data(host, v);
    }
    udp_data_array_append(&host->wb, v);
}

void udp_host_push_read(struct udp_host *host,struct udp_data *v)
{
    assert(host != NULL);
    assert(v != NULL);
    udp_data_array_append(&host->rb, v);
}

void udp_host_ack_data(struct udp_host *host,struct UDP_ACK *d)
{
    assert(host != NULL && d != NULL);
    struct udp_data **tmp = NULL;
    struct udp_data *data = NULL;
    udp_data_array_wrlock(&host->wb);
    while((tmp = udp_data_array_next(tmp, &host->wb)) != NULL){
        if((*tmp)->seq == d->seq){
            data = *tmp;
            data->state = DATA_STATE_ACK;
            data->ping = (int)(time_now() - data->time);
            break;
        }
    }
    udp_data_array_wrunlock(&host->wb);
    if(data == NULL){
        return;
    }
    UDP_LOG("DATA SEQ=%d ACK DATA_PING=%d HOST_PING=%d",data->seq,data->ping,host->ping);
}

void udp_host_recv_data(struct udp_host *host,struct UDP_DATA *d)
{
    assert(host != NULL && d != NULL);
    //send ack data
    struct UDP_ACK ack;
    ack.h.opt = UDP_OPT_ACK;
    ack.seq = d->seq;
    ack.time = d->h.time;
    int ret = udp_write_addr(host->udp, &host->addr, &ack, sizeof(struct UDP_ACK));
    //read data push read buf
    struct udp_data *data = udp_data_new(d->data, d->size, d->seq);
    data->time = d->h.time;
    data->state = DATA_STATE_RECV;
    data->ack = (ret == 0);
    udp_host_push_read(host, data);
    UDP_LOG("%llu RECV data %u SEQ=%d ReadBufferSize=%d",host->uid,d->size,data->seq,udp_data_array_size(&host->rb));
}

void udp_host_write_data(struct udp_host *host,struct udp_data *d)
{
    assert(host != NULL && d != NULL);
    assert(host->actived);
    udp_data_array_wrlock(&host->wb);
    udp_data_send(d, host);
    udp_data_array_wrunlock(&host->wb);
}

void udp_host_sync(struct udp_host *host)
{
    assert(host != NULL);
    //write buffer
    udp_data_array_wrlock(&host->wb);
    EACH_DATA_ARRAY(host->wb, wv){
        struct udp_data *data = *wv;
        if(data->state == DATA_STATE_INIT){
            udp_data_send(data, host);
            host->init_num ++;
        }else if(data->state == DATA_STATE_SEND){
            host->send_num ++;
        }else if(data->state == DATA_STATE_ACK){
            host->ack_num ++;
        }
    }
    udp_data_array_wrunlock(&host->wb);
    //read buffer
    udp_data_array_wrlock(&host->rb);
    EACH_DATA_ARRAY(host->rb, rv){
        struct udp_data *data = *rv;
    }
    udp_data_array_wrunlock(&host->rb);
}

void udp_host_free(struct udp_host *host)
{
    udp_data_array_free(&host->rb);
    udp_data_array_free(&host->wb);
    uv_mutex_destroy(&host->mutex);
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

int udp_ping_addr(struct udp *p,const struct sockaddr *addr)
{
    assert(p != NULL);
    struct UDP_PING d;
    d.h.opt = UDP_OPT_PING;
    return udp_write_addr(p, addr, &d, sizeof(struct UDP_PING));
}

int udp_cnt_host(struct udp *p)
{
    assert(p != NULL);
    int count = 0;
    uv_rwlock_rdlock(&p->hmutex);
    count = HASH_COUNT(p->hosts);
    uv_rwlock_rdunlock(&p->hmutex);
    return count;
}

void udp_clear_host(struct udp *p)
{
    assert(p != NULL);
    uv_rwlock_wrlock(&p->hmutex);
    struct udp_host *e = NULL;
    struct udp_host *t = NULL;
    HASH_ITER(hh, p->hosts, e, t){
        udp_host_free(e);
    }
    HASH_CLEAR(hh, p->hosts);
    uv_rwlock_wrunlock(&p->hmutex);
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
    uv_rwlock_wrlock(&p->hmutex);
    HASH_ADD(hh, p->hosts, uid, sizeof(udp_id), h);
    uv_rwlock_wrunlock(&p->hmutex);
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
    uv_rwlock_wrlock(&p->hmutex);
    HASH_ADD(hh, p->hosts, uid, sizeof(udp_id), h);
    uv_rwlock_wrunlock(&p->hmutex);
    return h;
}

struct udp_host *udp_find_host(struct udp *p,udp_id uid)
{
    assert(p != NULL && uid > 0);
    struct udp_host *h = NULL;
    uv_rwlock_rdlock(&p->hmutex);
    HASH_FIND(hh, p->hosts, &uid, sizeof(udp_id), h);
    uv_rwlock_rdunlock(&p->hmutex);
    return h;
}

int udp_del_host(struct udp *p,udp_id uid)
{
    struct udp_host *h = udp_find_host(p, uid);
    if(h == NULL){
        return udp_cnt_host(p);
    }
    uv_rwlock_wrlock(&p->hmutex);
    HASH_DEL(p->hosts, h);
    udp_host_free(h);
    int count = HASH_COUNT(p->hosts);
    uv_rwlock_wrunlock(&p->hmutex);
    return count;
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
    switch (data[0]) {
        case UDP_OPT_PING:{
            DEFINE_VALUE(UDP_PING);
            struct UDP_PONG d;
            d.h.opt = UDP_OPT_PONG;
            d.time = v->h.time;
            udp_write_addr(p, addr, &d, sizeof(struct UDP_PONG));
            struct udp_host *h = udp_add_host_with_addr(p, addr, v->h.uid);
            assert(h != NULL);
            udp_host_uptime(h,0,0);
            break;
        }
        case UDP_OPT_PONG:{
            DEFINE_VALUE(UDP_PONG);
            int ping = (int)(time_now() - v->time);
            struct udp_host *h = udp_add_host_with_addr(p, addr,v->h.uid);
            assert(h != NULL);
            udp_host_uptime(h, v->h.time, ping);
            break;
        }
        case UDP_OPT_DATA:{
            DEFINE_VALUE(UDP_DATA);
            struct udp_host *h = udp_find_host(p, v->h.uid);
            if(h != NULL){
                udp_host_uptime(h,0,0);
                udp_host_recv_data(h, v);
            }
            break;
        }
        case UDP_OPT_ACK:{
            DEFINE_VALUE(UDP_ACK);
            int ping = (int)(time_now() - v->time);
            struct udp_host *h = udp_find_host(p, v->h.uid);
            if(h != NULL){
                udp_host_uptime(h,v->h.time,ping);
                udp_host_ack_data(h, v);
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
    uv_mutex_lock(&p->mutex);
    int ret = uv_udp_send(req, &p->handle, &buf, 1, addr, udp_send_cb);
    uv_mutex_unlock(&p->mutex);
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

static void uv_opt_work_update_host(struct udp *p,struct udp_host *h)
{
    if(udp_host_is_active(h)){
        udp_host_active(h);
    }else{
        udp_host_closed(h);
    }
}

static void uv_opt_work_ping(struct udp *p)
{
    uv_rwlock_rdlock(&p->hmutex);
    struct udp_host *e = NULL;
    struct udp_host *t = NULL;
    HASH_ITER(hh, p->hosts, e, t){
        udp_ping_addr(p, &e->addr);
        uv_opt_work_update_host(p, e);
    }
    uv_rwlock_rdunlock(&p->hmutex);
}

static void uv_opt_work_write(struct udp *p)
{
    uv_rwlock_rdlock(&p->hmutex);
    struct udp_host *e = NULL;
    struct udp_host *t = NULL;
    HASH_ITER(hh, p->hosts, e, t){
        uv_mutex_lock(&e->mutex);
        if(e->actived){
            udp_host_sync(e);
        }
        uv_mutex_unlock(&e->mutex);
    }
    uv_rwlock_rdunlock(&p->hmutex);
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

static void timer_cb(uv_timer_t* handle)
{
    struct udp *p = handle->data;
    udp_run_opt_work(p, WORK_OPT_PING);
}

static void writer_cb(uv_timer_t* handle)
{
    struct udp *p = handle->data;
    udp_run_opt_work(p, WORK_OPT_WRITE);
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
    ret = uv_timer_start(&p->timer, timer_cb, PING_TIME, PING_TIME);
    if(ret != 0)return ret;
    p->port = port;
    strcpy(p->ip, ip);
    ret = uv_timer_start(&p->writer, writer_cb, WRITER_TIME, WRITER_TIME);
    if(ret != 0)return ret;
    return ret;
}

bool udp_equ_host(struct udp *p1,struct udp_host *p2)
{
    assert(p1 != NULL && p2 != NULL);
    return p1->uid == p2->uid;
}

void udp_init(struct udp *p,udp_id uid)
{
    assert(uid > 0);
    int ret = 0;
    p->closed = false;
    p->uid = uid;
    p->ip[0] = 0;
    p->port = 0;
    p->hosts = NULL;
    p->bufsiz = 1024;
    p->buffer = malloc(p->bufsiz);
    ret = uv_mutex_init(&p->mutex);
    assert(ret == 0);
    ret = uv_rwlock_init(&p->hmutex);
    assert(ret == 0);
    ret = uv_rwlock_init(&p->dmutex);
    assert(ret == 0);
    ret = uv_loop_init(&p->looper);
    assert(ret == 0);
    ret = uv_udp_init(&p->looper, &p->handle);
    assert(ret == 0);
    p->hints.ai_family = AF_INET;
    p->hints.ai_socktype = SOCK_DGRAM;
    p->hints.ai_protocol = IPPROTO_UDP;
    p->hints.ai_flags = 0;
    ret = uv_timer_init(&p->looper, &p->timer);
    assert(ret == 0);
    p->timer.data = p;
    ret = uv_timer_init(&p->looper, &p->writer);
    assert(ret == 0);
    p->writer.data = p;
}

void udp_close(struct udp *p)
{
    p->closed = true;
}

void udp_free(struct udp *p)
{
    uv_timer_stop(&p->timer);
    uv_timer_stop(&p->writer);
    uv_udp_recv_stop(&p->handle);
    uv_loop_close(&p->looper);
    free(p->buffer);
    uv_mutex_destroy(&p->mutex);
    uv_rwlock_destroy(&p->hmutex);
    uv_rwlock_destroy(&p->dmutex);
}
