//
//  cxUdpData.cpp
//  udp
//
//  Created by xuhua on 18/03/2017.
//  Copyright © 2017 xuhua. All rights reserved.
//


#include "cxUdpData.h"

CX_CPP_BEGIN

CX_IMPLEMENT(cxUdpData);

cxUdpData::cxUdpData()
{
    maxtry = MAX_TRY_SEND;
    seq = 0;
    src = 0;
    time = 0;
    dst = 0;
    buffer = cxStr::Alloc();
}

cxUdpData::~cxUdpData()
{
    buffer->Release();
}

cxUInt8 cxUdpData::DecMaxTry()
{
    maxtry--;
    return maxtry;
}

cxStr *cxUdpData::Data() const
{
    return buffer;
}

cxUInt64 cxUdpData::Src() const
{
    return src;
}

cxUInt64 cxUdpData::Dst() const
{
    return dst;
}

const UdpAddr *cxUdpData::Addr() const
{
    return &addr;
}

cxUInt32 cxUdpData::Seq() const
{
    return seq;
}

cxUInt64 cxUdpData::Time()
{
    return time;
}

void cxUdpData::SetTime(cxUInt64 v)
{
    time = v;
}

cxBool cxUdpData::Init(const UdpAddr *paddr,const cxStr *data)
{
    addr = *paddr;
    buffer->Clear();
    buffer->Append(data);
    return true;
}

// init for send data
cxBool cxUdpData::Init(cxUInt32 aseq, const cxStr *data,cxUInt64 adst,cxUInt64 atime)
{
    seq = aseq;
    buffer->Clear();
    buffer->Append(data);
    time = atime;
    dst = adst;
    return true;
}

// init for recv data
cxBool cxUdpData::Init(const udp_data_t *data)
{
    buffer->Clear();
    buffer->Append((cchars)data->data, data->size);
    seq = data->seq;
    src = data->src;
    time = data->time;
    dst = data->dst;
    return true;
}

CX_CPP_END

