//
//  cxCSV.h
//  cxEngineCore
//
//  Created by xuhua on 7/6/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#ifndef cxEngineCore_cxCSV_h
#define cxEngineCore_cxCSV_h

#include <vector>
#include <ext/csv.h>
#include "cxObject.h"

CX_CPP_BEGIN

class cxArray;
class cxStr;
class cxCSV : public cxObject
{
public:
    CX_DECLARE(cxCSV);
protected:
    explicit cxCSV();
    virtual ~cxCSV();
private:
    cxArray *rows;
    cxArray *row;
    struct csv_parser parser;
    static void colcbfunc(void *data, size_t datasiz, void *ud);
    static void rowcbfunc(int c, void *ud);
public:
    const cxInt Row() const;
    const cxInt Col(cxInt row=0) const;
    const cxStr *At(cxInt row,cxInt col) const;
    cxStr *At(cxInt row,cxInt col);
    cxCSV *Init(cxUInt8 opt);
public:
    static cxCSV *Create(const cxStr *data,cxUInt8 opt=0);
};

CX_CPP_END

#endif


