//
//  cxCSV.cpp
//  cxEngineCore
//
//  Created by xuhua on 7/6/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#include "cxArray.h"
#include "cxStr.h"
#include "cxCSV.h"

CX_CPP_BEGIN

CX_IMPLEMENT(cxCSV);

cxCSV::cxCSV()
{
    row = cxArray::Alloc();
    rows = cxArray::Alloc();
    csv_init(&parser, 0);
}

cxCSV::~cxCSV()
{
    rows->Release();
    row->Release();
    csv_free(&parser);
}

const cxInt cxCSV::Row() const
{
    return rows->Size();
}

const cxInt cxCSV::Col(cxInt row) const
{
    const cxArray *rs = rows->At(row)->To<cxArray>();
    return rs->Size();
}

const cxStr *cxCSV::At(cxInt row,cxInt col) const
{
    CX_ASSERT(row < Row(), "row out");
    CX_ASSERT(col < Col(row), "row out");
    cxArray *rs = rows->At(row)->To<cxArray>();
    return rs->At(col)->To<cxStr>();
}

cxStr *cxCSV::At(cxInt row,cxInt col)
{
    CX_ASSERT(row < Row(), "row out");
    CX_ASSERT(col < Col(row), "col out");
    cxArray *rs = rows->At(row)->To<cxArray>();
    return rs->At(col)->To<cxStr>();
}

void cxCSV::colcbfunc(void *data, size_t datasiz, void *ud)
{
    cxCSV *csv = static_cast<cxCSV *>(ud);
    cchars s = (cchars)data;
    cxStr *str = nullptr;
    if(datasiz > 0 && s[0] != '\0'){
        str = cxStr::Alloc()->Init((char *)s,(cxInt)datasiz);
    }else{
        str = cxStr::Alloc();
    }
    csv->row->Append(str);
    str->Release();
}

void cxCSV::rowcbfunc(int c, void *ud)
{
    cxCSV *csv = static_cast<cxCSV *>(ud);
    csv->rows->Append(csv->row);
    cxArray *newrow = cxArray::Alloc();
    cxObject::swap(&csv->row, newrow);
    newrow->Release();
}

cxCSV *cxCSV::Create(const cxStr *data)
{
    cxCSV *rv = cxCSV::Create();
    csv_parse(&rv->parser, data->Data(), data->Size()+1, colcbfunc, rowcbfunc, rv);
    csv_fini(&rv->parser, colcbfunc, rowcbfunc, rv);
    return rv;
}

CX_CPP_END

