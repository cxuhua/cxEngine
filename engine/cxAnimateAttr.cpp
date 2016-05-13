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
#include "cxAnimateAttr.h"

CX_CPP_BEGIN

CX_IMPLEMENT(cxAnimateAttr);

cxAnimateAttr::cxAnimateAttr()
{
    
}

cxAnimateAttr::~cxAnimateAttr()
{
    
}

void cxAnimateAttr::Load(cxHash *values,cchars file)
{
    const cxStr *data = cxUtil::Content(file);
    CX_ASSERT(cxStr::IsOK(data), "get %s file data error",file);
    cxCSV *csv = cxCSV::Create(data);
    cxStr *name = nullptr;
    cxInt nrow = 0;
    cxAnimateAttr *attr = nullptr;
    for(cxInt i=3; i < csv->Row(); i++){
        cxAutoPool::Push();
        const cxStr *cn = csv->At(i, 0);
        if(cxStr::IsOK(cn)){
            cxObject::swap(&name, cn);
            nrow = i;
            attr = cxAnimateAttr::Alloc();
            values->Set(name->ToString(), attr);
            attr->Release();
        }
        cchars aname = nullptr;
        cxActionAttr av;
        for(cxInt j = 2;j < csv->Col(1);j++){
            const cxStr *ktype = csv->At(1, j);
            const cxStr *value = csv->At(i, j);
            cxInt c  = i;
            while(!cxStr::IsOK(value) && c > nrow){
                value = csv->At(--c, j);
            }
            if(!cxStr::IsOK(value)){
                continue;
            }
            if(ktype->IsCaseEqu("Action")){
                aname = value->ToString();//动作名称
            }else if(ktype->IsCaseEqu("Repeat")){
                //播放次数 =0表示循环播放
                av.repeat = value->ToInt();
            }else if(ktype->IsCaseEqu("Speed")){
                //播放速度
                av.speed = value->ToFloat();
            }else if(ktype->IsCaseEqu("From")){
                //开始帧
                av.from = value->ToInt();
            }else if(ktype->IsCaseEqu("To")){
                //结束帧
                av.to = value->ToInt();
            }else if(ktype->IsCaseEqu("Key")){
                //关键帧
                av.key = value->ToInt();
            }else if(ktype->IsCaseEqu("Group")){
                //如果组>0表示使用固定组中的帧播放动画
                av.group = value->ToInt();
            }
        }
        if(av.key < av.from || av.key > av.to){
            av.key = av.to;
        }
        if(cxStr::IsOK(aname)){
            attr->actions.emplace(aname,av);
        }
        cxAutoPool::Pop();
    }
    cxObject::release(&name);
}

const cxActionAttr *cxAnimateAttr::Action(cchars key) const
{
    std::map<std::string,cxActionAttr>::const_iterator it = actions.find(key);
    if(it != actions.end()){
        return &it->second;
    }
    return nullptr;
}

CX_CPP_END

