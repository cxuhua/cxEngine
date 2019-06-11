//
// Created by xuhua on 2019/6/11.
//

#ifndef MSGAME_CGMSKTUSE_H
#define MSGAME_CGMSKTUSE_H

#include "gmsktclient.h"

CX_CPP_BEGIN

class CGmSktUse : public CGmSktClient{
public:
    explicit CGmSktUse();
    virtual ~CGmSktUse();
    //显示连接结果
    virtual int OnGmConnect(int nResult);
    //显示关闭提示
    virtual int OnGmClose(int nResult);
    //显示接收到的文本
    virtual int OnGmStream(int nDataType, const char *pData, int nLen);
};

CX_CPP_END

#endif //MSGAME_CGMSKTUSE_H
