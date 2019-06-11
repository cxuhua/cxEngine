//
// Created by xuhua on 2019/6/11.
//
#include <engine/cxEngine.h>
#include "CGmSktUse.h"

CX_CPP_BEGIN

CGmSktUse::CGmSktUse()
{

}

CGmSktUse::~CGmSktUse()
{

}
//显示连接结果
int CGmSktUse::OnGmConnect(int nResult)
{
    if(nResult == TCP_CONNECT_STARTREAD_SUCCESS){
        cxEngine::Instance()->PushEvent(1000);
    }
    return 0;
}
//显示关闭提示
int CGmSktUse::OnGmClose(int nResult)
{
    return 0;
}
//显示接收到的文本
int CGmSktUse::OnGmStream(int nDataType, const char *pData, int nLen)
{
    cxEngine::Instance()->PushEvent(1001, pData, nLen);
    return 0;
}

CX_CPP_END