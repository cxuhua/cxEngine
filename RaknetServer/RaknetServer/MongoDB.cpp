//
//  DB.cpp
//  RaknetServer
//
//  Created by xuhua on 5/9/16.
//  Copyright © 2016 xuhua. All rights reserved.
//


#include "MongoDB.h"

CX_CPP_BEGIN

CX_IMPLEMENT(MongoDB);

MongoDB::MongoDB()
{
    dbName = DB_NAME;
    std::string errmsg;
    ConnectionString cs = ConnectionString::parse(DB_HOST, errmsg);
    if (!cs.isValid()) {
        CX_ERROR("DB_HOST host not valid");
        exit(1);
    }
    db = cs.connect(errmsg);
    if(db == nullptr){
        CX_ERROR("conn server error :%s",errmsg.c_str());
        exit(1);
    }
}

MongoDB::~MongoDB()
{
    delete db;
}

std::auto_ptr<DBClientCursor> MongoDB::Find(const std::string& c,Query query,int limit,int skip,const BSONObj* fieldsToReturn,int queryOptions,int batchSize)
{
    return db->query(NS(c), query, limit, skip, fieldsToReturn, queryOptions, batchSize);
}

cxInt64 MongoDB::Count(const std::string& c,const Query& query ,int skip,int limit,int opts)
{
    return db->count(NS(c), query, opts, limit, skip);
}

void MongoDB::Remove(const std::string& c,Query q,bool justOne,const WriteConcern* wc)
{
    db->remove(NS(c), q, justOne, wc);
}

void MongoDB::RemoveAll(const std::string& c,Query q)
{
    Remove(c, q, false, NULL);
}

void MongoDB::Insert(const std::string& c,const std::vector<BSONObj> &obj,int flags,const WriteConcern* wc)
{
    db->insert(NS(c), obj, flags, wc);
}

void MongoDB::Insert(const std::string& c,BSONObj obj,int flags,const WriteConcern* wc)
{
    db->insert(NS(c), obj, flags, wc);
}

void MongoDB::Upsert(const std::string& c,Query query,BSONObj obj)
{
    Update(c, query, obj, true, false, NULL);
}

void MongoDB::UpdateAll(const std::string& c,Query query,BSONObj obj)
{
    Update(c, query, obj, false, true, NULL);
}

void MongoDB::Update(const std::string& c,Query query,BSONObj obj,bool upsert,bool multi,const WriteConcern* wc)
{
    db->update(NS(c), query, obj, upsert, multi, wc);
}

CX_CPP_END

