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
    Ping = MAX_PING;
    Port = 0;
    Attr = 0;
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

//192.168.199.244|9000
const cxStr *ServerInfo::ToString() const
{
    return cxStr::UTF8("%s|%d",Host->ToString(),Port);
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
    Attr    = json->Get("attr", Attr);
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

cxInt ListServers::sortItem(const void *lp, const void *rp)
{
    ServerInfo *l = *(ServerInfo **)lp;
    ServerInfo *r = *(ServerInfo **)rp;
    return l->Ping < r->Ping;
}

void ListServers::SetPing(RakNet::SystemAddress addr,cxInt ping)
{
    for(cxArray::FIter it=Items->FBegin();it!=Items->FEnd();it++){
        ServerInfo *info = (*it)->To<ServerInfo>();
        if(info->ToString()->IsEqu(addr.ToString())){
            info->Ping = ping;
            break;
        }
    }
    Items->Sort(ListServers::sortItem);
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

const ServerInfo *ListServers::Query(cxInt attr)
{
    cxInt ping = MAX_PING;
    ServerInfo *ret = nullptr;
    for(cxArray::FIter it=Items->FBegin();it!=Items->FEnd();it++){
        ServerInfo *info = (*it)->To<ServerInfo>();
        if(attr > 0 && (info->Attr & attr) == 0){
            continue;
        }
        if(info->Curr >= info->Max){
            continue;
        }
        if(info->Ping >= MAX_PING){
            continue;
        }
        if(info->Ping < ping){
            ret = info;
            ping = info->Ping;
        }
    }
    return ret;
}
CX_CPP_END

