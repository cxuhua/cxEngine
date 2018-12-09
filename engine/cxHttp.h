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
public:
    struct WSHeaderType {
        unsigned header_size;
        bool fin;
        bool mask;
        enum OPCodeType {
            CONTINUATION = 0x0,
            TEXT_FRAME = 0x1,
            BINARY_FRAME = 0x2,
            CLOSE = 0x8,
            PING = 0x9,
            PONG = 0xa,
        } opcode;
        int N0;
        uint64_t N;
        uint8_t masking_key[4];
    };
protected:
    explicit cxHttp();
    virtual ~cxHttp();
protected:
    //for websocket
    virtual void OnFrame(WSHeaderType::OPCodeType type,cchars data,cxInt len);
    //
    virtual void OnSwitchProto();
    virtual void OnCompleted();
    virtual void OnHeader();
    virtual void OnStart();
    virtual void OnBody(cchars data,cxInt len);
    virtual void OnFile(const cxStr *path,cxInt64 size);
    virtual void OnProgress(cxInt64 len,cxInt64 cur);
    virtual void OnConnected();
    virtual void OnData(char *buffer,cxInt size);
    virtual void OnClose();
    virtual void OnWillClose();
private:
    cxBool useMask;
    cxInt parseWebsocket();
public:
    void SetUseMask(cxBool v);
    cxBool WriteFrame(WSHeaderType::OPCodeType type,const cxStr *data);
    cxBool WriteFrame(const cxJson *jv);
private:
    //保存路径
    cxStr *spath;
    cxStr *smd5;
    FILE *fd;
    cxInt64 filesize;
    cxInt64 downsize;
    cxBool initFile();
    void closeFile();
    void writeFile(cchars data,cxInt size);    
    
    cxBool ishttpproto;
    cxUInt64 contentLength;
    cxUInt16 status;
    cxBool success;
    cxHttpMethod method;
    http_parser parser;
    http_parser_settings settings;
    cxStr *path;
    cxStr *schema;
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
    cxEvent<cxHttp, cxInt64,cxInt64> onProgress;
public:
    cxEvent<cxHttp, WSHeaderType::OPCodeType,cchars,cxInt> onFrame;
public:
    cxBool IsWebSocket() const;
    cxHttp *SetFileInfo(const cxStr *path,const cxStr *md5=nullptr);
    cxBool ConnectURL(cchars url);
    cxHash *ReqHeads();
    cxHash *ResHeads();
    const cxInt Status() const;
    const cxStr *Body() const;
    const cxBool Success() const;
    const cxStr *FileMD5() const;
    const cxStr *FilePath() const;
public:
    static cxHttp *Post(cchars url,const cxStr *post);
    static cxHttp *Get(cchars url);
    static cxHttp *WebSocket(cchars url);
    //支持断点续传 成功下载时只需OnFile
    static cxHttp *LoadFile(cchars url,const cxStr *path,const cxStr *md5);
};

CX_CPP_END

#endif


