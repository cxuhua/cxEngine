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
    if(instance == nullptr){
        instance = cxLocalized::Alloc();
    }
    return instance;
}

cxLocalized::cxLocalized()
{
    texts = cxHash::Alloc();
}

cxLocalized::~cxLocalized()
{
    texts->Release();
}

const cxStr *cxLocalized::Text(cchars key)
{
    cxLocalized *local = cxLocalized::Instance();
    cxObject *pobj = local->texts->Get(key);
    if(pobj == nullptr){
        CX_WARN("%s key localize text miss",key);
        return cxStr::UTF8(key);
    }
    return pobj->To<cxStr>();
}

void cxLocalized::Load(cchars file)
{
    const cxStr *keycode = cxUtil::Instance()->LocalizedKey();
    cxLocalized *local = cxLocalized::Instance();
    local->texts->Clear();
    const cxStr *data = cxUtil::Content(file);
    CX_ASSERT(cxStr::IsOK(data), "get %s file data error",file);
    cxCSV *csv = cxCSV::Create(data);
    cxInt col = 1;
    for(cxInt j=1;j<csv->Col();j++){
        const cxStr *text = csv->At(0, j);
        if(text->IsCaseEqu(keycode->Data())){
            col = j;
            break;
        }
    }
    for(cxInt i=2; i < csv->Row(); i++){
        const cxStr *key  = csv->At(i, 0);
        if(!cxStr::IsOK(key)){
            continue;
        }
        local->texts->Set(key->Data(), csv->At(i, col));
    }
}

CX_CPP_END

