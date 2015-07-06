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

class cxStr;
class cxCSV : public cxObject
{
public:
    CX_DECLARE(cxCSV);
protected:
    explicit cxCSV();
    virtual ~cxCSV();
private:
    std::vector<std::vector<std::string>> datas;
    struct csv_parser parser;
    static void colcbfunc(void *data, size_t datasiz, void *ud);
    static void rowcbfunc(int c, void *ud);
    std::vector<std::string> row;
public:
    const cxInt Row() const;
    const cxInt Col(cxInt row=0) const;
    const cxStr *At(cxInt row,cxInt col) const;
    static cxCSV *Create(const cxStr *data);
};

CX_CPP_END

#endif


