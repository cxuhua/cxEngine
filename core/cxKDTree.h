//
//  cxKDTree.h
//  cxEngineCore
//
//  Created by xuhua on 7/18/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#ifndef cxEngineCore_cxKDTree_h
#define cxEngineCore_cxKDTree_h

#include <vector>
#include <math/cxPoint2F.h>
#define KDTREE_SCALE float
#include <ext/kdtree.h>
#include "cxArray.h"

CX_CPP_BEGIN

struct cxTreeNode
{
    void *data;
    cxPoint2F pt;
    cxTreeNode()
    {
        data = NULL;
    }
    cxTreeNode(const cxFloat x,cxFloat y,void *ptr)
    {
        data = ptr;
        pt = cxPoint2F(x, y);
    }
};

class cxKDTree : public cxObject
{
public:
    CX_DECLARE(cxKDTree);
protected:
    explicit cxKDTree();
    virtual ~cxKDTree();
private:
    std::vector<cxTreeNode> nodes;
    struct kdtree *tree;
public:
    cxKDTree *Clear();
    cxKDTree *Append(const cxPoint2F &p,void *ref);
    cxInt Nearst(const cxPoint2F &cp,cxFloat max);
};

CX_CPP_END

#endif


