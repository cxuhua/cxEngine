//
//  DB.h
//  card
//
//  Created by xuhua on 12/14/15.
//  Copyright © 2015 xuhua. All rights reserved.
//

#ifndef card_DB_h
#define card_DB_h

#include <core/cxStr.h>
#include <core/cxSqlite.h>
#include <core/cxArray.h>

CX_CPP_BEGIN

#define TIME_MINUTE  (60)
#define TIME_HOUR    (60*TIME_MINUTE)
#define TIME_DAY     (24*TIME_HOUR)

class DB;

class Word : public cxObject
{
public:
    friend DB;
    CX_DECLARE(Word);
protected:
    explicit Word();
    virtual ~Word();
private:
    DB *db;
    cxStr *key;
    cxInt level;
    cxInt ver;
    cxInt group;
    cxBool isbuy;
    cxBool isuse;
    cxInt64 time;
public:
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
};

class DB : public cxSqlite
{
public:
    CX_DECLARE(DB);
protected:
    explicit DB();
    virtual ~DB();
private:
    cxInt64 LVS[9];
public:
    //获取count个需要复习的词卡
    cxArray *ReviewWords(cxInt count);
    //获取数据版本
    cxInt DataVersion();
    void SetDataVersion(cxInt ver);
    //获取用户Id
    void SetUserId(const cxStr *id);
    const cxStr *UserId();
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


