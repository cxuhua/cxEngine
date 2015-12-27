//
//  DB.h
//  card
//
//  Created by xuhua on 12/14/15.
//  Copyright © 2015 xuhua. All rights reserved.
//

#ifndef card_DB_h
#define card_DB_h

#include <core/cxSqlite.h>
#include "Data.h"

CX_CPP_BEGIN

#define TIME_MINUTE  (60)
#define TIME_HOUR    (60*TIME_MINUTE)
#define TIME_DAY     (24*TIME_HOUR)

class DB;

//更新
class UpData : public cxObject
{
public:
    CX_DECLARE(UpData);
protected:
    explicit UpData();
    virtual ~UpData();
private:
    DB *db;
    cxStr *key;
    cxInt version;
    cxStr *dataId;
    cxInt64 time;
public:
    //获取数据
    Data *GetData(const cxStr *data);
    
    const cxStr *Key();
    const cxStr *DataId();
    
    void SetVersion(cxInt v);
    void SetDataId(const cxStr *v);
    void SetTime(cxInt64 v);
    cxBool Insert();
    cxBool Delete();
public:
    static UpData *Create(DB *db,cchars key);
};

//词卡索引
class Word : public cxObject
{
public:
    friend DB;
    CX_DECLARE(Word);
protected:
    explicit Word();
    virtual ~Word();
private:
    Data *data;
    DB *db;
    cxStr *key;
    cxInt level;
    cxInt group;
    cxBool isbuy;
    cxBool isuse;
    cxInt64 time;
    cxInt64 last;
public:
    void SetGroup(cxInt v);
    //获取词卡数据
    Data *GetData();
    //设置可用值
    void SetValue(cxSqlStmt *stmt);
    //是否存在,如果存在设置字段值
    cxBool Exists();
    //获得下次复习时间
    cxInt64 NextTime();
    //插入库
    cxBool Insert();
public:
    static Word *Create(DB *db,cchars key);
    static Word *Create(DB *db,const cxStr *key);
    static Word *Create(DB *db,Data *data);
};

//词卡数据库
class DB : public cxSqlite
{
public:
    CX_DECLARE(DB);
protected:
    explicit DB();
    virtual ~DB();
private:
    cxInt64 LVS[9];
    cxInt maxVer;
public:
    //有数据
    cxBool HasData(const cxStr *key);
    //获得数据
    const cxStr *ReadData(const cxStr *key);
    //写入数据
    cxBool WriteData(const cxStr *key,const cxStr *data);
    //获得更新数量
    cxInt UpDataCount();
    //获得一个需要更新的单词名称
    UpData *OneUpData();
    //获取count个需要复习的词卡
    cxArray *ReviewWords(cxInt count);
    //获取数据版本
    cxInt DataVersion();
    cxBool SetDataVersion(cxInt ver);
    //获取用户Id
    cxBool SetBindId(const cxStr *id);
    const cxStr *BindId();
    //设置临时id
    cxBool SetTempId(const cxStr *id);
    const cxStr *TempId();
    //获得当前时间
    cxInt64 TimeNow();
    //根据当前等级获得复习时间
    cxInt64 LevelTime(cxInt level);
    //获取或设置配置值
    const cxStr *GetValue(cchars key);
    cxBool SetValue(cchars key,const cxStr *value);
    //初始化数据库
    cxBool Init();
};

CX_CPP_END

#endif


