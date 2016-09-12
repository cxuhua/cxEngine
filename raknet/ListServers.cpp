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
    PingNum = 0;
    Port = 0;
    Attr = 0;
    Max = 0;
    Curr = 0;
    Time = 0;
    CX_SET_NULL(Id);
    CX_SET_NULL(Host);
    CX_SET_NULL(Pass);
    CX_SET_NULL(Public);
}

ServerInfo::~ServerInfo()
{
    CX_RELEASE(Public);
    CX_RELEASE(Id);
    CX_RELEASE(Host);
    CX_RELEASE(Pass);
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

cxInt ServerInfo::GetPing()
{
    if(PingNum == 0){
        return Ping;
    }
    return Ping / PingNum;
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
    return l->GetPing() < r->GetPing();
}

void ListServers::Sort()
{
    Items->Sort(ListServers::sortItem);
}

const cxJson *ListServers::ToJson()
{
    cxJson *ret = cxJson::CreateArray();
    for(cxArray::FIter it=Items->FBegin();it!=Items->FEnd();it++){
        ServerInfo *info = (*it)->To<ServerInfo>();
        cxJson *item = cxJson::Alloc()->Object();
        item->Set("id", info->Id);
        item->Set("ping", info->Ping);
        item->Set("host", info->Host);
        item->Set("port", info->Port);
        ret->Append(item);
        item->Release();
    }
    return ret;
}

void ListServers::Each(std::function<cxBool(ServerInfo *)> func)
{
    for(cxArray::FIter it=Items->FBegin();it!=Items->FEnd();it++){
        ServerInfo *info = (*it)->To<ServerInfo>();
        if(!func(info)){
            break;
        }
    }
}

ServerInfo *ListServers::Query(RakNet::SystemAddress addr)
{
    for(cxArray::FIter it=Items->FBegin();it!=Items->FEnd();it++){
        ServerInfo *info = (*it)->To<ServerInfo>();
        if(info->ToString()->IsEqu(addr.ToString())){
            return info;
        }
    }
    return nullptr;
}

ServerInfo *ListServers::SetPing(RakNet::SystemAddress addr,cxInt ping)
{
    ServerInfo *info = Query(addr);
    CX_ASSERT(info != nullptr, "server info miss");
    info->PingNum ++;
    if(info->PingNum == 1){
        info->Ping = ping;
    }else{
        info->Ping += ping;
    }
    return info;
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

ServerInfo *ListServers::Query(cchars sid)
{
    for(cxArray::FIter it=Items->FBegin();it!=Items->FEnd();it++){
        ServerInfo *info = (*it)->To<ServerInfo>();
        if(info->Id->IsEqu(sid)){
            return info;
        }
    }
    return nullptr;
}

ServerInfo *ListServers::Query(cxInt attr)
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
        if(info->GetPing() >= MAX_PING){
            continue;
        }
        if(info->GetPing() < ping){
            ret = info;
            ping = info->GetPing();
        }
    }
    return ret;
}
CX_CPP_END

