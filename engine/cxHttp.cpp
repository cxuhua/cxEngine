//
//  cxHttp.cpp
//  cxEngineCore
//
//  Created by xuhua on 7/3/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#include <unistd.h>
#include <fcntl.h>
#include <core/cxUtil.h>
#include "cxHttp.h"

CX_CPP_BEGIN

CX_IMPLEMENT(cxHttp);

cxBool cxHttp::IsWebSocket() const
{
    return schema != nullptr && (schema->IsEqu("ws") || schema->IsEqu("wss"));
}

//返回消耗的字节数
cxInt cxHttp::parseWebsocket()
{
    if(body->Size() < 2){
        return 0;
    }
    cxUInt8 *data = (cxUInt8 *)body->Data();
    WSHeaderType ws;
    ws.fin = (data[0] & 0x80) == 0x80;
    ws.opcode = (WSHeaderType::OPCodeType)(data[0] & 0x0f);
    ws.mask = (data[1] & 0x80) == 0x80;
    ws.N0 = (data[1] & 0x7f);
    ws.header_size = 2 + (ws.N0 == 126? 2 : 0) + (ws.N0 == 127? 8 : 0) + (ws.mask? 4 : 0);
    if(body->Size() < ws.header_size){
        return 0;
    }
    int i = 0;
    if (ws.N0 < 126) {
        ws.N = ws.N0;
        i = 2;
    }else if (ws.N0 == 126) {
        ws.N = 0;
        ws.N |= ((uint64_t) data[2]) << 8;
        ws.N |= ((uint64_t) data[3]) << 0;
        i = 4;
    }else if (ws.N0 == 127) {
        ws.N = 0;
        ws.N |= ((uint64_t) data[2]) << 56;
        ws.N |= ((uint64_t) data[3]) << 48;
        ws.N |= ((uint64_t) data[4]) << 40;
        ws.N |= ((uint64_t) data[5]) << 32;
        ws.N |= ((uint64_t) data[6]) << 24;
        ws.N |= ((uint64_t) data[7]) << 16;
        ws.N |= ((uint64_t) data[8]) << 8;
        ws.N |= ((uint64_t) data[9]) << 0;
        i = 10;
    }
    if (ws.mask) {
        ws.masking_key[0] = ((uint8_t) data[i+0]) << 0;
        ws.masking_key[1] = ((uint8_t) data[i+1]) << 0;
        ws.masking_key[2] = ((uint8_t) data[i+2]) << 0;
        ws.masking_key[3] = ((uint8_t) data[i+3]) << 0;
    }else {
        ws.masking_key[0] = 0;
        ws.masking_key[1] = 0;
        ws.masking_key[2] = 0;
        ws.masking_key[3] = 0;
    }
    cxInt flen = ws.header_size + ws.N;
    if(body->Size() < flen) {
        return 0;
    }
    if (ws.opcode != WSHeaderType::CLOSE){
        if(ws.mask){
            for(size_t i = 0; i < ws.N; i++){
                data[i+ws.header_size] ^= ws.masking_key[i & 0x3];
            }
        }
        if(ws.fin){
            OnFrame(ws.opcode, (cchars)&data[ws.header_size], ws.N);
        }
    }else {
        Close(0);
    }
    return flen;
}

cxBool cxHttp::WriteFrame(const cxJson *jv)
{
    const cxStr *jtxt = jv->Dumps();
    if(!cxStr::IsOK(jtxt)){
        return false;
    }
    return WriteFrame(WSHeaderType::TEXT_FRAME, jtxt);
}

cxBool cxHttp::WriteFrame(WSHeaderType::OPCodeType type,const cxStr *data)
{
    if(data->IsEmpty()){
        return true;
    }
    const cxByte masking_key[4] = { 0x12, 0x34, 0x56, 0x78 };
    cxStr *d = cxStr::Create();
    d->Append((cxByte)(0x80|type));
    cxInt64 msiz = data->Size();
    if(msiz < 126){
        cxByte h1 = (msiz & 0xff) | (useMask ? 0x80 : 0);
        d->Append(h1);
    }else if(msiz < 65536){
        cxByte h1 = 126 | (useMask ? 0x80 : 0);
        d->Append(h1);
        cxByte h2 = (msiz >> 8) & 0xff;
        d->Append(h2);
        cxByte h3 = (msiz >> 0) & 0xff;
        d->Append(h3);
    }else {
        cxByte h1 = 127 | (useMask ? 0x80 : 0);
        d->Append(h1);
        cxByte h2 = (msiz >> 56) & 0xff;
        d->Append(h2);
        cxByte h3 = (msiz >> 48) & 0xff;
        d->Append(h3);
        cxByte h4 = (msiz >> 40) & 0xff;
        d->Append(h4);
        cxByte h5 = (msiz >> 32) & 0xff;
        d->Append(h5);
        cxByte h6 = (msiz >> 24) & 0xff;
        d->Append(h6);
        cxByte h7 = (msiz >> 16) & 0xff;
        d->Append(h7);
        cxByte h8 = (msiz >>  8) & 0xff;
        d->Append(h8);
        cxByte h9 = (msiz >>  0) & 0xff;
        d->Append(h9);
    }
    if(useMask){
        d->Append(masking_key[0]);
        d->Append(masking_key[1]);
        d->Append(masking_key[2]);
        d->Append(masking_key[3]);
    }
    d->Append(data);
    if(useMask){
        cxInt off = d->Size() - data->Size();
        cxUInt8 *ptr = (cxUInt8 *)d->Data() + off;
        for(cxInt i=0;i < data->Size();i++){
            ptr[i] ^= masking_key[i & 0x3];
        }
    }
    return Write(d);
}

