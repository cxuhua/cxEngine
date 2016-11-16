//
//  cxHttp.cpp
//  cxEngineCore
//
//  Created by xuhua on 7/3/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//


#include "cxHttp.h"

CX_CPP_BEGIN

CX_IMPLEMENT(cxHttp);


int cxHttp::onBodyFunc(http_parser *parser, const char *at, size_t length)
{
    cxHttp *http = static_cast<cxHttp *>(parser->data);
    http->body->Append(at, (cxInt)length);
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
    http->success = (http->status == 200);
    http->OnCompleted();
    return 0;
}

void cxHttp::OnData(char *buffer,cxInt size)
{
    data->Append(buffer, size);
    chars buf = data->Buffer() + offset;
    cxInt siz = data->Size() - offset;
    offset = (cxInt)http_parser_execute(&parser, &settings, buf, (size_t)siz);
}

cxHttp::cxHttp()
{
    success = false;
    parser.data = this;
    http_parser_init(&parser, HTTP_RESPONSE);
    settings.on_body = onBodyFunc;
    settings.on_header_field = onHeadField;
    settings.on_header_value = onHeadValue;
    settings.on_message_begin = messageBegin;
    settings.on_headers_complete = headCompleted;
    settings.on_message_complete = messageCompleted;
    
    method = HTTP_GET;
    
    path = nullptr;
    post = nullptr;
    host = nullptr;
    
    body = cxStr::Alloc();
    reqHeads = cxHash::Alloc();
    resHeads = cxHash::Alloc();
    data = cxStr::Alloc();
    field = cxStr::Alloc();

    reqHeads->Set("User-Agent", cxStr::UTF8("Mozilla/4.0(compatible;MSIE6.0;Windows NT 5.0)"));
    reqHeads->Set("Connection", cxStr::UTF8("close"));
}

cxHttp::~cxHttp()
{
    cxObject::release(&host);
    cxObject::release(&path);
    cxObject::release(&post);
    field->Release();
    body->Release();
    reqHeads->Release();
    resHeads->Release();
    data->Release();
}

void cxHttp::OnClose()
{
    cxTcp::OnClose();
    if(!Success()){
        onError.Fire(this);
    }
}

void cxHttp::OnConnected()
{
    offset = 0;
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
        header->AppFmt("%s: %s\r\n", it->first.data,it->second->To<cxStr>()->Data());
    }
    header->AppFmt("\r\n");
    if(!Write(header)){
        CX_ERROR("http write data error");
    }
    if(method == HTTP_POST && cxStr::IsOK(post)){
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
    Connect(host->ToString(), port);
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
    rv->Forever();
    return rv;
}

cxHttp *cxHttp::Get(cchars url)
{
    CX_ASSERT(cxStr::IsOK(url), "args error");
    cxHttp *rv = cxHttp::Create();
    if(!rv->ConnectURL(url)){
        CX_ERROR("http url error");
    }
    rv->Forever();
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
    
}

void cxHttp::OnStart()
{
    
}

CX_CPP_END

