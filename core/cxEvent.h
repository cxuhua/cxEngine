//
//  cxEvent.h
//  cxEngineCore
//
//  Created by xuhua on 6/29/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#ifndef cxEngineCore_cxEvent_h
#define cxEngineCore_cxEvent_h

#include <functional>
#include <vector>
#include "cxDefine.h"

CX_CPP_BEGIN

#define cxBindEvent(_o_,_f_)       std::bind(&_f_,_o_,std::placeholders::_1)

template<class T>
class cxEvent
{
public:
    explicit cxEvent(){}
    virtual ~cxEvent(){}
public:
    typedef std::function<void(T *sender)> Event;
private:
    typedef std::vector<Event> Events;
    Events es;
public:
    cxEvent &operator+=(const Event &v)
    {
        es.push_back(v);
        return *this;
    }
    void Clear()
    {
        es.clear();
    }
    void Fire(T *sender)
    {
        if(es.empty())return;
        typename Events::iterator it = es.begin();
        while(it!=es.end()){(*it)(sender);it++;}
    }
};

CX_CPP_END

#endif