void cxHttp::SetUseMask(cxBool v)
{
    useMask = v;
}

void cxHttp::OnFrame(WSHeaderType::OPCodeType type,cchars data,cxInt len)
{
    onFrame.Fire(this, type, data, len);
}

void cxHttp::OnWillClose()
{
    if(IsWebSocket()){
        uint8_t closeFrame[6] = {0x88, 0x80, 0x00, 0x00, 0x00, 0x00};
        cxStr *d = cxStr::Create();
        d->Append(closeFrame, 6);
        Write(d);
    }
}

void cxHttp::OnBody(cchars data,cxInt len)
{
    if(len == 0){
        return;
    }
    if(IsWebSocket()){
        //web socket
        body->Append(data, len);
        int ret = parseWebsocket();
        if(ret > 0){
            body->Erase(0, ret);
        }
    }else if(fd != NULL){
        //download file
        writeFile(data, len);
    }else{
        //get all body
        body->Append(data, len);
    }
    downsize += len;
    OnProgress(contentLength + filesize,downsize + filesize);
}

int cxHttp::onBodyFunc(http_parser *parser, const char *at, size_t length)
{
    cxHttp *http = static_cast<cxHttp *>(parser->data);
    http->OnBody(at, (cxInt)length);
    return 0;
}

int cxHttp::onHeadField(http_parser *parser, const char *at, size_t length)
{
    cxHttp *http = static_cast<cxHttp *>(parser->data);
    http->field->Append(at, (cxInt)length);
    return 0;
}

int cxHttp::onHeadValue(http_parser *parser, const char *at, size_t length)
{
    cxHttp *http = static_cast<cxHttp *>(parser->data);
    cchars key = http->field->Data();
    cxStr *data = cxStr::Create()->Init((cxAny)at,(cxInt)length);
    http->resHeads->Set(key, data);
    http->field->Clear();
    return 0;
}

int cxHttp::messageBegin(http_parser *parser)
{
    cxHttp *http = static_cast<cxHttp *>(parser->data);
    http->downsize = 0;
    http->OnStart();
    return 0;
}

int cxHttp::headCompleted(http_parser *parser)
{
    cxHttp *http = static_cast<cxHttp *>(parser->data);
    http->OnHeader();
    return 0;
}

int cxHttp::messageCompleted(http_parser *parser)
{
    cxHttp *http = static_cast<cxHttp *>(parser->data);
    http->status = parser->status_code;
    if(http->status == 101){
        http->ishttpproto = false;
        http->success = true;
        http->OnSwitchProto();
    }else{
        http->success = (http->status >= 200 && http->status < 300);
        http->closeFile();
        http->OnCompleted();
    }
    return 0;
}

int cxHttp::onChunkHeader(http_parser *parser)
{
    return 0;
}

int cxHttp::onChunkComplete(http_parser *parser)
{
    return 0;
}

void cxHttp::OnData(char *buffer,cxInt size)
{
    cxInt len = size;
    cxInt off = 0;
    while(off < len && ishttpproto){
        off += http_parser_execute(&parser, &settings, buffer + off, len - off);
    }
    if(len > off){
        OnBody(buffer + off, len - off);
    }
}

cxHttp::cxHttp()
{
    http_parser_init(&parser, HTTP_RESPONSE);
    http_parser_settings_init(&settings);
    success = false;
    parser.data = this;
    settings.on_body = onBodyFunc;
    settings.on_header_field = onHeadField;
    settings.on_header_value = onHeadValue;
    settings.on_message_begin = messageBegin;
    settings.on_headers_complete = headCompleted;
    settings.on_message_complete = messageCompleted;
    settings.on_chunk_header = onChunkHeader;
    settings.on_chunk_complete = onChunkComplete;
    
    useMask = true;
    ishttpproto = true;
    method = HTTP_GET;
    spath = nullptr;
    path = nullptr;
    post = nullptr;
    host = nullptr;
    schema = nullptr;
    fd = NULL;
    status = 0;
    smd5 = nullptr;
    contentLength = 0;
    downsize = 0;
    filesize = 0;
    
    body = cxStr::Alloc();
    reqHeads = cxHash::Alloc();
    resHeads = cxHash::Alloc();
    field = cxStr::Alloc();
}

