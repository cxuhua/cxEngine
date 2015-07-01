//
//  cxNotice.cpp
//  cxEngineCore
//
//  Created by xuhua on 6/14/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//


#include "cxNotice.h"

CX_CPP_BEGIN

CX_IMPLEMENT(cxNotice);

cxNotice cxNotice::instance = cxNotice();

cxNotice::Notice::Notice():dst(nullptr),func(nullptr)
{
    
}

cxNotice::Notice::~Notice()
{
    
}

void cxNotice::Remove(cxObject *dst)
{
    CX_ASSERT(dst != nullptr, "args error");
    Keys::iterator kt = instance.keys.begin();
    while (kt != instance.keys.end()) {
        for(Notices::iterator it=kt->second.begin();it!=kt->second.end();){
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

void cxNotice::Post(cxLong key,cxAny src)
{
    Keys::iterator it = instance.keys.find(key);
    if(it == instance.keys.end()){
        return;
    }
    Notices &vs = it->second;
    for(Notices::iterator vt=vs.begin();vt!=vs.end();vt++){
        (*vt).func((*vt).dst,src);
    }
}

void cxNotice::Append(cxLong key,cxObject *dst,NoticeFunc func)
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
    instance.keys.emplace(key,notices);
}

cxNotice::cxNotice()
{
    
}

cxNotice::~cxNotice()
{
    
}

CX_CPP_END

