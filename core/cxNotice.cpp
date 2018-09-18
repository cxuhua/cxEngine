//
//  cxNotice.cpp
//  cxEngineCore
//
//  Created by xuhua on 6/14/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//


#include "cxNotice.h"

CX_CPP_BEGIN

cxNotice cxNotice::instance = cxNotice();

cxNotice::Notice::Notice():dst(nullptr),func(nullptr)
{
    
}

cxNotice::Notice::~Notice()
{
    
}

void cxNotice::Remove(cxAny dst)
{
    CX_ASSERT(dst != nullptr, "args error");
    Keys::iterator kt = instance.keys.begin();
    while (kt != instance.keys.end()) {
        Notices &vs = kt->second;
        Notices::iterator it=vs.begin();
        while(it != vs.end()){
            if((*it).dst == dst){
                it = kt->second.erase(it);
            }else{
                it++;
            }
        }
        if(kt->second.size() == 0){
            kt = instance.keys.erase(kt);
        }else{
            kt++;
        }
    }
}

void cxNotice::Remove(cxLong key)
{
    instance.keys.erase(key);
}

cxInt cxNotice::Post(cxLong key,cxAny src)
{
    Keys::iterator it = instance.keys.find(key);
    if(it == instance.keys.end()){
        return 0;
    }
    Notices &vs = it->second;
    for(cxInt i=0;i <vs.size(); i++){
        Notice &vt = vs.at(i);
        vt.func(src);
    }
    return (cxInt)vs.size();
}

void cxNotice::Append(cxLong key,cxAny dst,NoticeFunc func)
{
    CX_ASSERT(dst != nullptr, "dst error");
    Keys::iterator it = instance.keys.find(key);
    Notice notice;
    notice.func = func;
    notice.dst = dst;
    if(it != instance.keys.end()){
        it->second.push_back(notice);
        return;
    }
    Notices notices;
    notices.push_back(notice);
    instance.keys[key] = notices;
}

cxNotice::cxNotice()
{
    
}

cxNotice::~cxNotice()
{
    
}

CX_CPP_END

