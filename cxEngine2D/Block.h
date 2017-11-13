//
//  Controller.h
//  cxEngineTest
//
//  Created by xuhua on 5/16/16.
//  Copyright © 2016 xuhua. All rights reserved.
//

#ifndef cxEngineTest_Block_h
#define cxEngineTest_Block_h

#include <engine/cxMultiple.h>
#include <core/cxHash.h>
#include <engine/cxAtlas.h>
#include <engine/cxSequence.h>
#include "Move.h"
#include "Point.h"

CX_CPP_BEGIN

enum Layer {
    LayerBackgroud,     //背景层
    LayerActive,        //活动层，用来放可移动的块,需要Clip
    LayerStatic,        //静态物
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
    BoxType1x3,
    BoxType1x4,
    BoxType3x3,
    BoxType1x5,
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


class Block : public cxView
{
public:
    CX_DECLARE(Block);
protected:
    explicit Block();
    virtual ~Block();
private:
    Controller *map;
    cxUInt type;
    cxPoint2I idx;
    Move *move;
    CardAttr attr;
    BoxType box;
public:
    //初始化
    virtual void OnInit(Controller *pmap,const cxPoint2I &idx);
    //是否可移动（固定物不能移动)
    virtual cxBool IsEnableMoving();
    //是否允许搜索和计算类型
    virtual cxBool IsEnableCompute();
    //类型改变
    virtual void OnTypeChanged(cxUInt typ);
public:
    //获取特殊类型
    BoxType GetBoxType();
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
    virtual void OnKeep(BoxType bt);
    //当移动动画完成时
    virtual void OnMoveFinished();
public:
    static Block *Create(Controller *pmap,const cxPoint2I &idx);
};
CX_CPP_END

#endif



