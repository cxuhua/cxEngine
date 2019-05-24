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

cxKDTree::cxKDTree()
{
    tree = kd_create(2);
}

cxKDTree::~cxKDTree()
{
    kd_free(tree);
}

cxInt cxKDTree::Nearst(const cxPoint2F &cp,cxFloat max)
{
    const KDTREE_SCALE ps[]={cp.x,cp.y};
    nodes.clear();
    struct kdres *set = kd_nearest_range(tree, ps, max);
    if(set == NULL){
        return 0;
    }
    KDTREE_SCALE pos[2];
    while( !kd_res_end( set ) ) {
        void *data = kd_res_item(set, pos );
        cxTreeNode node(pos[0],pos[1],data);
        nodes.push_back(node);
        kd_res_next( set );
    }
    kd_res_free(set);
    return (cxInt)nodes.size();
}

cxKDTree *cxKDTree::Clear()
{
    kd_clear(tree);
    return this;
}

cxKDTree *cxKDTree::Append(const cxPoint2F &p,void *data)
{
    const KDTREE_SCALE ps[]={p.x,p.y};
    kd_insert(tree, ps, data);
    return this;
}

CX_CPP_END

