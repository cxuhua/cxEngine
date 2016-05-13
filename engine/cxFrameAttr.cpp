//
//  cxFrameAttr.cpp
//  godcity
//
//  Created by xuhua on 7/8/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#include <core/cxUtil.h>
#include <core/cxAutoPool.h>
#include <core/cxCSV.h>
#include "cxFrameAttr.h"

CX_CPP_BEGIN

CX_IMPLEMENT(cxFrameAttr);

cxFrameAttr::cxFrameAttr()
{
    
}

cxFrameAttr::~cxFrameAttr()
{
    
}

void cxFrameAttr::Load(cxHash *values,cchars file)
{
    const cxStr *data = cxUtil::Content(file);
    CX_ASSERT(cxStr::IsOK(data), "get %s file data error",file);
    cxCSV *csv = cxCSV::Create(data);
    cxStr *name = nullptr;
    cxInt nrow = 0;
    cxInt level = 1;
    for(cxInt i=3; i < csv->Row(); i++){
        cxAutoPool::Push();
        const cxStr *cn = csv->At(i, 0);
        if(cxStr::IsOK(cn)){
            cxObject::swap(&name, cn);
            nrow = i;
            level = 1;
        }else{
            level ++;
        }
        cxFrameAttr *attr = cxFrameAttr::Create();
        for(cxInt j=2;j<csv->Col(1);j++){
            const cxStr *ktype = csv->At(1, j);
            const cxStr *value = csv->At(i, j);
            cxInt c = i;
            while(!cxStr::IsOK(value) && c > nrow){
                value = csv->At(--c, j);
            }
            if(!cxStr::IsOK(value)){
                continue;
            }
            if(ktype->IsCaseEqu("Texture")){
                attr->SetTexture(value->ToString());
                continue;
            }
            if(ktype->IsCaseEqu("Size")){
                attr->SetSize(value->ToSize2F());
                continue;
            }
            if(ktype->IsCaseEqu("Scale")){
                attr->SetScale(value->ToSize2F());
                continue;
            }
            if(ktype->IsCaseEqu("Offset")){
                attr->SetOffset(value->ToInt());
                continue;
            }
            if(ktype->IsCaseEqu("Group")){
                attr->SetGroup(value->ToInt());
                continue;
            }
            if(ktype->IsCaseEqu("Count")){
                attr->SetCount(value->ToInt());
                continue;
            }
            if(ktype->IsCaseEqu("Layer")){
                attr->SetLayer(value->ToInt());
                continue;
            }
            if(ktype->IsCaseEqu("Map")){
                attr->SetMaps(value);
                continue;
            }
            if(ktype->IsCaseEqu("Repeats")){
                attr->SetRepeats(value);
                continue;
            }
        }
        if(attr->Init()){
            values->Set(cxHashKey::Format("%s_%d",name->ToString(),level), attr);
        }
        cxAutoPool::Pop();
    }
    cxObject::release(&name);
}

CX_CPP_END

