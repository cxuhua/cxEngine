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
    cxBool Bind(cxInt idx,cxInt value);
    cxBool Bind(cxInt idx,cxInt64 value);
    cxBool Bind(cxInt idx,cchars value);
    cxBool Bind(cxInt idx,const cxStr *value);
    cxBool Bind(cxInt idx,cxFloat value);
    cxBool Bind(cxInt idx,cxDouble value);
    //run
    cxBool Step();
    cxBool Exec();
    //step success value
    cxInt ToInt(cxInt idx);
    cxInt64 ToInt64(cxInt idx);
    cchars ToString(cxInt idx);
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
public:
    //TRANSACTION
    cxBool Begin();
    cxBool Rollback();
    cxBool Commit();
    //
    cxSqlStmt *Prepare(cchars sql);
    cxBool Exec(cchars sql);
    cxBool Init(cchars db,cxBool onlyread);
    //create document dir
    static cxSqlite *Create(cchars db,cxBool onlyread);
};

CX_CPP_END

#endif


