//
//  cxPackage.cpp
//  godwar-client
//
//  Created by xuhua on 5/27/16.
//  Copyright © 2016 xuhua. All rights reserved.
//


#include "cxPackage.h"

CX_CPP_BEGIN

cxBool IsValidHeader(cxPackHeader *header,cxInt size)
{
    if(header->Size != size){
        return false;
    }
    cchars buf = (cchars)header;
    cxUInt32 hv = XXH32(buf + sizeof(cxUInt32), header->Size - sizeof(cxUInt32), 0);
    return hv == header->Hash;
}

void SetHashHeader(cxPackHeader *header)
{
    cchars buf = (cchars)header;
    header->Hash = XXH32(buf + sizeof(cxUInt32), header->Size - sizeof(cxUInt32), 0);
}

CX_CPP_END

