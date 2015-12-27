//
//  cxSqlite.h
//  cxEngineCore
//
//  Created by xuhua on 12/14/15.
//  Copyright © 2015 xuhua. All rights reserved.
//

#ifndef cxEngineCore_cxSqlite_h
#define cxEngineCore_cxSqlite_h

#include <ext/sqlite3.h>
#include "cxObject.h"

CX_CPP_BEGIN

class cxSqlStmt : public cxObject
{
public:
    CX_DECLARE(cxSqlStmt);
protected:
    explicit cxSqlStmt();
    virtual ~cxSqlStmt();
private:
    sqlite3_stmt *stmt;
public:
    //binds
    cxBool Bind(cxInt idx,cxBool value);
    cxBool Bind(cxInt idx,cxInt value);
    cxBool Bind(cxInt idx,cxInt64 value);
    cxBool Bind(cxInt idx,cchars value);
    cxBool Bind(cxInt idx,const cxStr *value);
    cxBool Bind(cxInt idx,cxFloat value);
    cxBool Bind(cxInt idx,cxDouble value);
    //reset
    void Reset();
    //run
    cxBool Step();
    cxBool Exec();
    //step success value
    cxBool ToBool(cxInt idx);
    cxInt ToInt(cxInt idx);
    cxInt64 ToInt64(cxInt idx);
    cchars ToChars(cxInt idx);
    const cxStr *ToString(cxInt idx);
    const cxStr *ToBlob(cxInt idx);
    cxFloat ToFloat(cxInt idx);
    cxDouble ToDouble(cxInt idx);
public:
    static cxSqlStmt *Create(sqlite3_stmt *pstmt);
};

class cxSqlite : public cxObject
{
public:
    CX_DECLARE(cxSqlite);
protected:
    explicit cxSqlite();
    virtual ~cxSqlite();
private:
    sqlite3 *sqlptr;
    static int xAuth(void *data, int type, const char *s1, const char *s2, const char *s3, const char *s4);
public:
    cxBool ExistTable(cchars name);
    //return SQLITE_OK, SQLITE_DENY, or SQLITE_IGNORE
    virtual cxInt Auth(int type, const char *s1, const char *s2, const char *s3, const char *s4);
    //TRANSACTION
    cxBool Begin();
    cxBool Rollback();
    cxBool Commit();
    //
    cxSqlStmt *Prepare(cchars fmt,...);
    cxBool Exec(cchars fmt,...);
    virtual cxBool Init(cchars db,cxBool onlyread);
    //create document dir
    //if db == nullptr will open db at memory
    static cxSqlite *Create(cchars db,cxBool onlyread);
};

CX_CPP_END

#endif