cxHttp::~cxHttp()
{
    closeFile();
    cxObject::release(&schema);
    cxObject::release(&smd5);
    cxObject::release(&spath);
    cxObject::release(&host);
    cxObject::release(&path);
    cxObject::release(&post);
    field->Release();
    body->Release();
    reqHeads->Release();
    resHeads->Release();
}

cxHttp *cxHttp::SetFileInfo(const cxStr *path,const cxStr *md5)
{
    CX_ASSERT(cxStr::IsOK(path), "path error");
    CX_ASSERT(cxStr::IsOK(md5), "md5 error");
    cxObject::swap(&spath, path);
    cxObject::swap(&smd5, md5);
    return this;
}

void cxHttp::OnFile(const cxStr *path,cxInt64 size)
{
    onFile.Fire(this, path, size);
}

void cxHttp::OnProgress(cxInt64 len,cxInt64 cur)
{
    onProgress.Fire(this, len, cur);
}

void cxHttp::closeFile()
{
    if(fd == NULL){
        return;
    }
    fclose(fd);
    fd = NULL;
    char file[PATH_MAX]={0};
    snprintf(file, PATH_MAX, "%s.tmp",spath->ToChars());
    if(!cxStr::IsOK(smd5)){
        return;
    }
    cxInt64 size = cxUtil::ValidFile(file, smd5->ToChars());
    //校验失败，删除文件
    if(size <= 0){
        remove(file);
        return;
    }
    //成功重命名成需要的名称
    rename(file, spath->ToChars());
    OnFile(spath,size);
}

void cxHttp::writeFile(cchars data,cxInt size)
{
    fwrite(data, size, 1, fd);
}

cxBool cxHttp::initFile()
{
    //未设置文件下载
    if(!cxStr::IsOK(spath) || !cxStr::IsOK(smd5)){
        return true;
    }
    //检测原文件是否成功
    cxInt64 fsiz = cxUtil::ValidFile(spath->ToChars(), smd5->ToChars());
    if(fsiz > 0){
        success = true;
        OnFile(spath,fsiz);
        return false;
    }
    //检测临时文件是否成功
    char file[PATH_MAX]={0};
    snprintf(file, PATH_MAX, "%s.tmp",spath->ToChars());
    fsiz = cxUtil::GetFileSize(file);
    if(fsiz >= 0 && cxUtil::ValidFile(file, smd5->ToChars())){
        rename(file, spath->ToChars());
        success = true;
        OnFile(spath,fsiz);
        return false;
    }
    //继续下载
    if(fsiz >= 0){
        filesize = fsiz;
        fd = fopen(file, "ab+");
        reqHeads->Set("Range", cxStr::UTF8("bytes=%lld-",fsiz));
    }else{
        filesize = 0;
        fd = fopen(file, "wb+");
    }
    if(fd == NULL){
        CX_ERROR("open file failed %s - %d",file,errno);
    }
    return fd != NULL;
}

void cxHttp::OnClose()
{
    closeFile();
    cxTcp::OnClose();
    if(!Success()){
        onError.Fire(this);
    }
}

void cxHttp::OnConnected()
{
    if(!initFile()){
        Close(0);
        return;
    }
    cxStr *header = cxStr::Alloc();
    if(method == HTTP_POST){
        header->AppFmt("POST %s HTTP/1.1\r\n",path->Data());
    }else{
        header->AppFmt("GET %s HTTP/1.1\r\n",path->Data());
    }
    if(port == 80){
        header->AppFmt("Host: %s\r\n", host->ToChars());
    }else{
        header->AppFmt("Host: %s:%d\r\n", host->ToChars(),port);
    }
    if(method == HTTP_POST && cxStr::IsOK(post)){
        reqHeads->Set("Content-Length", cxStr::Create()->AppFmt("%d", post->Size()));
    }
    for(cxHash::Iter it=reqHeads->Begin();it!=reqHeads->End();it++){
        header->AppFmt("%s: %s\r\n", it->first.data,it->second->To<cxStr>()->ToChars());
    }
    header->AppFmt("\r\n");
    if(!Write(header)){
        CX_ERROR("http write data error");
    }
    if(method != HTTP_GET && cxStr::IsOK(post)){
        Write(post);
    }
    header->Release();
}

