//
//  cxUdpData.h
//  udp
//
//  Created by xuhua on 18/03/2017.
//  Copyright © 2017 xuhua. All rights reserved.
//

#ifndef udp_cxUdpData_h
#define udp_cxUdpData_h

#include <core/cxStr.h>
#include "cxUdpBase.h"

CX_CPP_BEGIN

class cxUdpData : public cxObject
{
public:
    CX_DECLARE(cxUdpData);
protected:
    explicit cxUdpData();
    virtual ~cxUdpData();
private:
    cxStr *buffer;
private:
    cxUInt32    seq;
    cxUInt64    src;
    cxUInt64    time;
    cxUInt64    dst;
public:
    cxBool Init(const udp_data_t *data);
public:
    cxUInt32 Seq() const;
    cxUInt64 Src() const;
    cxUInt64 Dst() const;
    const cxStr *Data() const;
};

CX_CPP_END

#endif


