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

void cxHttp::OnBody(cchars data,cxInt len)
{
    if(fd != NULL){
        writeFile(data, len);
    }else{
        body->Append(data, len);
    }
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
    http->success = (http->status >= 200 && http->status < 300);
    http->closeFile();
    http->OnCompleted();
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
    while(off < len){
        off += http_parser_execute(&parser, &settings, buffer + off, len - off);
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
    
    method = HTTP_GET;
    spath = nullptr;
    path = nullptr;
    post = nullptr;
    host = nullptr;
    fd = NULL;
    smd5 = nullptr;
    contentLength = 0;
    
    body = cxStr::Alloc();
    reqHeads = cxHash::Alloc();
    resHeads = cxHash::Alloc();
    field = cxStr::Alloc();

    reqHeads->Set("User-Agent", cxStr::UTF8("Mozilla/4.0(compatible;MSIE6.0;Windows NT 5.0)"));
    reqHeads->Set("Connection", cxStr::UTF8("close"));
}

cxHttp::~cxHttp()
{
    closeFile();
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
    if(size <= 0){
        return;
    }
    rename(file, spath->ToChars());
    OnFile(spath,size);
}

void cxHttp::writeFile(cchars data,cxInt size)
{
    fwrite(data, size, 1, fd);
}

cxBool cxHttp::initFile()
{
    //检测原文件是否成功
    cxInt64 fsiz = cxUtil::ValidFile(spath->ToChars(), smd5->ToChars());
    if(fsiz > 0){
        OnFile(spath,fsiz);
        return false;
    }
    //检测临时文件是否成功
    char file[PATH_MAX]={0};
    snprintf(file, PATH_MAX, "%s.tmp",spath->ToChars());
    fsiz = cxUtil::Instance()->GetFileSize(file);
    if(fsiz >= 0 && cxUtil::ValidFile(file, smd5->ToChars())){
        rename(file, spath->ToChars());
        OnFile(spath,fsiz);
        return false;
    }
    //继续下载
    if(fsiz >= 0){
        fd = fopen(file, "ab+");
        reqHeads->Set("Range", cxStr::UTF8("bytes=%lld-",fsiz));
    }else{
        fd = fopen(file, "wb+");
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
    reqHeads->Set("Host", host);
    cxStr *header = cxStr::Alloc();
    if(method == HTTP_GET){
        header->AppFmt("GET %s HTTP/1.1\r\n",path->Data());
    }else if(method == HTTP_POST){
        header->AppFmt("POST %s HTTP/1.1\r\n",path->Data());
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
    CX_SWAP(rv->post,post);
    rv->method = HTTP_POST;
    if(!rv->ConnectURL(url)){
        CX_ERROR("http url error");
    }
    return rv;
}

cxHttp *cxHttp::Get(cchars url)
{
    CX_ASSERT(cxStr::IsOK(url), "args error");
    cxHttp *rv = cxHttp::Create();
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
    if(!rv->initFile()){
        return rv;
    }
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

const cxJson *cxHttp::Json() const
{
    return cxJson::Create()->From(body);
}

cxHash *cxHttp::ReqHeads()
{
    return reqHeads;
}

cxHash *cxHttp::ResHeads()
{
    return resHeads;
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

