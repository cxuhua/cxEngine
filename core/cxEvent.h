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

class cxArgs
{
public:
    explicit cxArgs()
    {
        //
    }
    virtual ~cxArgs()
    {
        //
    }
    template<class T>
    const T &To() const
    {
        return static_cast<const T&>(*this);
    }
};

template<class T,class A>
class cxEvent
{
public:
    explicit cxEvent(){}
    virtual ~cxEvent(){}
public:
    typedef std::function<void(T *sender,const A &args)> Event;
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
        A args;
        if(es.empty())return;
        typename Events::iterator it = es.begin();
        while(it!=es.end()){(*it)(sender,args);it++;}
    }
    void Fire(T *sender,const A &args)
    {
        if(es.empty())return;
        typename Events::iterator it = es.begin();
        while(it!=es.end()){(*it)(sender,args);it++;}
    }
};

CX_CPP_END

#endif


