//
//  cxBoxColor4F.h
//  cxEngineCore
//
//  Created by xuhua on 6/17/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#ifndef cxEngineCore_cxBoxColor4F_h
#define cxEngineCore_cxBoxColor4F_h

#include <vector>
#include "cxColor4F.h"

CX_CPP_BEGIN

struct cxBoxColor4F
{
    cxColor4F lt;
    cxColor4F lb;
    cxColor4F rt;
    cxColor4F rb;
    cxBoxColor4F();
    cxBoxColor4F(const cxBoxColor4F &v);
    cxBoxColor4F(const cxColor4F &v);
    const cxBoxColor4F ToLoop() const;
    void SetAlpha(cxFloat alpha);
    void SetColor(const cxColor4F &color);
    const cxFloat Alpha() const;
    cxBool operator==(const cxColor4F &v) const;
    cxBool operator!=(const cxColor4F &v) const;
    const cxColor4F &Color() const;
};

class cxBoxColor4FArray : private std::vector<cxBoxColor4F>
{
public:
    explicit cxBoxColor4FArray();
    virtual ~cxBoxColor4FArray();
public:
    void Append(cxInt n);
    void Append(cxInt n,const cxColor4F &v);
    void Append(const cxBoxColor4F &v);
    void Remove(cxInt idx);
    void Remove(cxInt idx,cxInt n);
    void SetAlpha(cxInt idx,cxFloat alpha);
    void SetAlpha(cxInt idx,cxInt n,cxFloat alpha);
    void SetColor(cxInt idx,const cxColor4F &color);
    const cxBoxColor4F *Buffer() const;
    cxBoxColor4F &At(cxInt idx);
    const cxInt Size() const;
    cxBool IsEmpty() const;
};

CX_CPP_END

#endif


