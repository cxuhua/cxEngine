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
    typedef std::list<cxObject *>::const_iterator CFIter;
    typedef std::list<cxObject *>::reverse_iterator RIter;
    typedef std::list<cxObject *>::const_reverse_iterator CRIter;
public:
    static cxBool IsOK(const cxList *v);
public:
    RIter RBegin();
    CRIter RBegin() const;
    
    RIter REnd();
    CRIter REnd() const;
    
    FIter FBegin();
    CFIter FBegin() const;
    
    FIter FEnd();
    CFIter FEnd() const;
    
    FIter Remove(FIter &iter);
    CFIter Remove(CFIter &iter);
    
    RIter Remove(RIter &iter);
    CRIter Remove(CRIter &iter);
private:
    std::list<cxObject *> ml;
public:
    void Elements(std::function<void(cxObject *)> func);
    cxObject *At(cxInt idx);
    cxList *Clear();
    cxBool IsEmpty() const;
    cxInt Size() const;
    cxObject *Front() const;
    cxObject *Back() const;
    cxList *Append(cxObject *obj);
    cxList *Prepend(cxObject *obj);
    cxList *Remove(cxObject *obj);
    cxList *PopFront();
    cxList *PopBack();
};

CX_CPP_END

#endif


