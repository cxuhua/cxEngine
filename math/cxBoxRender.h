//
//  cxBoxRanderF.h
//  cxEngineCore
//
//  Created by xuhua on 6/17/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#ifndef cxEngineCore_cxBoxRanderF_h
#define cxEngineCore_cxBoxRanderF_h

#include <vector>
#include "cxRenderF.h"
#include "cxBoxPoint3F.h"

CX_CPP_BEGIN

struct cxBoxColor4F;
struct cxBoxCoord2F;
struct cxBoxRender
{
    cxRenderF lt;
    cxRenderF lb;
    cxRenderF rt;
    cxRenderF rb;
    cxBoxRender();
    cxBoxRender(const cxBoxRender &v);
    void SetColor(const cxBoxColor4F &v);
    void SetColor(const cxColor4F &v);
    void SetVertices(const cxBoxPoint3F &v);
    void SetCoords(const cxBoxCoord2F &v);
    void SetAlpha(const cxFloat &v);
    cxBoxPoint3F ToBoxPoint();
    cxBoxRender &operator*=(const cxMatrixF &v);
    cxBoxRender operator*(const cxMatrixF &v);
};

class cxBoxRenderArray : private std::vector<cxBoxRender>
{
public:
    explicit cxBoxRenderArray();
    virtual ~cxBoxRenderArray();
private:
    cxInt number;
public:
    void Init(cxInt n);
    void Append(cxInt n);
    void Append(cxBoxRenderArray &v);
    void Append(const cxBoxRender &v);
    void Remove(cxInt idx);
    void Remove(cxInt idx,cxInt n);
    cxInt Inc(cxInt inc);
    cxBoxRender &Inc();
    void Clear();
    void SetAlpha(cxInt idx,cxFloat alpha);
    void SetAlpha(cxInt idx,cxInt n,cxFloat alpha);
    void SetColor(const cxColor4F &v);
    cxInt DataBytes() const;
    cxInt BufferBytes() const;
    const cxBoxRender *Buffer() const;
    cxBoxRender &At(cxInt idx);
    const cxBoxRender &At(cxInt idx) const;
    const cxInt Capacity() const;
    const cxInt Size() const;
    cxBool IsEmpty() const;
};

CX_CPP_END

#endif


