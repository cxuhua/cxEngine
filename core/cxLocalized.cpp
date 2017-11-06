//
//  cxLocalized.cpp
//  cxEngineCore
//
//  Created by xuhua on 7/11/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#include <core/cxUtil.h>
#include <core/cxAutoPool.h>
#include "cxCSV.h"
#include "cxLocalized.h"

CX_CPP_BEGIN

/* 语言文件CVS例子
 TID,EN,CN
 String,String,String
 TID_LOCALIZED_NAME,English,中文
 */

CX_IMPLEMENT(cxLocalized);

cxLocalized *cxLocalized::instance = nullptr;

cxLocalized *cxLocalized::Instance()
{
    return cxCore::One<cxLocalized>(&instance);
}

cxLocalized::cxLocalized()
{
    texts = cxHash::Alloc();
    defaultlng = cxStr::Alloc()->Init("CN");
    lang = nullptr;
}

cxLocalized::~cxLocalized()
{
    cxObject::release(&defaultlng);
    cxObject::release(&lang);
    texts->Release();
}

const cxStr *cxLocalized::GetDefault()
{
    cxLocalized *instance = Instance();
    return instance->defaultlng;
}

void cxLocalized::SetDefault(const cxStr *lng)
{
    cxLocalized *instance = Instance();
    cxObject::swap(&instance->defaultlng, lng);
}

void cxLocalized::SetLang(const cxStr *lng)
{
    cxLocalized *instance = Instance();
    cxObject::swap(&instance->lang, lng);
}

const cxStr *cxLocalized::GetLang()
{
    cxLocalized *instance = Instance();
    if(instance->lang == nullptr){
        cxObject::swap(&instance->lang, cxUtil::Instance()->LocalizedKey());
    }
    return instance->lang;
}

const cxStr *cxLocalized::Format(cchars fmt,...)
{
    char key[1024]={0};
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(key, 1024, fmt, ap);
    va_end(ap);
    return Text(key);
}

const cxStr *cxLocalized::Content(cchars key,...)
{
    const cxStr *fs = Text(key);
    if(!cxStr::IsOK(fs)){
        return nullptr;
    }
    cxStr *ret = cxStr::Create();
    va_list ap;
    va_start(ap, key);
    ret->AppFmt(fs->ToChars(), ap);
    va_end(ap);
    return ret;
}

const cxStr *cxLocalized::Text(cchars key)
{
    cxLocalized *instance = Instance();
    cxObject *tobj = instance->texts->Get(GetLang()->ToChars());
    if(tobj == nullptr){
        tobj = instance->texts->Get(GetDefault()->ToChars());
    }
    if(tobj == nullptr){
        return cxStr::Create()->Init(key);
    }
    cxHash *texts = tobj->To<cxHash>();
    cxObject *pobj = texts->Get(key);
    if(pobj == nullptr){
        return cxStr::Create()->Init(key);
    }
    return pobj->To<cxStr>();
}

void cxLocalized::Load(cchars file)
{
    cxLocalized *local = cxLocalized::Instance();
    local->texts->Clear();
    const cxStr *data = cxUtil::Content(file);
    CX_ASSERT(cxStr::IsOK(data), "get %s file data error",file);
    cxCSV *csv = cxCSV::Create(data);
    for(cxInt j = 1;j < csv->Col();j++){
        const cxStr *text = csv->At(0, j);
        if(!cxStr::IsOK(text)){
            continue;
        }
        cxHash *lngtxt = cxHash::Alloc();
        for(cxInt i = 2; i < csv->Row(); i++){
            const cxStr *key  = csv->At(i, 0);
            if(!cxStr::IsOK(key)){
                continue;
            }
            lngtxt->Set(key->ToChars(), csv->At(i, j));
        }
        local->texts->Set(text->ToChars(), lngtxt);
        lngtxt->Release();
    }
}

CX_CPP_END

