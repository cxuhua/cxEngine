//
//  DB.cpp
//  RaknetServer
//
//  Created by xuhua on 5/9/16.
//  Copyright © 2016 xuhua. All rights reserved.
//


#include "DB.h"

CX_CPP_BEGIN

CX_IMPLEMENT(DB);

DB::DB()
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

DB::~DB()
{
    delete db;
}

cxInt64 DB::Count(const std::string& c,const Query& query ,int skip,int limit,int opts)
{
    return db->count(NS(c), query, opts, limit, skip);
}

void DB::Remove(const std::string& c,Query q,bool justOne,const WriteConcern* wc)
{
    db->remove(NS(c), q, justOne, wc);
}

void DB::RemoveAll(const std::string& c,Query q)
{
    Remove(c, q, false, NULL);
}

void DB::Insert(const std::string& c,const std::vector<BSONObj> &obj,int flags,const WriteConcern* wc)
{
    db->insert(NS(c), obj, flags, wc);
}

void DB::Insert(const std::string& c,BSONObj obj,int flags,const WriteConcern* wc)
{
    db->insert(NS(c), obj, flags, wc);
}

void DB::Upsert(const std::string& c,Query query,BSONObj obj)
{
    Update(c, query, obj, true, false, NULL);
}

void DB::UpdateAll(const std::string& c,Query query,BSONObj obj)
{
    Update(c, query, obj, false, true, NULL);
}

void DB::Update(const std::string& c,Query query,BSONObj obj,bool upsert,bool multi,const WriteConcern* wc)
{
    db->update(NS(c), query, obj, upsert, multi, wc);
}

CX_CPP_END

