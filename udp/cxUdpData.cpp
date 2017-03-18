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
    src = 0;
    time = 0;
    dst = 0;
    buffer = cxStr::Alloc();
}

cxUdpData::~cxUdpData()
{
    buffer->Release();
}

const cxStr *cxUdpData::Data() const
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

cxUInt32 cxUdpData::Seq() const
{
    return seq;
}

cxBool cxUdpData::Init(const udp_data_t *data)
{
    seq = data->seq;
    buffer->Clear();
    buffer->Append((cchars)data->data, data->size);
    src = data->src;
    time = data->time;
    dst = data->dst;
    return true;
}

CX_CPP_END

