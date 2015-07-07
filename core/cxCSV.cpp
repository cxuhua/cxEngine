//
//  cxCSV.cpp
//  cxEngineCore
//
//  Created by xuhua on 7/6/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#include "cxStr.h"
#include "cxCSV.h"

CX_CPP_BEGIN

CX_IMPLEMENT(cxCSV);

cxCSV::cxCSV()
{
    csv_init(&parser, 0);
}

cxCSV::~cxCSV()
{
    csv_free(&parser);
}

const cxInt cxCSV::Row() const
{
    return (cxInt)datas.size();
}

const cxInt cxCSV::Col(cxInt row) const
{
    const std::vector<std::string> &r = datas.at(row);
    return (cxInt)r.size();
}

const cxStr *cxCSV::At(cxInt row,cxInt col) const
{
    CX_ASSERT(row < Row(), "row out");
    CX_ASSERT(col < Col(row), "row out");
    const std::vector<std::string> &r = datas.at(row);
    const std::string &s = r.at(col);
    return cxStr::Create()->Init(s.data());
}

void cxCSV::colcbfunc(void *data, size_t datasiz, void *ud)
{
    cxCSV *csv = static_cast<cxCSV *>(ud);
    if(datasiz == 0){
        csv->row.push_back(std::string(""));
    }else{
        csv->row.push_back(std::string((char *)data,datasiz));
    }
}

void cxCSV::rowcbfunc(int c, void *ud)
{
    cxCSV *csv = static_cast<cxCSV *>(ud);
    csv->datas.push_back(csv->row);
    csv->row.clear();
}

cxCSV *cxCSV::Create(const cxStr *data)
{
    cxCSV *rv = cxCSV::Create();
    csv_parse(&rv->parser, data->Data(), data->Size(), colcbfunc, rowcbfunc, rv);
    rv->datas.push_back(rv->row);
    return rv;
}

CX_CPP_END

