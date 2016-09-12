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
    cxStr *s = cxStr::Alloc();
    if(method == HTTP_GET){
        s->AppFmt("GET %s HTTP/1.1\r\n",path->Data());
    }else if(method == HTTP_POST){
        s->AppFmt("POST %s HTTP/1.1\r\n",path->Data());
    }
    if(method == HTTP_POST && cxStr::IsOK(post)){
        reqHeads->Set("Content-Length", cxStr::Create()->AppFmt("%d", post->Size()));
    }
    for(cxHash::Iter it=reqHeads->Begin();it!=reqHeads->End();it++){
        s->AppFmt("%s: %s\r\n", it->first.data,it->second->To<cxStr>()->Data());
    }
    s->AppFmt("\r\n");
    if(!Write(s)){
        CX_ERROR("http write data error");
    }
    if(method == HTTP_POST && cxStr::IsOK(post)){
        Write(post);
    }
    s->Release();
}

void cxHttp::ConnectURL(cchars url)
{
    char sserver[256]={0};
    char spath[1024]={0};
    int serverlen = 0;
    int sport;
    int numread = 0;
    url = url+7;
    sscanf(url, "%255[^/:]", sserver);
    serverlen = (int)strlen(sserver);
    if(url[serverlen] == ':'){
        sscanf(&url[serverlen+1], "%d%n", &sport, &numread);
        numread++;
    }else{
        sport = 80;
    }
    strcpy(spath, &url[serverlen+numread]);
    cxObject::swap(&path, cxStr::Create()->AppFmt("%s",strlen(spath)>0?spath:"/"));
    cxObject::swap(&host, cxStr::Create()->Init(sserver));
    port = sport;
    Connect((cchars)sserver, port);
}

cxHttp *cxHttp::Post(cchars url,const cxStr *post)
{
    CX_ASSERT(cxStr::IsOK(url), "args error");
    cxHttp *rv = cxHttp::Create();
    CX_SWAP(rv->post,post);
    rv->method = HTTP_POST;
    rv->ConnectURL(url);
    rv->Forever();
    return rv;
}

cxHttp *cxHttp::Get(cchars url)
{
    CX_ASSERT(cxStr::IsOK(url), "args error");
    cxHttp *rv = cxHttp::Create();
    rv->ConnectURL(url);
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

