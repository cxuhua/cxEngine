//
//  cxPackage.h
//  godwar-client
//
//  Created by xuhua on 5/27/16.
//  Copyright © 2016 xuhua. All rights reserved.
//

#ifndef godwar_client_cxPackage_h
#define godwar_client_cxPackage_h

#include <core/cxObject.h>
#include <ext/xxhash.h>

CX_CPP_BEGIN

#pragma pack(1)

struct cxPackHeader {
    cxUInt32 Hash;  //校验hash
    cxUInt16 Size;  //包大小
    cxUInt16 Type;  //类型
    cxUInt16 Ping;  //发送ping值(ms)
    template<class T>
    T *To()
    {
        return (T *)this;
    }
    template<class T>
    const T *To() const
    {
        return (const T *)this;
    }
};

//数据校验
cxBool IsValidHeader(cxPackHeader *header,cxInt size);
//设置hash校验值
void SetHashHeader(cxPackHeader *header);

template <class T,int typeId>
struct cxPackTemplate {
    static const cxInt TypeId = typeId;
    cxPackTemplate()
    {
        Header.Size = sizeof(cxPackTemplate);
        Header.Type = typeId;
    }
    cxPackTemplate(cxPackTemplate &p)
    {
        Init(p);
    }
    cxInt Size()
    {
        return Header.Size;
    }
    cxAny Buffer()
    {
        return (cxAny)this;
    }
    cxPackHeader *GetHeader()
    {
        return (cxPackHeader *)this;
    }
    void Init(cxAny data,cxInt size)
    {
        memcpy(Buffer(), data, Size());
    }
    void Init(cxPackTemplate &p)
    {
        memcpy(Buffer(), p.Buffer(), p.Size());
    }
    cxPackHeader Header;
    T Value;
};

#pragma pack(0)

CX_CPP_END

#endif


