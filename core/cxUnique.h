//
//  cxUnique.h
//  cxEngineCore
//
//  Created by xuhua on 6/22/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#ifndef cxEngineCore_cxUnique_h
#define cxEngineCore_cxUnique_h

#include <map>
#include "cxObject.h"

CX_CPP_BEGIN

class cxArray;
class cxUnique : public cxObject
{
public:
    CX_DECLARE(cxUnique);
protected:
    explicit cxUnique();
    virtual ~cxUnique();
public:
    typedef std::map<cxULong, cxObject *>::const_iterator Iter;
    const Iter Begin() const;
    const Iter End() const;
    Iter Remove(const Iter &iter);
private:
    std::map<cxULong, cxObject *> mm;
public:
    cxInt Size() const;
    cxBool IsEmpty() const;
    cxUnique *Append(cxObject *obj);
    cxUnique *Remove(cxObject *obj);
    cxUnique *Clear();
    cxArray *ToArray();
};

CX_CPP_END

#endif


