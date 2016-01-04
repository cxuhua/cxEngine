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

void cxSqlStmt::Reset()
{
    CX_ASSERT(stmt != nullptr, "stmt null");
    sqlite3_reset(stmt);
}

cxBool cxSqlStmt::Exec()
{
    CX_ASSERT(stmt != nullptr, "stmt null");
    return sqlite3_step(stmt) == SQLITE_DONE;
}

cxBool cxSqlStmt::Step()
{
    CX_ASSERT(stmt != nullptr, "stmt null");
    return sqlite3_step(stmt) == SQLITE_ROW;
}

cxInt cxSqlStmt::ToInt(cxInt idx)
{
    CX_ASSERT(stmt != nullptr, "stmt null");
    CX_ASSERT(idx > 0, "idx > 0");
    return sqlite3_column_int(stmt, idx-1);
}

cxBool cxSqlStmt::ToBool(cxInt idx)
{
    CX_ASSERT(stmt != nullptr, "stmt null");
    CX_ASSERT(idx > 0, "idx > 0");
    return ToInt(idx) != 0;
}

cxInt64 cxSqlStmt::ToInt64(cxInt idx)
{
    CX_ASSERT(stmt != nullptr, "stmt null");
    CX_ASSERT(idx > 0, "idx > 0");
    return sqlite3_column_int64(stmt, idx-1);
}

cchars cxSqlStmt::ToChars(cxInt idx)
{
    CX_ASSERT(stmt != nullptr, "stmt null");
    CX_ASSERT(idx > 0, "idx > 0");
    return (cchars)sqlite3_column_text(stmt, idx-1);
}

const cxStr *cxSqlStmt::ToString(cxInt idx)
{
    CX_ASSERT(stmt != nullptr, "stmt null");
    CX_ASSERT(idx > 0, "idx > 0");
    cchars s =  (cchars)sqlite3_column_text(stmt, idx-1);
    if(!cxStr::IsOK(s)){
        return nullptr;
    }
    return cxStr::Create()->Init(s);
}

const cxStr *cxSqlStmt::ToBlob(cxInt idx)
{
    CX_ASSERT(stmt != nullptr, "stmt null");
    CX_ASSERT(idx > 0, "idx > 0");
    cchars data = (cchars)sqlite3_column_blob(stmt, idx-1);
    cxInt bytes = sqlite3_column_bytes(stmt, idx-1);
    if(bytes == 0){
        return nullptr;
    }
    return cxStr::Create()->Init((cxAny)data, bytes);
}

cxFloat cxSqlStmt::ToFloat(cxInt idx)
{
    CX_ASSERT(stmt != nullptr, "stmt null");
    CX_ASSERT(idx > 0, "idx > 0");
    return (cxFloat)sqlite3_column_double(stmt, idx-1);
}

cxDouble cxSqlStmt::ToDouble(cxInt idx)
{
    CX_ASSERT(stmt != nullptr, "stmt null");
    CX_ASSERT(idx > 0, "idx > 0");
    return sqlite3_column_double(stmt, idx-1);
}

cxBool cxSqlStmt::Bind(cxInt idx,cxBool value)
{
    CX_ASSERT(stmt != nullptr, "stmt null");
    CX_ASSERT(idx > 0, "idx > 0");
    return Bind(idx, value?1:0);
}

cxBool cxSqlStmt::Bind(cxInt idx,cxInt value)
{
    CX_ASSERT(stmt != nullptr, "stmt null");
    CX_ASSERT(idx > 0, "idx > 0");
    return sqlite3_bind_int(stmt, idx, value) == SQLITE_OK;
}

cxBool cxSqlStmt::Bind(cxInt idx,cxInt64 value)
{
    CX_ASSERT(stmt != nullptr, "stmt null");
    CX_ASSERT(idx > 0, "idx > 0");
    return sqlite3_bind_int64(stmt, idx, value) == SQLITE_OK;
}

