//
//  cxList.h
//  cxEngineCore
//
//  Created by xuhua on 6/21/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#ifndef cxEngineCore_cxList_h
#define cxEngineCore_cxList_h

#include <list>
#include "cxObject.h"

CX_CPP_BEGIN

class cxList : public cxObject
{
public:
    CX_DECLARE(cxList);
protected:
    explicit cxList();
    virtual ~cxList();
public:
    typedef std::list<cxObject *>::iterator FIter;
    typedef std::list<cxObject *>::reverse_iterator RIter;
    RIter RBegin();
    RIter REnd();
    FIter FBegin();
    FIter FEnd();
    FIter Remove(FIter &iter);
    RIter Remove(RIter &iter);
private:
    std::list<cxObject *> ml;
public:
    cxList *Clear();
    cxBool IsEmpty() const;
    cxInt Size() const;
    cxObject *Front() const;
    cxObject *Back() const;
    cxList *PushBack(cxObject *obj);
    cxList *PushFront(cxObject *obj);
    cxList *Remove(cxObject *obj);
    cxObject *PopFront();
    cxObject *PopBack();
};

CX_CPP_END

#endif


