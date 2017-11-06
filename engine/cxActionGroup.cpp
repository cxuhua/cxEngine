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

/* CVS 格式例子
 ID,名字ID,动作,播放次数,播放速度,组编码,开始帧,结束帧,关键帧
 Name,TID,Action,Repeat,Speed,Group,From,To,Key
 String,String,String,Int,Float,Int,Int,Int,Int
 Mage,精灵,move,0,1,0,0,7,
 ,,attack,,1,,8,15,"9,13"
*/

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
            values->Set(name->ToChars(), attr);
            attr->Release();
        }
        cchars aname = nullptr;
        cxActionAttr av;
        for(cxInt j = 2;j < csv->Col(1);j++){
            const cxStr *ktype = csv->At(1, j);
            const cxStr *value = csv->At(i, j);
            cxInt c  = i;
            //向上查找
            while(!cxStr::IsOK(value) && c > nrow){
                value = csv->At(--c, j);
            }
            if(!cxStr::IsOK(value)){
                continue;
            }
            if(ktype->IsCaseEqu("Action")){
                aname = value->ToChars();//动作名称
            }else if(ktype->IsCaseEqu("Repeat")){
                av.SetRepeat(value);//播放次数 =0表示循环播放
            }else if(ktype->IsCaseEqu("Speed")){
                av.SetSpeed(value);//播放速度
            }else if(ktype->IsCaseEqu("From")){
                av.SetFrom(value);//开始帧
            }else if(ktype->IsCaseEqu("To")){
                av.SetTo(value);//结束帧
            }else if(ktype->IsCaseEqu("Key")){
                av.SetKey(value);//设置关键帧
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

