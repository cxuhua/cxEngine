//
//  cxRenderF.h
//  cxEngineCore
//
//  Created by xuhua on 6/17/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#ifndef cxEngineCore_cxRenderF_h
#define cxEngineCore_cxRenderF_h

#include "cxPoint3F.h"
#include "cxColor4F.h"
#include "cxBoxCoord2F.h"
#include "cxMatrixF.h"

CX_CPP_BEGIN

struct cxRenderF
{
    cxPoint3F vertices;
    cxColor4F colors;
    cxCoord2F coords;
    cxRenderF();
    cxRenderF(const cxRenderF &v);
    cxRenderF &operator*=(const cxMatrixF &v);
    cxRenderF operator*(const cxMatrixF &v);
};

class cxRenderFArray : private std::vector<cxRenderF>
{
public:
    explicit cxRenderFArray();
    virtual ~cxRenderFArray();
private:
    cxInt number;
public:
    
    void Append(cxInt n);
    void Append(cxRenderFArray &v);
    void Append(const cxRenderF &v);
    
    void Remove(cxInt idx);
    void Remove(cxInt idx,cxInt n);
    
    cxInt Inc(cxInt inc);
    cxRenderF &Inc();
    void Clear();

    const cxRenderF *Buffer() const;
    cxRenderF &At(cxInt idx);
    const cxRenderF &At(cxInt idx) const;
    const cxInt Capacity() const;
    const cxInt Size() const;
    cxBool IsEmpty() const;
};

CX_CPP_END

#endif


