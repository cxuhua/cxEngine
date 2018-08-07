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
#include <core/cxJson.h>
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
    virtual void OnBody(cchars data,cxInt len);
    virtual void OnFile(const cxStr *path,cxInt64 size);
    void OnConnected();
    void OnData(char *buffer,cxInt size);
    void OnClose();
private:
    //保存路径
    cxStr *spath;
    cxStr *smd5;
    FILE *fd;
    cxBool initFile();
    void closeFile();
    void writeFile(cchars data,cxInt size);    
    
    cxUInt64 contentLength;
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
    cxStr *field;
    cxStr *host;
    cxInt port;
    static int onChunkHeader(http_parser *parser);
    static int onChunkComplete(http_parser *parser);
    static int onBodyFunc(http_parser *parser, const char *at, size_t length);
    static int onHeadField(http_parser *parser, const char *at, size_t length);
    static int onHeadValue(http_parser *parser, const char *at, size_t length);
    static int messageBegin(http_parser *parser);
    static int headCompleted(http_parser *parser);
    static int messageCompleted(http_parser *parser);
public:
    cxEvent<cxHttp> onSuccess;
    cxEvent<cxHttp> onError;
    cxEvent<cxHttp, const cxStr *, cxInt64> onFile;
public:
    cxHttp *SetFileInfo(const cxStr *path,const cxStr *md5=nullptr);
    cxBool ConnectURL(cchars url);
    cxHash *ReqHeads();
    cxHash *ResHeads();
    const cxInt Status() const;
    const cxStr *Body() const;
    const cxJson *Json() const;
    const cxBool Success() const;
public:
    static cxHttp *Post(cchars url,const cxStr *post);
    static cxHttp *Get(cchars url);
    //支持断点续传 成功下载时只需OnFile
    static cxHttp *LoadFile(cchars url,const cxStr *path,const cxStr *md5);
};

CX_CPP_END

#endif


