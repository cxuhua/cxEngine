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
    //write file
    static cxBool WriteFile(const cxStr *path,const cxStr *data,cxBool replace);
    static void SetRandSeed();
    //-1 <---> +1 随机数
    static cxFloat Rand();
    //随机数
    static cxInt Rand(cxInt min,cxInt max);
    //当前时间
    static cxDouble Timestamp();
    //获取当前时间戳
    static cxUInt64 Now();
    //获取工具类实例
    static cxUtil *Instance();
    //获取assets数据
    static cxStr *Assets(cchars file);
    //获取文档目录数据
    static cxStr *Document(cchars file);
    //优先从document目录获取数据
    static cxStr *Content(cchars file);
    //本地内容文件大小，优先文档路径
    static cxInt64 ContentSize(cchars file);
    //校验文件md5 >0 成功并返回文件长度
    static cxInt64 ValidFile(cchars file,cchars md5);
    //获取文件大小
    static cxInt64 GetFileSize(cchars file);
    //获取当前app版本号
    virtual cxInt GetVersionCode();
    //获取版本名称
    virtual cxStr *GetVersionName();
    //获取语言代码
    virtual cxStr *GetLang();       //zh en
    //获取国家代码
    virtual cxStr *GetCountry();    //CN TW
    //国家语言代码
    virtual cxStr *LocalizedKey();
    //扩展卡路径
    virtual cxStr *ExtPath(cchars file);
    //获取资源目录
    virtual cxStr *AssetsPath(cchars file);
    //获取资源文件数据
    virtual cxStr *AssetsData(cchars file);
    //获取文档路径
    virtual cxStr *DocumentPath(cchars file);
    //获取资源文件大小
    virtual cxInt64 AssetsSize(cchars file);
    //获取文档文件大小
    virtual cxInt64 DocumentSize(cchars file);
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


