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

CX_IMPLEMENT(cxLocalized);

cxLocalized *cxLocalized::instance = nullptr;

cxLocalized *cxLocalized::Instance()
{
    return cxCore::One<cxLocalized>(&instance);
}

cxLocalized::cxLocalized()
{
    texts = cxHash::Alloc();
    lang = nullptr;
}

cxLocalized::~cxLocalized()
{
    cxObject::release(&lang);
    texts->Release();
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

const cxStr *cxLocalized::Text(cchars key)
{
    cxLocalized *instance = Instance();
    cxObject *tobj = instance->texts->Get(GetLang()->Data());
    if(tobj == nullptr){
        return cxStr::UTF8(key);
    }
    cxHash *texts = tobj->To<cxHash>();
    cxObject *pobj = texts->Get(key);
    if(pobj == nullptr){
        return cxStr::UTF8(key);
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
            lngtxt->Set(key->Data(), csv->At(i, j));
        }
        local->texts->Set(text->Data(), lngtxt);
        lngtxt->Release();
    }
}

CX_CPP_END

