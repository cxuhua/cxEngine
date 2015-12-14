//
//  cxSqlite.cpp
//  cxEngineCore
//
//  Created by xuhua on 12/14/15.
//  Copyright © 2015 xuhua. All rights reserved.
//


#include "cxUtil.h"
#include "cxSqlite.h"

CX_CPP_BEGIN

CX_IMPLEMENT(cxSqlStmt);

cxSqlStmt::cxSqlStmt()
{
    stmt = nullptr;
}

cxSqlStmt::~cxSqlStmt()
{
    if(stmt != nullptr){
        sqlite3_finalize(stmt);
    }
}

cxBool cxSqlStmt::Exec()
{
    return sqlite3_step(stmt) == SQLITE_DONE;
}

cxBool cxSqlStmt::Step()
{
    return sqlite3_step(stmt) == SQLITE_ROW;
}

cxInt cxSqlStmt::ToInt(cxInt idx)
{
    return sqlite3_column_int(stmt, idx);
}

cxInt64 cxSqlStmt::ToInt64(cxInt idx)
{
    return sqlite3_column_int64(stmt, idx);
}

cchars cxSqlStmt::ToString(cxInt idx)
{
    return (cchars)sqlite3_column_text(stmt, idx);
}

const cxStr *cxSqlStmt::ToBlob(cxInt idx)
{
    cchars data = (cchars)sqlite3_column_blob(stmt, idx);
    cxInt bytes = sqlite3_column_bytes(stmt, idx);
    if(bytes == 0){
        return nullptr;
    }
    return cxStr::Create()->Init((cxAny)data, bytes);
}

cxFloat cxSqlStmt::ToFloat(cxInt idx)
{
    return (cxFloat)sqlite3_column_double(stmt, idx);
}

cxDouble cxSqlStmt::ToDouble(cxInt idx)
{
    return sqlite3_column_double(stmt, idx);
}

cxBool cxSqlStmt::Bind(cxInt idx,cxInt value)
{
    CX_ASSERT(stmt != nullptr, "stmt null");
    return sqlite3_bind_int(stmt, idx, value) == SQLITE_OK;
}

cxBool cxSqlStmt::Bind(cxInt idx,cxInt64 value)
{
    CX_ASSERT(stmt != nullptr, "stmt null");
    return sqlite3_bind_int64(stmt, idx, value) == SQLITE_OK;
}

cxBool cxSqlStmt::Bind(cxInt idx,cchars value)
{
    CX_ASSERT(stmt != nullptr, "stmt null");
    return sqlite3_bind_text(stmt, idx, value, -1, nullptr) == SQLITE_OK;
}

cxBool cxSqlStmt::Bind(cxInt idx,const cxStr *value)
{
    CX_ASSERT(stmt != nullptr, "stmt null");
    return sqlite3_bind_blob(stmt, idx, value->Data(), value->Size(), nullptr) == SQLITE_OK;
}

cxBool cxSqlStmt::Bind(cxInt idx,cxFloat value)
{
    CX_ASSERT(stmt != nullptr, "stmt null");
    return sqlite3_bind_double(stmt, idx, value) == SQLITE_OK;
}

cxBool cxSqlStmt::Bind(cxInt idx,cxDouble value)
{
    CX_ASSERT(stmt != nullptr, "stmt null");
    return sqlite3_bind_double(stmt, idx, value) == SQLITE_OK;
}

cxSqlStmt *cxSqlStmt::Create(sqlite3_stmt *pstmt)
{
    cxSqlStmt *ret = cxSqlStmt::Create();
    ret->stmt = pstmt;
    return ret;
}

CX_IMPLEMENT(cxSqlite);

cxSqlite::cxSqlite()
{
    sqlptr = nullptr;
}

cxSqlite::~cxSqlite()
{
    if(sqlptr != nullptr){
        sqlite3_close_v2(sqlptr);
        sqlptr = nullptr;
    }
}

cxBool cxSqlite::Begin()
{
    return Exec("BEGIN TRANSACTION");
}

cxBool cxSqlite::Rollback()
{
    return Exec("ROLLBACK");
}

cxBool cxSqlite::Commit()
{
    return Exec("COMMIT");
}

cxSqlStmt *cxSqlite::Prepare(cchars sql)
{
    sqlite3_stmt *stmt = nullptr;
    if(sqlite3_prepare_v2(sqlptr, sql, -1, &stmt, nullptr) != SQLITE_OK){
        return nullptr;
    }
    return cxSqlStmt::Create(stmt);
}

cxBool cxSqlite::Exec(cchars sql)
{
    CX_ASSERT(sqlptr != nullptr, "sqlptr null");
    char *msg = nullptr;
    int ret = sqlite3_exec(sqlptr, sql, nullptr, nullptr, &msg);
    if(ret != SQLITE_OK){
        CX_ERROR("exec error %s",msg);
        sqlite3_free(msg);
        return false;
    }
    return true;
}

cxBool cxSqlite::Init(cchars db,cxBool onlyread)
{
    const cxStr *path = cxUtil::Instance()->DocumentPath(db);
    int flags = 0;
    if(onlyread){
        flags = SQLITE_OPEN_READONLY;
    }else{
        flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;
    }
    return sqlite3_open_v2(path->ToString(), &sqlptr, flags, nullptr) == SQLITE_OK;
}

cxSqlite *cxSqlite::Create(cchars db,cxBool onlyread)
{
    cxSqlite *ret = cxSqlite::Create();
    if(!ret->Init(db,onlyread)){
        CX_ERROR("create db %s error",db);
        return nullptr;
    }
    return ret;
}

CX_CPP_END

