//
//  AnimaAttr.cpp
//  godcity
//
//  Created by xuhua on 7/7/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#include <core/cxUtil.h>
#include <core/cxAutoPool.h>
#include <core/cxCSV.h>
#include "cxActionGroup.h"

CX_CPP_BEGIN

CX_IMPLEMENT(cxActionGroup);

cxActionGroup::cxActionGroup()
{
    
}

cxActionGroup::~cxActionGroup()
{
    
}

void cxActionGroup::Load(cxHash *values,cchars file)
{
    const cxStr *data = cxUtil::Content(file);
    CX_ASSERT(cxStr::IsOK(data), "get %s file data error",file);
    cxCSV *csv = cxCSV::Create(data);
    cxStr *name = nullptr;
    cxInt nrow = 0;
    cxActionGroup *attr = nullptr;
    for(cxInt i=3; i < csv->Row(); i++){
        const cxStr *cn = csv->At(i, 0);
        if(cxStr::IsOK(cn)){
            cxObject::swap(&name, cn);
            nrow = i;
            attr = cxActionGroup::Alloc();
            values->Set(name->ToString(), attr);
            attr->Release();
        }
        cchars aname = nullptr;
        cxActionAttr av;
        for(cxInt j = 2;j < csv->Col(1);j++){
            const cxStr *ktype = csv->At(1, j);
            const cxStr *value = csv->At(i, j);
            cxInt c  = i;
            //向后查找
            while(!cxStr::IsOK(value) && c > nrow){
                value = csv->At(--c, j);
            }
            if(!cxStr::IsOK(value)){
                continue;
            }
            if(ktype->IsCaseEqu("Action")){
                aname = value->ToString();//动作名称
            }else if(ktype->IsCaseEqu("Repeat")){
                av.repeat = value->ToInt();//播放次数 =0表示循环播放
            }else if(ktype->IsCaseEqu("Speed")){
                av.speed = value->ToFloat();//播放速度
            }else if(ktype->IsCaseEqu("From")){
                av.from = value->ToInt();//开始帧
            }else if(ktype->IsCaseEqu("To")){
                av.to = value->ToInt();//结束帧
            }else if(ktype->IsCaseEqu("Key")){
                av.key = value->ToInt();//关键帧
            }else if(ktype->IsCaseEqu("Group")){
                av.group = value->ToInt();//如果组>0表示使用固定组中的帧播放动画
            }
        }
        if(av.from < 0 || av.to < 0){
            CX_ERROR("cxActionAttr from or to < 0");
            continue;
        }
        if(!cxStr::IsOK(aname)){
            CX_ERROR("cxActionAttr name miss");
            continue;
        }
        attr->actions.emplace(aname,av);
    }
    cxObject::release(&name);
}

const cxActionAttr *cxActionGroup::Action(cchars key) const
{
    std::map<std::string,cxActionAttr>::const_iterator it = actions.find(key);
    if(it == actions.end()){
        CX_ERROR("%s action miss",key);
        return nullptr;
    }
    return &it->second;
}

CX_CPP_END

