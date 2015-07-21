//
//  cxAstar.cpp
//  cxEngineCore
//
//  Created by xuhua on 7/4/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#include "cxAstar.h"

CX_CPP_BEGIN

SearchNode::SearchNode()
{
    star = nullptr;
}

SearchNode::SearchNode(cxAstar *s,const cxPoint2I &p)
{
    star = s;
    point = p;
}

float SearchNode::GoalDistanceEstimate(SearchNode &nodeGoal)
{
    return fabsf((cxFloat)point.x - (cxFloat)nodeGoal.point.x) + fabsf((cxFloat)point.y - (cxFloat)nodeGoal.point.y);
}

bool SearchNode::IsGoal(SearchNode &nodeGoal)
{
    return point == nodeGoal.Point();
}

bool SearchNode::GetSuccessors(AStarSearch<SearchNode> *astarsearch, SearchNode *parent_node)
{
    cxPoint2I parent = parent_node!=nullptr?parent_node->point:cxPoint2I(-1,-1);
    return star->GetSuccessors(point, parent);
}

float SearchNode::GetCost(SearchNode &successor)
{
    return star->GetCost(successor.Point());
}

bool SearchNode::IsSameState(SearchNode &rhs)
{
    return point == rhs.point;
}

const cxPoint2I &SearchNode::Point() const
{
    return point;
}

CX_IMPLEMENT(cxAstar);

cxAstar::cxAstar()
{
    object = nullptr;
    delegate = nullptr;
}

cxAstar::~cxAstar()
{
    astar.EnsureMemoryFreed();
}

void cxAstar::SetDelegate(cxAstarDelegate *gate)
{
    delegate = gate;
}

void cxAstar::AddSuccessNode(const cxPoint2I &p)
{
    SearchNode node(this,p);
    astar.AddSuccessor(node);
}

void cxAstar::OnSuccess()
{
    isSuccess = true;
    onSuccess.Fire(this);
}

void cxAstar::OnFailed()
{
    isSuccess = false;
    onFailed.Fire(this);
}

const cxBool cxAstar::IsSuccess() const
{
    return isSuccess;
}

void cxAstar::OnSearching()
{
    onSearching.Fire(this);
}

const cxPoint2IArray &cxAstar::Points() const
{
    return points;
}

void cxAstar::Cancel()
{
    astar.CancelSearch();
}

cxBool cxAstar::Step(cxInt iter)
{
    cxBool rv = true;
    while(!isSuccess && iter > 0){
        cxUInt state = astar.SearchStep();
        if(state == AStarSearch<SearchNode>::SEARCH_STATE_SEARCHING){
            OnSearching();
        }else if(state == AStarSearch<SearchNode>::SEARCH_STATE_SUCCEEDED){
            points.Clear();
            SearchNode *node = astar.GetSolutionStart();
            while(node != nullptr){
                points.Append(node->Point());
                node = astar.GetSolutionNext();
            }
            astar.FreeSolutionNodes();
            rv = true;
            OnSuccess();
            break;
        }else if(state == AStarSearch<SearchNode>::SEARCH_STATE_FAILED){
            points.Clear();
            rv = false;
            OnFailed();
            break;
        }else{
            CX_ERROR("a star find error %d",state);
            break;
        }
        iter --;
    }
    return rv;
}

cxFloat cxAstar::GetCost(const cxPoint2I &p)
{
    return 1.0f;
}

void cxAstar::Init(const cxPoint2I &from,const cxPoint2I &to)
{
    isSuccess = false;
    SearchNode start(this,from);
    SearchNode end(this,to);
    points.Clear();
    astar.SetStartAndGoalStates(start, end);
}

cxPoint2I cxAstar::GetStart()
{
    SearchNode *node = astar.GetSolutionStart();
    return node->Point();
}

cxPoint2I cxAstar::GetEnd()
{
    SearchNode *node = astar.GetSolutionEnd();
    return node->Point();
}

cxBool cxAstar::GetSuccessors(const cxPoint2I &point,const cxPoint2I &parent)
{
    cxPoint2I lv = point.LV();
    if(IsAppend(this,lv) && lv != parent){
        AddSuccessNode(lv);
    }
    cxPoint2I rv = point.RV();
    if(IsAppend(this,rv) && rv != parent){
        AddSuccessNode(rv);
    }
    cxPoint2I tv = point.TV();
    if(IsAppend(this,tv) && tv != parent){
        AddSuccessNode(tv);
    }
    cxPoint2I bv = point.BV();
    if(IsAppend(this,bv) && bv != parent){
        AddSuccessNode(bv);
    }
    cxPoint2I lt = point.LT();
    if(IsAppend(this,lt) && lt != parent){
        AddSuccessNode(lt);
    }
    cxPoint2I lb = point.LB();
    if(IsAppend(this,lb) && lb != parent){
        AddSuccessNode(lb);
    }
    cxPoint2I rt = point.RT();
    if(IsAppend(this,rt) && rt != parent){
        AddSuccessNode(rt);
    }
    cxPoint2I rb = point.RB();
    if(IsAppend(this,rb) && rb != parent){
        AddSuccessNode(rb);
    }
    return true;
}

void cxAstar::SetObject(cxObject *pobj)
{
    object = pobj;
}

cxObject *cxAstar::GetObject()
{
    return object;
}

cxBool cxAstar::IsAppend(cxAstar *star,const cxPoint2I &point)
{
    if(delegate != nullptr){
        return delegate->IsAppend(star,point);
    }
    return false;
}

CX_CPP_END

