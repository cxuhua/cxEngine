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

#define CX_BIND_EVENT_A1(_s_,_f_) \
    std::bind(&_f_,_s_,std::placeholders::_1)

#define CX_BIND_EVENT_A2(_s_,_f_) \
    std::bind(&_f_,_s_,std::placeholders::_1,std::placeholders::_2)

#define CX_BIND_EVENT_A3(_s_,_f_) \
    std::bind(&_f_,_s_,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3)

#define CX_BIND_EVENT_A4(_s_,_f_) \
    std::bind(&_f_,_s_,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3,std::placeholders::_4)

#define CX_BIND_EVENT_A5(_s_,_f_) \
    std::bind(&_f_,_s_,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3,std::placeholders::_4,std::placeholders::_5)

template<class T,typename...A>
class cxEvent
{
public:
    explicit cxEvent(){}
    virtual ~cxEvent(){}
private:
    typedef std::function<void(T *,A...)> Event;
    typedef std::vector<Event> Events;
    Events es;
public:
    cxBool IsEmpty()
    {
        return es.empty();
    }
    cxEvent &operator+=(const Event &v)
    {
        es.push_back(v);
        return *this;
    }
    cxEvent &operator=(const cxEvent &v)
    {
        es = v.es;
        return *this;
    }
    cxEvent &operator=(const Event &v)
    {
        es.clear();
        es.push_back(v);
        return *this;
    }
    void Clear()
    {
        es.clear();
    }
    void Fire(T *sender,A...args)
    {
        if(IsEmpty())return;
        typename Events::iterator it = es.begin();
        while(it!=es.end()){(*it)(sender,args...);it++;}
    }
};

CX_CPP_END

#endif


