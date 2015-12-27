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

cxBool UpData::Insert()
{
    cxSqlStmt *stmt = db->Prepare("INSERT INTO updates VALUES(?,?,?,?);");
    stmt->Bind(1, key->ToString());
    stmt->Bind(2, version);
    stmt->Bind(3, dataId->ToString());
    stmt->Bind(4, time);
    return stmt->Exec();
}

UpData *UpData::Create(DB *db,cchars key)
{
    UpData *ret = UpData::Create();
    ret->db = db;
    ret->key = cxStr::Alloc()->Init(key);
    return ret;
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

CX_IMPLEMENT(Word)

Word::Word()
{
    key = nullptr;
    level = 1;
    isbuy = false;
    isuse = true;
    time = 0;
    group = 0;
    data = nullptr;
}

Word::~Word()
{
    cxObject::release(&data);
    cxObject::release(&key);
}

void Word::SetGroup(cxInt v)
{
    group = v;
}

Data *Word::GetData()
{
    if(data != nullptr){
        return data;
    }
    //获取数据
    const cxStr *bytes = db->ReadData(key);
    if(!cxStr::IsOK(bytes)){
        CX_ERROR("read data error");
        return nullptr;
    }
    //加载解码数据
    Data *d = Data::Create(key);
    if(!d->Load(bytes)){
        CX_ERROR("from data load error");
        return nullptr;
    }
    //保存数据
    cxObject::swap(&data, d);
    return d;
}

Word *Word::Create(DB *db,Data *data)
{
    Word *ret = Create(db, data->Key());
    cxObject::swap(&ret->data, data);
    ret->SetGroup(data->GroupId());
    return ret;
}

Word *Word::Create(DB *db,const cxStr *key)
{
    return Create(db, key->ToString());
}

Word *Word::Create(DB *db,cchars key)
{
    Word *word = Word::Create();
    word->db = db;
    word->key = cxStr::Alloc()->Init(key);
    return word;
}

void Word::SetValue(cxSqlStmt *stmt)
{
    //1 -> key
    level = stmt->ToInt(2);
    isbuy = stmt->ToBool(3);
    isuse = stmt->ToBool(4);
    time = stmt->ToInt64(5);
    group = stmt->ToInt(6);
    last = stmt->ToInt64(7);
}

cxBool Word::Exists()
{
    CX_ASSERT(cxStr::IsOK(key), "key not set");
    CX_ASSERT(db != nullptr, "db not set");
    cxSqlStmt *stmt = db->Prepare("SELECT Key,Level,IsBuy,IsUse,Time,GroupId,Last FROM words WHERE key=? LIMIT 1");
    stmt->Bind(1, key->ToString());
    if(!stmt->Step()){
        return false;
    }
    SetValue(stmt);
    return true;
}

cxInt64 Word::NextTime()
{
    CX_ASSERT(db != nullptr, "db not set");
    return db->LevelTime(level);
}

cxBool Word::Insert()
{
    CX_ASSERT(cxStr::IsOK(key), "key not set");
    cxSqlStmt *stmt = db->Prepare("INSERT INTO words VALUES(?,?,?,?,?,?,?);");
    stmt->Bind(1, key->ToString());
    stmt->Bind(2, group);
    stmt->Bind(3, level);
    stmt->Bind(4, isbuy);
    stmt->Bind(5, isuse);
    stmt->Bind(6, NextTime());
    stmt->Bind(7, db->TimeNow());
    if(!stmt->Exec()){
        return false;
    }
    if(data != nullptr){
        return db->WriteData(key, data->Bytes());
    }
    return true;
}

CX_IMPLEMENT(DB);

DB::DB()
{
    LVS[0] = 5 * TIME_MINUTE;
    LVS[1] = 30 * TIME_MINUTE;
    LVS[2] = 12 * TIME_HOUR;
    LVS[3] = 1 * TIME_DAY;
    LVS[4] = 2 * TIME_DAY;
    LVS[5] = 4 * TIME_DAY;
    LVS[6] = 7 * TIME_DAY;
    LVS[7] = 15 * TIME_DAY;
    LVS[8] = 30 * TIME_DAY;
}

DB::~DB()
{
    
}

cxArray *DB::ReviewWords(cxInt count)
{
    cxInt64 now = TimeNow();
    cxArray *ws = cxArray::Create();
    cxSqlStmt *stmt = Prepare("SELECT Key,Level,IsBuy,IsUse,Time,GroupId,Last FROM words "
                              "WHERE IsBuy = 1 AND IsUse = 1 AND Time <= ? LIMIT ?");
    stmt->Bind(1, now);
    stmt->Bind(2, count);
    while(stmt->Step()){
        Word *word = Word::Create(this, stmt->ToString(1));
        word->SetValue(stmt);
        ws->Append(word);
    }
    return ws;
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

cxInt64 DB::LevelTime(cxInt level)
{
    cxInt64 now = TimeNow();
    if(level >=1 && level <= 9){
        return now + LVS[level-1];
    }
    return now + (level * 3 + 1) * TIME_DAY;
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
    cxSqlStmt *stmt = Prepare("SELECT COUNT(*) FROM updates");
    if(stmt->Step()){
        return stmt->ToInt(1);
    }
    return 0;
}

UpData *DB::OneUpData()
{
    cxSqlStmt *stmt = Prepare("SELECT Key,Version,DataId,Time FROM updates LIMIT 1");
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
    cxSqlStmt *stmt = Prepare("SELECT COUNT(*) FROM datas WHERE Key=? LIMIT 1;");
    stmt->Bind(1, key->ToString());
    return stmt->Step() && stmt->ToInt(1) > 0;
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
        stmt = Prepare("UPDATE config SET Data=? WHERE Key=?;");
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
    if(!ExistTable("words")){
        //创建词卡表
        Exec("CREATE TABLE words("
             "Key VARCHAR(256) PRIMARY KEY,"//词条主键,按小写方式存储
             "GroupId INT,"                 //分组id
             "Level INT,"                   //等级
             "IsBuy INT,"                   //是否已经购买
             "IsUse INT,"                   //是否用于复习
             "Time INT,"                    //下次复习时间
             "Last INT);");                 //最后复习时间
        //创建分类索引
        Exec("CREATE INDEX GroupIndex ON words(GroupId ASC)");
        //创建复习时间索引
        Exec("CREATE INDEX TimeIndex ON words(Time ASC)");
        //创建是否使用索引
        Exec("CREATE INDEX IsUseIndex ON words(IsUse ASC)");
    }
    if(!ExistTable("updates")){
        //将要更新的word列表,更新数据后删除
        Exec("CREATE TABLE updates(Key VARCHAR(32) PRIMARY KEY,Version INT, DataId VARCHAR(32),Time INT);");
    }
    if(!ExistTable("datas")){
        //创建数据表
        Exec("CREATE TABLE datas(Key VARCHAR(32) PRIMARY KEY,Data BLOB);");
    }
    Commit();
    return true;
}

CX_CPP_END

