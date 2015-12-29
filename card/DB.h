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
    
    cxBool Exists();
    cxBool Insert();
    cxBool Delete();
public:
    static UpData *Create(DB *db,cchars key);
    static UpData *Create(DB *db,const cxStr *key);
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
    cxInt maxVer;
public:
    //有数据
    cxBool HasData(const cxStr *key);
    //获得数据
    const cxStr *ReadData(const cxStr *key);
    //获得单词数据
    Data *WordData(const cxStr *key);
    //写入数据
    cxBool WriteData(const cxStr *key,const cxStr *data);
    //获得更新数量
    cxInt UpDataCount();
    //获得一个需要更新的单词名称
    UpData *OneUpData();
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
    //获取或设置配置值
    const cxStr *GetValue(cchars key);
    cxBool SetValue(cchars key,const cxStr *value);
    //初始化数据库
    cxBool Init();
};

CX_CPP_END

#endif


