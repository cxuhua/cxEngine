//
//  ListServers.cpp
//  cxEngineTest
//
//  Created by xuhua on 5/9/16.
//  Copyright © 2016 xuhua. All rights reserved.
//


#include "ListServers.h"

CX_CPP_BEGIN

/*
 {"items":[
 {
 "id":"test",
 "host":"192.168.199.244",
 "port":9000,
 "pass":"123",
 "max":512,
 "curr":0,
 "time":1462783353,
 "public":"AN1fTJdhgEDjEmYx/oE2gnoQ3hxVDOMvJSra6af15x5DpHDbxJ3g6JJUaA9Tv68h44GgIMV/zmDI0EBLm+GY0Q=="
 }]
 */

CX_IMPLEMENT(ServerInfo)

ServerInfo::ServerInfo()
{
    Port = 0;
    Max = 0;
    Curr = 0;
    Time = 0;
    Id = nullptr;
    Host = nullptr;
    Pass = nullptr;
    Public = nullptr;
}

ServerInfo::~ServerInfo()
{
    cxObject::release(&Public);
    cxObject::release(&Id);
    cxObject::release(&Host);
    cxObject::release(&Pass);
}

ServerInfo *ServerInfo::Init(const cxJson *json)
{
    cxObject::swap(&Id, json->Get("id"));
    cxObject::swap(&Host, json->Get("host"));
    cxObject::swap(&Pass, json->Get("pass"));
    const cxStr *pub = json->Get("public");
    cxObject::swap(&Public, pub->Base64Decode());
    Port    = json->Get("port", Port);
    Max     = json->Get("max", Max);
    Curr    = json->Get("curr", Curr);
    Time    = json->Get("time", Time);
    return this;
}

CX_IMPLEMENT(ListServers);

ListServers::ListServers()
{
    Error = nullptr;
    Items = cxArray::Alloc();
}

ListServers::~ListServers()
{
    Items->Release();
    cxObject::release(&Error);
}

ListServers *ListServers::Init(const cxStr *txt)
{
    cxJson *json = cxJson::Create()->From(txt);
    if(json == nullptr){
        return nullptr;
    }
    Code = json->Get("code", 0);
    cxObject::swap(&Error, json->Get("error"));
    const cxJson *items = json->At("items");
    if(items == nullptr || !items->IsArray() || items->Size() == 0){
        CX_LOGGER("server list empty");
        return this;
    }
    for(cxJson::Iter it=items->Begin(); it!=items->End(); it++){
        const cxJson *item = it.Value();
        ServerInfo *info = ServerInfo::Alloc()->Init(item);
        Items->Append(info);
        info->Release();
        CX_LOGGER("Servers: + %s",item->Dumps()->ToString());
    }
    return this;
}

const ServerInfo *ListServers::Query()
{
    if(Items->Size() == 0){
        return nullptr;
    }
    return Items->At(0)->To<ServerInfo>();
}
CX_CPP_END

