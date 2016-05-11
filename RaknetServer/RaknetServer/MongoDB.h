//
//  DB.h
//  RaknetServer
//
//  Created by xuhua on 5/9/16.
//  Copyright © 2016 xuhua. All rights reserved.
//

#ifndef RaknetServer_DB_h
#define RaknetServer_DB_h

#include <mongo/client/dbclient.h>
#include <core/cxObject.h>

//数据库连接
#define DB_HOST     "mongodb://192.168.199.244:27017"

//服务器表
#define DB_NAME        "raknet"
#define T_SERVERS      "servers"

using namespace mongo;

CX_CPP_BEGIN

class DBQuery : public cxObject
{
public:
    CX_DECLARE(DBQuery);
protected:
    explicit DBQuery();
    virtual ~DBQuery();
};

class MongoDB : public cxObject
{
public:
    CX_DECLARE(MongoDB);
protected:
    explicit MongoDB();
    virtual ~MongoDB();
private:
    std::string dbName;
    DBClientBase *db;
    std::string NS(const std::string& c)
    {
        if(dbName.size() == 0)return c;
        return dbName + "." + c;
    };
public:
    template<class T>
    BSONObj FindId(const std::string& c,T v)
    {
        BSONObj q = BSON("_id" << v);
        return db->findOne(NS(c), q);
    }
    std::auto_ptr<DBClientCursor> Find(const std::string& c,Query query,int skip=0,int limit=0,const BSONObj* fieldsToReturn = 0,int queryOptions = 0,int batchSize = 0);
    
    cxInt64 Count(const std::string& c,const Query& query ,int skip,int limit,int opts=0);
    
    void Insert(const std::string& c,const std::vector<BSONObj> &obj,int flags = 0,const WriteConcern* wc = NULL);
    void Insert(const std::string& c,BSONObj obj,int flags = 0,const WriteConcern* wc = NULL);
    
    void Upsert(const std::string& c,Query query,BSONObj obj);
    template<class T>
    void UpdateId(const std::string& c,T v,BSONObj obj)
    {
        BSONObj q = BSON("_id" << v);
        Update(c, q, obj);
    }
    void UpdateAll(const std::string& c,Query query,BSONObj obj);
    void Update(const std::string& c,Query query,BSONObj obj,bool upsert=false,bool multi=false,const WriteConcern* wc=NULL);
    
    void Remove(const std::string& c,Query q,bool justOne = 0,const WriteConcern* wc = NULL);
    void RemoveAll(const std::string& c,Query q);
    template<class T>
    void RemoveId(const std::string& c,T v)
    {
        BSONObj q = BSON("_id" << v);
        Remove(c, q, true);
    }
};

CX_CPP_END

#endif


