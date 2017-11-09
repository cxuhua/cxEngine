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

CX_CPP_BEGIN

#define MAX_ITEM    32

enum Layer {
    LayerBackgroud,     //背景层
    LayerActive,        //活动层，用来放可移动的块,需要Clip
    LayerAnimate,       //顶部动画层
    LayerMax,           //最大层数量
};

#define AT_LEFT(_src_,_dst_)    (_dst_.x == _src_.x-1 && _dst_.y == _src_.y)

#define AT_RIGHT(_src_,_dst_)   (_dst_.x == _src_.x+1 && _dst_.y == _src_.y)

#define AT_TOP(_src_,_dst_)     (_dst_.x == _src_.x && _dst_.y == _src_.y+1)

#define AT_BOTTOM(_src_,_dst_)  (_dst_.x == _src_.x && _dst_.y == _src_.y-1)

class Controller;
class Block;

//方块类型
enum BoxType{
    BoxTypeNone = 0,
    BoxType3,
    BoxType4,
    BoxType5,
};

//位置属性
struct ItemAttr {
    cxPoint2I Src;  //来源坐标
    cxPoint2I SrcP; //跳跃出现点
    
    cxPoint2I Dst;  //目标坐标
    cxPoint2I DstP; //跳跃消失点
    
    cxBool Factory; //创建点
    cxBool Static;  //静态位置
    
    Block *Item;
    ItemAttr()
    {
        Item = nullptr;
        Factory = false;
        Static = false;
        Src = cxPoint2I(-1, -1);
        SrcP = cxPoint2I(-1, -1);
        Dst = cxPoint2I(-1, -1);
        DstP = cxPoint2I(-1, -1);
    }
    //是否是空位置
    cxBool IsEmpty(Controller *map);
    //是否可以进行左右搜索
    cxBool IsSearchLR(Controller *map);
    //是否是一个通道
    cxBool IsPipe(Controller *map);
    //是否搜索此点
    cxBool IsSearch(Controller *map);
    //是否可活动块
    cxBool IsActiveItem(Controller *map);
    //是否是创建点
    cxBool IsFactory(Controller *map);
};

//方块属性
struct CardAttr {
    CardAttr()
    {
        
    }
};


class Block : public cxSprite
{
public:
    CX_DECLARE(Block);
protected:
    explicit Block();
    virtual ~Block();
private:
    Controller *controller;
    cxUInt type;
    cxPoint2I idx;
    Move *move;
    CardAttr attr;
public:
    //是否可移动（固定物不能移动)
    virtual cxBool IsEnableMoving();
public:
    //获取所属的层
    cxInt GetLayer();
    //设置类型
    void SetType(cxUInt typ);
    //丢弃
    void Drop();
    //当前所在的位置
    cxPoint2I Index() const;
    void SetIdx(const cxPoint2I &i);
    //两个item是否相等，相等意味着可合成
    virtual cxBool IsEqu(const Block *item);
    //移动到新位置
    void StartMove(cxMultiple *m,const PointArray &ps);
    //当移除块时
    virtual void OnDrop(cxMultiple *m);
    //当移动块发生消除时
    virtual cxBool OnCompute(BoxType bt,const cxPoint2IArray &ps);
    //块保留改变
    virtual void OnKeepUp(BoxType bt);
    //当移动动画完成时
    virtual void OnMoveFinished();
public:
    static Block *Create(Controller *c,const cxPoint2I &idx);
};

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
protected:
    cxBool OnDispatch(const cxengine::cxTouchable *e);
    void OnEnter();
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
    //动画完成
    
    //计算idx位置处左右上下相等的元素数量，不包括idx
    cxBox4I Compute(const cxPoint2I &idx);
    //丢弃idx位置的view
    Block *DropView(const cxPoint2I &idx);
    
    cxMultiple *CheckSwap(const cxPoint2I &src,const cxPoint2I &dst);
    cxMultiple *SwapView(const cxPoint2I &src,const cxPoint2I &dst);
    void Reset();
    //是否可以从src移动到dst
    cxBool IsMoveTo(const cxPoint2I &src,const cxPoint2I &dst);
    cxBool IsValidIdx(const cxPoint2I &idx);
    Block *GetView(const cxPoint2I &idx);
    cxBool HasView(const cxPoint2I &idx);
    void SetView(const cxPoint2I &idx,Block *pview);
    cxPoint2F ToPos(const cxPoint2I &idx);
    cxPoint2I ToIdx(const cxPoint2F &pos);
    cxPoint2I ToIdx(cxInt key);
public:
    cxView *GetLayer(cxInt layer);
    const cxSize2F ItemSize() const;
    virtual void Init();
    static Controller *Create(cxInt col,cxInt row,const cxSize2F &size);
public:
    //搜索高级方块位置，如果不是当前位置返回true,并返回方块位置
    BoxType FindHighRanking(const cxPoint2IArray &ps,const cxPoint2I &idx,cxPoint2I &out,cxBox4I &box);
    //计算单个位置
    //advance=true将搜索高级块
    cxBool ComputeItem(cxMultiple *m,const cxPoint2I &idx,cxBool advance);
    //计算ps坐标内的点是否可以消除
    virtual cxBool HasSwap(const PointArray &ps);
    //可交换
    virtual cxBool OnSwap(const cxPoint2I &src,const cxPoint2I &dst);
};

CX_CPP_END

#endif


