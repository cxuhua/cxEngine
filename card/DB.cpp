//
//  DB.cpp
//  card
//
//  Created by xuhua on 12/14/15.
//  Copyright © 2015 xuhua. All rights reserved.
//

#include <core/cxUtil.h>
#include "DB.h"
#include "Game.h"

CX_CPP_BEGIN

CX_IMPLEMENT(UpData)

UpData::UpData()
{
    key = nullptr;
    dataId = nullptr;
    version = 0;
    time = 0;
}

UpData::~UpData()
{
    cxObject::release(&key);
    cxObject::release(&dataId);
}

const cxStr *UpData::Key()
{
    return key;
}

const cxStr *UpData::DataId()
{
    return dataId;
}

void UpData::SetVersion(cxInt v)
{
    version = v;
}

void UpData::SetDataId(const cxStr *v)
{
    cxObject::swap(&dataId, v);
}

void UpData::SetTime(cxInt64 v)
{
    time = v;
}

cxBool UpData::Delete()
{
    cxSqlStmt *stmt = db->Prepare("DELETE FROM updates WHERE Key=?;");
    stmt->Bind(1, key->ToString());
    return stmt->Exec();
}

cxBool UpData::Exists()
{
    cxSqlStmt *stmt = db->Prepare("SELECT COUNT(Key) FROM updates WHERE Key=? LIMIT 1;");
    stmt->Bind(1, key->ToString());
    if(!stmt->Step()){
        return false;
    }
    return stmt->ToInt(1) > 0;
}

cxBool UpData::Insert()
{
    cxSqlStmt *stmt = nullptr;
    if(Exists()){
        stmt = db->Prepare("UPDATE updates SET Version=?,DataId=?,Time=? WHERE Key=?;");
        stmt->Bind(1, version);
        stmt->Bind(2, dataId->ToString());
        stmt->Bind(3, time);
        stmt->Bind(4, key->ToString());
    }else{
        stmt = db->Prepare("INSERT INTO updates VALUES(?,?,?,?);");
        stmt->Bind(1, key->ToString());
        stmt->Bind(2, version);
        stmt->Bind(3, dataId->ToString());
        stmt->Bind(4, time);
    }
    return stmt->Exec();
}

UpData *UpData::Create(DB *db,cchars key)
{
    UpData *ret = UpData::Create();
    ret->db = db;
    ret->key = cxStr::Alloc()->Init(key);
    return ret;
}

UpData *UpData::Create(DB *db,const cxStr *key)
{
    return Create(db, key->ToString());
}

Data *UpData::GetData(const cxStr *data)
{
    Data *d = Data::Create(key->ToString());
    if(!d->Load(data)){
        CX_ERROR("load data error");
        return nullptr;
    }
    return d;
}

CX_IMPLEMENT(DB);

DB::DB()
{
    dataCaches = cxHash::Alloc();
}

DB::~DB()
{
    dataCaches->Release();
}

const cxStr *DB::GetValue(cchars key)
{
    cxSqlStmt *stmt = Prepare("SELECT Value FROM config WHERE Key='%s';",key);
    if(stmt->Step()){
        return stmt->ToString(1);
    }
    return nullptr;
}

cxBool DB::SetValue(cchars key,const cxStr *value)
{
    cxSqlStmt *stmt = Prepare("UPDATE config SET Value=? WHERE Key='%s';",key);
    stmt->Bind(1, value->ToString());
    return stmt->Exec();
}

cxInt64 DB::TimeNow()
{
    //从网络获得当前时间
    return G()->GetNet()->TimeNow();
}

cxInt DB::DataVersion()
{
    return GetValue("DataVersion")->ToInt();
}

cxBool DB::SetDataVersion(cxInt ver)
{
    return SetValue("DataVersion", cxStr::UTF8("%d",ver));
}

cxBool DB::SetBindId(const cxStr *id)
{
    return SetValue("BindId", id);
}

const cxStr *DB::BindId()
{
    return GetValue("BindId");
}

cxBool DB::SetTempId(const cxStr *id)
{
    return SetValue("TempId", id);
}

const cxStr *DB::TempId()
{
    return GetValue("TempId");
}

cxInt DB::UpDataCount()
{
    cxSqlStmt *stmt = Prepare("SELECT COUNT(Key) FROM updates;");
    if(stmt->Step()){
        return stmt->ToInt(1);
    }
    return 0;
}

UpData *DB::OneUpData()
{
    cxSqlStmt *stmt = Prepare("SELECT Key,Version,DataId,Time FROM updates LIMIT 1;");
    if(!stmt->Step()){
        return nullptr;
    }
    UpData *up = UpData::Create(this, stmt->ToChars(1));
    up->SetVersion(stmt->ToInt(2));
    up->SetDataId(stmt->ToString(3));
    up->SetTime(stmt->ToInt64(4));
    return up;
}

cxBool DB::HasData(const cxStr *key)
{
    if(dataCaches->Has(key->ToString())){
        return true;
    }
    cxSqlStmt *stmt = Prepare("SELECT COUNT(Key) FROM datas WHERE Key=? LIMIT 1;");
    stmt->Bind(1, key->ToString());
    return stmt->Step() && stmt->ToInt(1) > 0;
}

Data *DB::CardData(const cxStr *key)
{
    Data *ret = dataCaches->Get(key)->To<Data>();
    if(ret != nullptr){
        return ret;
    }
    const cxStr *d = ReadData(key);
    if(!cxStr::IsOK(d)){
        return nullptr;
    }
    ret = Data::Create(key);
    if(!ret->Load(d)){
        return nullptr;
    }
    dataCaches->Set(key, ret);
    return ret;
}

const cxStr *DB::ReadData(const cxStr *key)
{
    cxSqlStmt *stmt = Prepare("SELECT Data FROM datas WHERE Key=? LIMIT 1;");
    stmt->Bind(1, key->ToString());
    if(stmt->Step()){
        return stmt->ToBlob(1);
    }
    return nullptr;
}

cxBool DB::WriteData(const cxStr *key,const cxStr *data)
{
    cxSqlStmt *stmt = nullptr;
    if(!HasData(key)){
        stmt = Prepare("INSERT INTO datas VALUES(?,?);");
        stmt->Bind(1, key->ToString());
        stmt->Bind(2, data);
    }else{
        stmt = Prepare("UPDATE datas SET Data=? WHERE Key=?;");
        stmt->Bind(1, data);
        stmt->Bind(2, key->ToString());
    }
    return stmt->Exec();
}

cxBool DB::Init()
{
    if(!cxSqlite::Init("card.db", false)){
        CX_ERROR("init db error");
        return false;
    }
    Begin();
    if(!ExistTable("config")){
        //创建配置表
        Exec("CREATE TABLE config(Key VARCHAR(32) PRIMARY KEY,Value VARCHAR(128));");
        //词库版本(DataVersion),是否有新的词卡
        Exec("INSERT INTO config VALUES('DataVersion','0');");
        //用户TempId,用户唯一id
        const cxStr *id = cxUtil::Instance()->UUID()->MD5();
        Exec("INSERT INTO config VALUES('TempId','%s');",id->ToString());
    }
    if(!ExistTable("updates")){
        //将要更新的word临时列表,更新数据后删除
        Exec("CREATE TABLE updates(Key VARCHAR(255) PRIMARY KEY,Version INT, DataId VARCHAR(32),Time INT);");
    }
    if(!ExistTable("datas")){
        //创建数据表
        Exec("CREATE TABLE datas(Key VARCHAR(255) PRIMARY KEY,Data BLOB);");
    }
    Commit();
    return true;
}

CX_CPP_END

