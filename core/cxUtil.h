//
//  cxUtil.h
//  cxEngineCore
//
//  Created by xuhua on 5/30/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#ifndef cxEngineCore_cxUtil_h
#define cxEngineCore_cxUtil_h

#include "cxStr.h"
#include "cxObject.h"

CX_CPP_BEGIN

class cxUtil : public cxObject
{
    friend cxCore;
private:
    CX_DECLARE(cxUtil);
protected:
    explicit cxUtil();
    virtual ~cxUtil();
    cxBool FileExists(const cxStr *path,cxInt *length);
private:
    static cxUtil *instance;
    cxBool WriteFileData(const cxStr *path,const cxStr *data,cxBool replace);
    cxStr *ReadFileData(const cxStr *path);
public:
    static void SetRandSeed();
    //-1 <---> +1 随机数
    static cxFloat Rand();
    //随机数
    static cxInt Rand(cxInt min,cxInt max);
    //当前时间
    static cxDouble Timestamp();
    static cxUInt64 Now();
    //获取工具类实例
    static cxUtil *Instance();
    //获取assets数据
    static cxStr *Assets(cchars file);
    static cxStr *Document(cchars file);
    static cxStr *Content(cchars file);//优先从document目录获取数据
    //获取语言代码
    virtual cxStr *GetLang();       //zh en
    //获取国家代码
    virtual cxStr *GetCountry();    //CN TW
    //国家语言代码
    virtual cxStr *LocalizedKey();
    //获取资源目录
    virtual cxStr *AssetsPath(cchars file);
    //获取资源文件数据
    virtual cxStr *AssetsData(cchars file);
    //获取文档路径
    virtual cxStr *DocumentPath(cchars file);
    //向文档目录写文件
    virtual cxBool WriteDocument(cchars file,const cxStr *data,cxBool replace);
    //获取目录文件数据
    virtual cxStr *DocumentData(cchars file);
    //删除文档
    virtual cxBool DeleteDocument(cchars file);
    //打印日志
    virtual void Logger(const char* type,const char*file,int line,const char* format,va_list ap);
    //内容缩放
    virtual cxFloat ScaleFactor();
    //UUID
    virtual cxStr *UUID();
};

CX_CPP_END

#endif