cxBool cxSqlStmt::Bind(cxInt idx,cchars value)
{
    CX_ASSERT(stmt != nullptr, "stmt null");
    CX_ASSERT(idx > 0, "idx > 0");
    cxInt ret = 0;
    if(!cxStr::IsOK(value)){
        ret = sqlite3_bind_null(stmt, idx);
    }else{
        ret = sqlite3_bind_text(stmt, idx, value, -1, nullptr);
    }
    return ret == SQLITE_OK;
}

cxBool cxSqlStmt::Bind(cxInt idx,const cxStr *value)
{
    CX_ASSERT(stmt != nullptr, "stmt null");
    CX_ASSERT(idx > 0, "idx > 0");
    cxInt ret = 0;
    if(!cxStr::IsOK(value)){
        ret = sqlite3_bind_null(stmt, idx);
    }else{
        ret = sqlite3_bind_blob(stmt, idx, value->Data(), value->Size(), nullptr);
    }
    return ret == SQLITE_OK;
}

cxBool cxSqlStmt::Bind(cxInt idx,cxFloat value)
{
    CX_ASSERT(stmt != nullptr, "stmt null");
    CX_ASSERT(idx > 0, "idx > 0");
    return sqlite3_bind_double(stmt, idx, value) == SQLITE_OK;
}

cxBool cxSqlStmt::Bind(cxInt idx,cxDouble value)
{
    CX_ASSERT(stmt != nullptr, "stmt null");
    CX_ASSERT(idx > 0, "idx > 0");
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

cxBool cxSqlite::ExistTable(cchars name)
{
    cxSqlStmt *stmt = Prepare("SELECT name FROM sqlite_master WHERE type='table' AND name='%s';",name);
    return stmt->Step();
}

cxBool cxSqlite::Begin()
{
    return Exec("BEGIN TRANSACTION;");
}

cxBool cxSqlite::Rollback()
{
    return Exec("ROLLBACK;");
}

cxBool cxSqlite::Commit()
{
    return Exec("COMMIT;");
}

cxSqlStmt *cxSqlite::Prepare(cchars fmt,...)
{
    cxStr *sql = cxStr::Create();
    va_list ap;
    va_start(ap, fmt);
    sql->AppFmt(fmt, ap);
    va_end(ap);
    if(!cxStr::IsOK(sql)){
        return nullptr;
    }
    sqlite3_stmt *stmt = nullptr;
    if(sqlite3_prepare_v2(sqlptr, sql->Data(), sql->Size(), &stmt, nullptr) != SQLITE_OK){
        return nullptr;
    }
    return cxSqlStmt::Create(stmt);
}

cxBool cxSqlite::Exec(cchars fmt,...)
{
    CX_ASSERT(sqlptr != nullptr, "sqlptr null");
    cxStr *sql = cxStr::Create();
    va_list ap;
    va_start(ap, fmt);
    sql->AppFmt(fmt,ap);
    va_end(ap);
    char *msg = nullptr;
    int ret = sqlite3_exec(sqlptr, sql->ToString(), nullptr, nullptr, &msg);
    if(ret != SQLITE_OK){
        CX_ERROR("exec error %s",msg);
        sqlite3_free(msg);
        return false;
    }
    return true;
}

cxInt cxSqlite::Auth(int type, const char *s1, const char *s2, const char *s3, const char *s4)
{
    return SQLITE_OK;
}

int cxSqlite::xAuth(void *data, int type, const char *s1, const char *s2, const char *s3, const char *s4)
{
    cxSqlite *db = static_cast<cxSqlite *>(data);
    return db->Auth(type, s1, s2, s3, s4);
}

cxBool cxSqlite::Init(cchars db,cxBool onlyread)
{
    const cxStr *path = nullptr;
    if(db != nullptr){
        path = cxUtil::Instance()->DocumentPath(db);
    }else{
        path = cxStr::Create()->Init(":memory:");
    }
    int flags = 0;
    if(onlyread){
        flags = SQLITE_OPEN_READONLY;
    }else{
        flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;
    }
    cxInt ret = sqlite3_open_v2(path->ToString(), &sqlptr, flags, nullptr);
    if(ret != SQLITE_OK){
        return false;
    }
    sqlite3_set_authorizer(sqlptr, cxSqlite::xAuth, this);
    CX_LOGGER("db init at:%s",path->ToString());
    return true;
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

