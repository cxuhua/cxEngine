//
//  cxHttp.h
//  cxEngineCore
//
//  Created by xuhua on 7/3/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#ifndef cxEngineCore_cxHttp_h
#define cxEngineCore_cxHttp_h

#include <ext/http_parser.h>
#include <core/cxHash.h>
#include "cxTcp.h"

CX_CPP_BEGIN

typedef enum http_method cxHttpMethod;

class cxHttp : public cxTcp
{
public:
    CX_DECLARE(cxHttp);
protected:
    explicit cxHttp();
    virtual ~cxHttp();
protected:
    virtual void OnCompleted();
    virtual void OnHeader();
    virtual void OnStart();
    void OnConnected();
    void OnData(char *buffer,cxInt size);
    void OnClose();
private:
    cxUInt16 status;
    cxBool success;
    cxHttpMethod method;
    http_parser parser;
    http_parser_settings settings;
    cxStr *path;
    cxStr *body;
    cxStr *post;
    cxHash *reqHeads;
    cxHash *resHeads;
    cxStr *data;
    cxStr *field;
    cxInt offset;
    cxStr *host;
    cxInt port;
    static int onBodyFunc(http_parser *parser, const char *at, size_t length);
    static int onHeadField(http_parser *parser, const char *at, size_t length);
    static int onHeadValue(http_parser *parser, const char *at, size_t length);
    static int messageBegin(http_parser *parser);
    static int headCompleted(http_parser *parser);
    static int messageCompleted(http_parser *parser);
    void connect(const char *url);
public:
    cxEvent<cxHttp> onSuccess;
    cxEvent<cxHttp> onError;
public:
    cxHash *ReqHeads();
    cxHash *ResHeads();
    const cxInt Status() const;
    const cxStr *Body() const;
    const cxBool Success() const;
public:
    static cxHttp *Post(cchars url,cxStr *post);
    static cxHttp *Get(cchars url);
};

CX_CPP_END

#endif


