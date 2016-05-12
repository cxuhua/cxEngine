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
    cxInt64 Time;
    cxStr *Public;
public:
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
    ListServers *Init(const cxStr *txt);
    //查询最适合的服务器
    const ServerInfo *Query();
};

CX_CPP_END

#endif


