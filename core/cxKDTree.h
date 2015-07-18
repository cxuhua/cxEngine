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
#include <ext/KDTree.h>
#include "cxArray.h"

CX_CPP_BEGIN

struct KDTreeNode
{
    static const cxInt DIM = 2;
    cxLong tag;
    cxFloat vs[2];
    KDTreeNode()
    {
        tag = 0;
        vs[0] = 0;
        vs[1] = 0;
    }
    KDTreeNode(const cxPoint2F &p,cxLong atag)
    {
        tag = atag;
        vs[0] = p.x;
        vs[1] = p.y;
    }
    const cxFloat *getPtr() const
    {
        return vs;
    }
    cxFloat operator[](std::size_t idx) const
    {
        return vs[idx];
    }
};

class cxKDTree : public cxObject
{
public:
    CX_DECLARE(cxKDTree);
protected:
    explicit cxKDTree();
    virtual ~cxKDTree();
public:
    typedef ofx::KDTree<KDTreeNode> KDTree;
    typedef ofx::KDTree<KDTreeNode>::SearchResults Results;
    typedef cxKDTree::Results::const_iterator Iter;
private:
    std::vector<KDTreeNode> nodes;
    KDTree tree;
    Results results;
public:
    cxKDTree *Clear();
    cxKDTree *Append(const cxPoint2F &p,cxLong tag);
    cxKDTree *Build();
    cxInt Nearst(const cxPoint2F &cp,cxFloat max);
    
    const Results &GetResults();
    
    const KDTreeNode &GetTreeNode(cxKDTree::Results::const_iterator it);
};

CX_CPP_END

#endif