cxBool cxHttp::ConnectURL(cchars url)
{
    struct http_parser_url data={0};
    http_parser_url_init(&data);
    if(http_parser_parse_url(url, strlen(url), 0, &data) != 0){
        return false;
    }
    if(data.field_set & (1 << UF_PORT)){
        port = data.port;
    }else{
        port = 80;
    }
    if(data.field_set & (1 << UF_SCHEMA)){
        cxStr *tmp = cxStr::Create();
        tmp->Init((cxAny)(url+data.field_data[UF_SCHEMA].off), data.field_data[UF_SCHEMA].len);
        cxObject::swap(&schema, tmp);
    }
    if(data.field_set & (1 << UF_HOST)){
        cxStr *tmp = cxStr::Create();
        tmp->Init((cxAny)(url+data.field_data[UF_HOST].off), data.field_data[UF_HOST].len);
        cxObject::swap(&host, tmp);
    }
    if(data.field_set & (1 << UF_PATH)){
        cxStr *tmp = cxStr::Create();
        tmp->Init((cxAny)(url+data.field_data[UF_PATH].off), data.field_data[UF_PATH].len);
        cxObject::swap(&path, tmp);
    }else{
        cxObject::swap(&path, cxStr::Create("/"));
    }
    Connect(host->ToChars(), port);
    return  true;
}

cxHttp *cxHttp::Post(cchars url,const cxStr *post)
{
    CX_ASSERT(cxStr::IsOK(url), "args error");
    cxHttp *rv = cxHttp::Create();
    rv->reqHeads->Set("User-Agent", cxStr::UTF8("Mozilla/4.0(compatible;MSIE6.0;Windows NT 5.0)"));
    rv->reqHeads->Set("Connection", cxStr::UTF8("close"));
    CX_SWAP(rv->post,post);
    rv->method = HTTP_POST;
    if(!rv->ConnectURL(url)){
        CX_ERROR("http url error");
    }
    return rv;
}

cxHttp *cxHttp::WebSocket(cchars url)
{
    CX_ASSERT(cxStr::IsOK(url), "args error");
    cxHttp *rv = cxHttp::Create();
    rv->reqHeads->Set("Upgrade",cxStr::UTF8("websocket"));
    rv->reqHeads->Set("Connection",cxStr::UTF8("Upgrade"));
    rv->reqHeads->Set("Sec-WebSocket-Key",cxStr::UTF8("x3JJHMbDL1EzLkh9GBhXDw=="));
    rv->reqHeads->Set("Sec-WebSocket-Version",cxStr::UTF8("13"));
    rv->reqHeads->Set("Sec-WebSocket-Protocol",cxStr::UTF8("chat, superchat"));
    if(!rv->ConnectURL(url)){
        CX_ERROR("http url error");
    }
    return rv;
}

cxHttp *cxHttp::Get(cchars url)
{
    CX_ASSERT(cxStr::IsOK(url), "args error");
    cxHttp *rv = cxHttp::Create();
    rv->reqHeads->Set("User-Agent", cxStr::UTF8("Mozilla/4.0(compatible;MSIE6.0;Windows NT 5.0)"));
    rv->reqHeads->Set("Connection", cxStr::UTF8("close"));
    if(!rv->ConnectURL(url)){
        CX_ERROR("http url error");
    }
    return rv;
}

cxHttp *cxHttp::LoadFile(cchars url,const cxStr *path,const cxStr *md5)
{
    CX_ASSERT(cxStr::IsOK(url), "url args error");
    CX_ASSERT(cxStr::IsOK(path), "path args error");
    CX_ASSERT(cxStr::IsOK(md5), "md5 args error");
    cxHttp *rv = cxHttp::Create();
    rv->SetFileInfo(path,md5);
    if(!rv->ConnectURL(url)){
        CX_ERROR("http url error");
    }
    return rv;
}

const cxBool cxHttp::Success() const
{
    return success;
}

const cxInt cxHttp::Status() const
{
    return status;
}

const cxStr *cxHttp::Body() const
{
    return body;
}

const cxStr *cxHttp::FileMD5() const
{
    return smd5;
}

const cxStr *cxHttp::FilePath() const
{
    return spath;
}

cxHash *cxHttp::ReqHeads()
{
    return reqHeads;
}

cxHash *cxHttp::ResHeads()
{
    return resHeads;
}

void cxHttp::OnSwitchProto()
{
    
}

void cxHttp::OnCompleted()
{
    if(Success()){
        onSuccess.Fire(this);
    }
}

void cxHttp::OnHeader()
{
    contentLength = parser.content_length;
}

void cxHttp::OnStart()
{
    
}

CX_CPP_END

