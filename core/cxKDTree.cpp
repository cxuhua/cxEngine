//
//  cxKDTree.cpp
//  cxEngineCore
//
//  Created by xuhua on 7/18/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//


#include "cxKDTree.h"

CX_CPP_BEGIN

CX_IMPLEMENT(cxKDTree);

cxKDTree::cxKDTree() : tree(nodes)
{
    
}

cxKDTree::~cxKDTree()
{
    
}

const cxKDTree::Results &cxKDTree::GetResults()
{
    return results;
}

const KDTreeNode &cxKDTree::GetTreeNode(cxKDTree::Results::const_iterator it)
{
    return nodes[it->first];
}

cxInt cxKDTree::Nearst(const cxPoint2F &cp,cxFloat max)
{
    results.clear();
    if(nodes.empty()){
        return 0;
    }
    return (cxInt)tree.findPointsWithinRadius(KDTreeNode(cp, 0), max, results, cxEqualFloat, true);
}

cxKDTree *cxKDTree::Build()
{
    tree.buildIndex();
    return this;
}

cxKDTree *cxKDTree::Clear()
{
    nodes.clear();
    return this;
}

cxKDTree *cxKDTree::Append(const cxPoint2F &p,void *ref)
{
    nodes.push_back(KDTreeNode(p, ref));
    return this;
}

CX_CPP_END

