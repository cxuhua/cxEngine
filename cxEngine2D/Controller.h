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

CX_CPP_BEGIN

#define MAX_ITEM    32

#define AT_LEFT(_src_,_dst_)    (_dst_.x == _src_.x-1 && _dst_.y == _src_.y)

#define AT_RIGHT(_src_,_dst_)   (_dst_.x == _src_.x+1 && _dst_.y == _src_.y)

#define AT_TOP(_src_,_dst_)     (_dst_.x == _src_.x && _dst_.y == _src_.y+1)

#define AT_BOTTOM(_src_,_dst_)  (_dst_.x == _src_.x && _dst_.y == _src_.y-1)

enum BoxType{
    BoxTypeNone = 0,
    BoxType3,
    BoxType4,
    BoxType5,
};


class Controller;
class CardItem : public cxSprite
{
public:
    CX_DECLARE(CardItem);
protected:
    explicit CardItem();
    virtual ~CardItem();
private:
    Controller *controller;
    cxUInt type;
    cxPoint2I idx;
public:
    //丢弃
    void Drop();
    //当前所在的位置
    cxPoint2I Index() const;
    void SetIdx(const cxPoint2I &i);
    //两个item是否相等，相等意味着可合成
    virtual cxBool IsEqu(const CardItem *item);
    //移动到新位置
    cxAction *MoveTo(const cxPoint2I &dst);
    //有卡合并过来
    virtual void MergeMe(const cxPoint2IArray &ps);
public:
    static CardItem *Create(Controller *c,const cxPoint2I &idx);
};

class Controller : public cxView
{
public:
    CX_DECLARE(Controller);
protected:
    explicit Controller();
    virtual ~Controller();
private:
    CardItem *items[MAX_ITEM][MAX_ITEM];
    cxSize2F itemSize;
    cxInt col;
    cxInt row;
    cxPoint2I srcIdx;//选中的key位置
    cxPoint2I dstIdx;//目标位置
    cxPoint2IArray points;
protected:
    cxBool OnDispatch(const cxengine::cxTouchable *e);
    void OnEnter();
public:
    //把ps中的view合并到idx位置
    cxInt MergeTo(cxMultiple *m,const cxPoint2IArray &ps,const cxPoint2I &idx);
    //获得类型 3x3 3x4 4x3 4x4 5x5
    BoxType ParseBoxType(const cxBox4I &box);
    //转换为坐标点集合
    cxPoint2IArray ToPoints(const cxBox4I &box,const cxPoint2I &idx);
    //查找空位处上面的卡片
    cxAction *Find(const cxPoint2I &idx);
    //扫描所有格子,返回并发动画
    cxMultiple *ScanSwap();
    //动画完成
    void MultipleExit(cxMultiple *m);
    //计算idx位置处左右上下相等的元素数量，不包括idx
    cxBox4I Compute(const cxPoint2I &idx);
    //丢弃idx位置的view
    CardItem *DropView(const cxPoint2I &idx);
    
    cxMultiple *CheckSwap(const cxPoint2I &src,const cxPoint2I &dst);
    cxMultiple *SwapView(const cxPoint2I &src,const cxPoint2I &dst);
    void Reset();
    //是否可以从src移动到dst
    cxBool IsMoveTo(const cxPoint2I &src,const cxPoint2I &dst);
    cxBool IsValidIdx(const cxPoint2I &idx);
    CardItem *GetView(const cxPoint2I &idx);
    cxBool HasView(const cxPoint2I &idx);
    void SetView(const cxPoint2I &idx,CardItem *pview);
    cxPoint2F ToPos(const cxPoint2I &idx);
    cxPoint2I ToIdx(const cxPoint2F &pos);
    cxPoint2I ToIdx(cxInt key);
public:
    const cxSize2F ItemSize() const;
    void Init();
    static Controller *Create(cxInt c,cxInt r);
public:
    //计算ps坐标内的点是否可以消除
    virtual cxBool HasSwap(const cxPoint2IArray &ps);
    //可交换
    virtual cxBool OnSwap(const cxPoint2I &src,const cxPoint2I &dst);
    //进入战斗
    virtual cxBool OnFight(const cxPoint2I &src);
};

CX_CPP_END

#endif


