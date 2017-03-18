//
//  cxUdpHost.cpp
//  udp
//
//  Created by xuhua on 17/03/2017.
//  Copyright © 2017 xuhua. All rights reserved.
//


#include "cxUdpBase.h"
#include "cxUdpHost.h"

CX_CPP_BEGIN

CX_IMPLEMENT(cxUdpHost);

cxUdpHost::cxUdpHost()
{
    seq = 0;
    Reset();
    base = nullptr;
}

cxUdpHost::~cxUdpHost()
{
    
}

cxUInt32 cxUdpHost::SeqInc()
{
    mutex.Lock();
    seq ++;
    mutex.Unlock();
    return seq;
}

void cxUdpHost::Reset()
{
    isactived = false;
    isclosed = true;
    uptime = 0;
    ping = -1;
}

cxInt cxUdpHost::WriteData(const cxStr *data)
{
    CX_ASSERT(base != nullptr, "base not set");
    if(!IsActived()){
        return -1;
    }
    SeqInc();
    return base->WriteData(Addr(), seq, uid, data);
}

cxBool cxUdpHost::IsClosed()
{
    return isclosed;
}

cxBool cxUdpHost::IsActived()
{
    return isactived;
}

cxBool cxUdpHost::CheckClosed(cxUInt64 time)
{
    if(uptime > 0 && !isclosed && time - uptime > 3000000L){
        isclosed = true;
        isactived = false;
        uptime = 0;
    }
    return IsClosed();
}

cxBool cxUdpHost::CheckActived(cxUInt64 time)
{
    if(!isactived){
        isactived = true;
        isclosed = false;
    }
    return IsActived();
}

void cxUdpHost::UpdatePing(cxUInt64 time,cxUInt64 value)
{
    uptime = time;
    ping =  (cxInt)value;
}

void cxUdpHost::UpdateTime(cxUInt64 time)
{
    uptime = time;
}

UdpAddr *cxUdpHost::Addr()
{
    return &addr;
}

cxUInt64 cxUdpHost::UID()
{
    return uid;
}

cxBool cxUdpHost::Init(cxUdpBase *pb,cchars ip,cxInt port,cxUInt64 id)
{
    struct sockaddr_in in;
    cxInt ret = uv_ip4_addr(ip, port, &in);
    CX_ASSERT(ret == 0, "iv ip4 addr error:%s",uv_err_name(ret));
    if(ret != 0){
        return false;
    }
    return Init(pb, (struct sockaddr *)&in, id);
}

cxBool cxUdpHost::Init(cxUdpBase *pb,const UdpAddr *paddr,cxUInt64 id)
{
    uid = id;
    base = pb;
    addr = *paddr;
    return true;
}

CX_CPP_END

