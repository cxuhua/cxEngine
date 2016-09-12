//
//  ListServers.h
//  cxEngineTest
//
//  Created by xuhua on 5/9/16.
//  Copyright © 2016 xuhua. All rights reserved.
//

#ifndef cxEngineTest_ListServers_h
#define cxEngineTest_ListServers_h

#include <core/cxArray.h>
#include <core/cxJson.h>
#include "cxRaknet.h"

CX_CPP_BEGIN

#define MAX_PING    4000

class ServerInfo : public cxObject
{
public:
    CX_DECLARE(ServerInfo);
protected:
    explicit ServerInfo();
    virtual ~ServerInfo();
private:
public:
    cxStr *Id;
    cxStr *Host;
    cxInt Port;
    cxStr *Pass;
    cxInt Max;
    cxInt Curr;
    cxInt Attr;
    cxInt64 Time;
    cxStr *Public;
    const cxStr *ToString() const;
    cxInt Ping;
    cxInt PingNum;
public:
    cxInt GetPing();
    ServerInfo *Init(const cxJson *json);
};

class ListServers : public cxObject
{
public:
    CX_DECLARE(ListServers);
protected:
    explicit ListServers();
    virtual ~ListServers();
private:
public:
    cxArray *Items;//ServerInfo
    cxInt Code;
    cxStr *Error;
public:
    static cxInt sortItem(const void *lp, const void *rp);
    //
    void Each(std::function<cxBool(ServerInfo *)> func);
    //排序
    void Sort();
    //设置ping
    ServerInfo *SetPing(RakNet::SystemAddress addr,cxInt ping);
    //初始化
    ListServers *Init(const cxStr *txt);
    //查询最适合的服务器
    ServerInfo *Query(cxInt attr);
    //获取服务器信息
    ServerInfo *Query(cchars sid);
    //根据地质查询
    ServerInfo *Query(RakNet::SystemAddress addr);
public:
    const cxJson *ToJson();
};

CX_CPP_END

#endif


