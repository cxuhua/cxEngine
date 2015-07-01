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
    cxBool FileExists(const cxStr *path,cxInt *length) const;
private:
    static cxUtil *instance;
    cxBool WriteFileData(const cxStr *path,const cxStr *data,cxBool replace);
    const cxStr *ReadFileData(const cxStr *path);
public:
    //-1 <---> +1
    static cxFloat Rand();
    //
    static cxInt Rand(cxInt min,cxInt max);
    //当前时间
    static cxDouble Timestamp();
    //获取工具类实例
    static cxUtil *Instance();
    //获取assets数据
    static const cxStr *Assets(cchars file);
    static const cxStr *Document(cchars file);
    static const cxStr *Content(cchars file);//优先从document目录获取数据
    //获取资源目录
    virtual const cxStr *AssetsPath(const cxStr *file);
    //获取资源文件数据
    virtual const cxStr *AssetsData(const cxStr *file);
    //获取文档路径
    virtual const cxStr *DocumentPath(const cxStr *file);
    //向文档目录写文件
    virtual cxBool WriteDocument(const cxStr *file,const cxStr *data,cxBool replace);
    //获取目录文件数据
    virtual const cxStr *DocumentData(const cxStr *file);
    //删除文档
    virtual cxBool DeleteDocument(const cxStr *file);
    //打印日志
    virtual void Logger(const char* type,const char*file,int line,const char* format,va_list ap);
    //内容缩放
    virtual const cxFloat ScaleFactor() const;
};

CX_CPP_END

#endif


