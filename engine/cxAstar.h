//
//  cxAstar.h
//  cxEngineCore
//
//  Created by xuhua on 7/4/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#ifndef cxEngineCore_cxAstar_h
#define cxEngineCore_cxAstar_h

#include <ext/stlastar.h>
#include <core/cxObject.h>
#include <math/cxPoint2I.h>

CX_CPP_BEGIN

class cxAstar;
class SearchNode
{
private:
    cxPoint2I point;
    cxAstar *star;
public:
    const cxPoint2I &Point() const;
    SearchNode();
    SearchNode(cxAstar *s,const cxPoint2I &p);
    float GoalDistanceEstimate(SearchNode &nodeGoal);
    bool IsGoal(SearchNode &nodeGoal);
    bool GetSuccessors(AStarSearch<SearchNode> *astarsearch, SearchNode *parent_node);
    float GetCost(SearchNode &successor);
    bool IsSameState(SearchNode &rhs);
};

class cxAstar : public cxObject
{
public:
    CX_DECLARE(cxAstar);
protected:
    explicit cxAstar();
    virtual ~cxAstar();
private:
    cxPoint2I from;
    cxPoint2I to;
    cxPoint2IArray points;
    AStarSearch<SearchNode> astar;
    void AddSuccessNode(const cxPoint2I &p);
protected:
    virtual cxBool IsAppend(const cxPoint2I &point);
    virtual void OnSuccess();
    virtual void OnFailed();
    virtual void OnSearching();
public:
    void Step(cxInt iter=INT_MAX);
    virtual cxBool GetSuccessors(const cxPoint2I &point,const cxPoint2I &parent);
    virtual cxBool IsGoal(const cxPoint2I &c,const cxPoint2I &p);
    virtual cxFloat GetCost(const cxPoint2I &p);
    void Init(const cxPoint2I &afrom,const cxPoint2I &ato);
    const cxPoint2IArray &Points() const;
};

CX_CPP_END

#endif


