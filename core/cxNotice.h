//
//  cxNotice.h
//  cxEngineCore
//
//  Created by xuhua on 6/14/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#ifndef cxEngineCore_cxNotice_h
#define cxEngineCore_cxNotice_h

#include <functional>
#include <map>
#include <vector>
#include "cxObject.h"

CX_CPP_BEGIN

class cxNotice
{
protected:
    explicit cxNotice();
    virtual ~cxNotice();
private:
    typedef std::function<void(cxAny data)> NoticeFunc;
    struct Notice
    {
        cxObject *dst;
        NoticeFunc func;
        Notice();
        ~Notice();
    };
    static cxNotice instance;
    typedef std::vector<Notice> Notices;
    typedef std::map<cxLong, Notices> Keys;
    Keys keys;
public:
    //must invoke remove(object)
    static void Append(cxLong key,cxObject *dst,NoticeFunc func);
    static void Post(cxLong key,cxAny src);
    static void Remove(cxLong key);
    static void Remove(cxObject *dst);
};

CX_CPP_END

#endif


