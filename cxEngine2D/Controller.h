//
//  Controller.h
//  cxEngineTest
//
//  Created by xuhua on 5/16/16.
//  Copyright © 2016 xuhua. All rights reserved.
//

#ifndef cxEngineTest_Controller_h
#define cxEngineTest_Controller_h

#include <engine/cxMultiple.h>
#include <core/cxHash.h>
#include <engine/cxAtlas.h>
#include <engine/cxSequence.h>
#include "Move.h"
#include "Point.h"
#include "Block.h"

CX_CPP_BEGIN

#define MAX_ITEM    32

class Controller : public cxView
{
public:
    CX_DECLARE(Controller);
protected:
    explicit Controller();
    virtual ~Controller();
private:
    Block *items[MAX_ITEM][MAX_ITEM];
    ItemAttr attrs[MAX_ITEM][MAX_ITEM];
    cxInt YTV[MAX_ITEM];    //记录新创建的方块位置Y递增值
    cxInt YCV[MAX_ITEM];    //计数器偶数优先左
    cxSize2F itemSize;
    cxInt col;
    cxInt row;
    cxPoint2I srcIdx;//选中的key位置
    cxPoint2I dstIdx;//目标位置
    PointArray points;
    cxView *layers[LayerMax];
    cxInt combo;
protected:
    virtual cxBool OnDispatch(const cxengine::cxTouchable *e);
    virtual void OnEnter();
public:
    //加入块
    void AppendBlock(Block *b);
    //获取位置属性
    ItemAttr *GetAttr(const cxPoint2I &idx);
    //把ps中的所有view消除
    cxInt MergeTo(cxMultiple *m,const cxPoint2IArray &ps);
    //获得类型 3x3 3x4 4x3 4x4 5x5
    BoxType ParseBoxType(const cxBox4I &box);
    //转换为坐标点集合
    cxPoint2IArray ToPoints(const cxBox4I &box,const cxPoint2I &idx);
    //搜索落下的view和路径
    Block *SearchUpAndView(PointArray &mps,const cxPoint2I &idx);
    //idx左右搜索
    cxBool EnableSearch(const cxPoint2I &idx);
    //是否有搜索到移动路径
    cxBool HasSearchPath(Block **item,const PointArray &mps);
    //从指定点开始搜索
    Block *SearchPointAndView(PointArray &mps,const cxPoint2I &next,const cxPoint2I &prev);
    //扫描所有格子,返回并发动画
    cxMultiple *ScanSwap();
    void ScanSwapExit(cxMultiple *m);
    //持续扫描路径
    cxMultiple *ScanEmpty();
    void ScanEmptyExit(cxMultiple *m);
    //搜索某个点
    cxBool Search(cxMultiple *m,PointArray &mps,const cxPoint2I &next);
    //计算idx位置处左右上下相等的元素数量，不包括idx
    cxBox4I Compute(const cxPoint2I &idx);
    //丢弃idx位置的view
    Block *DropView(const cxPoint2I &idx);
    //检测是否有移动消除
    cxMultiple *CheckSwap(const cxPoint2I &src,const cxPoint2I &dst);
    //开始交换选中方块
    cxMultiple *SwapView(const cxPoint2I &src,const cxPoint2I &dst);
    void Reset();
    //是否可以从src移动到dst
    cxBool IsMoveTo(const cxPoint2I &src,const cxPoint2I &dst);
    //是否是有效的坐标
    cxBool IsValidIdx(const cxPoint2I &idx);
    //获取坐标上的方块
    Block *GetView(const cxPoint2I &idx);
    //是否有方块
    cxBool HasView(const cxPoint2I &idx);
    //设置方块在坐标点上
    void SetView(const cxPoint2I &idx,Block *pview);
    //坐标点转为显示位置
    cxPoint2F ToPos(const cxPoint2I &idx);
    //反转
    cxPoint2I ToIdx(const cxPoint2F &pos);
public:
    //获取绘制层
    cxView *GetLayer(cxInt layer);
    //获取方块大小
    const cxSize2F ItemSize() const;
    //创建地图场景
    static Controller *Create(cxInt col,cxInt row,const cxSize2F &size);
    //搜索高级方块位置，如果不是当前位置返回true,并返回方块位置
    BoxType FindHighRanking(const cxPoint2IArray &ps,const cxPoint2I &idx,cxPoint2I &out,cxBox4I &box);
    //计算单个位置
    //advance=true将搜索高级块
    cxBool ComputeBox(cxMultiple *m,const cxPoint2I &idx,cxBool advance);
protected:
    //当需要创建一个方块时
    //idx 最终位置
    //pos 显示位置
    virtual Block *OnFactory(const cxPoint2I &idx,const cxPoint2I &pos);
    //如果有指定的特殊移动
    virtual cxBool HasSpecialSwap(Block *src,Block *dst);
    //初始化
    virtual void OnInit(cxInt col,cxInt row,const cxSize2F &size);
    //一次消除结束
    virtual void OnOneFinished();
    //产生一次连击
    virtual void OnOneCombo();
    //计算ps坐标内的点是否可以消除
    virtual cxBool HasSwap(const PointArray &ps);
    //可交换
    virtual cxBool OnSwap(const cxPoint2I &src,const cxPoint2I &dst);
};

CX_CPP_END

#endif


