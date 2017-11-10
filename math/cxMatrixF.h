//
//  cxMatrixF.h
//  cxEngineCore
//
//  Created by xuhua on 6/14/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#ifndef cxEngineCore_cxMatrixF_h
#define cxEngineCore_cxMatrixF_h

#include <vector>
#include "cxMath.h"

CX_CPP_BEGIN

struct cxPoint3F;
struct cxPoint2F;
struct cxMatrixF
{
    kmMat4 mat4;
    cxMatrixF();
    cxMatrixF(const cxMatrixF &v);
    cxMatrixF(kmMat4 *data);
    cxMatrixF(const cxPoint3F &trans);
    void Identity();
    void *Data();
    cxFloat At(cxInt idx) const;
    cxMatrixF Inverse() const;
    cxMatrixF Transpose() const;
    cxBool IsIdentity() const;
    cxBool operator==(const cxMatrixF &v) const;
    cxBool operator!=(const cxMatrixF &v) const;
    cxMatrixF operator*(const cxMatrixF &v) const;
    cxMatrixF &operator*=(const cxMatrixF &v);
    void InitScaling(const cxPoint2F &v);
    void InitScaling(const cxPoint3F &v);
    void Scaling(const cxPoint2F &v);
    void Scaling(const cxPoint3F &v);
    void Scaling(cxFloat x,cxFloat y,cxFloat z);
    void InitTrans(const cxPoint2F &v);
    void InitTrans(cxFloat x,cxFloat y,cxFloat z);
    void Translation(const cxPoint2F &v);
    void Translation(const cxPoint3F &v);
    void Translation(cxFloat x,cxFloat y,cxFloat z);
    void InitRotation(const cxPoint3F &axis,cxFloat radians);
    void Rotation(const cxPoint3F &axis,cxFloat radians);
    void Rotation(const cxPoint2F &axis,cxFloat radians);
    void Rotation(cxFloat x,cxFloat y,cxFloat z,cxFloat radians);
    void RotationX(cxFloat radians);
    void RotationY(cxFloat radians);
    void RotationZ(cxFloat radians);
};

class cxMatrixArray : private std::vector<cxMatrixF>
{
public:
    explicit cxMatrixArray();
    virtual ~cxMatrixArray();
public:
    void Append(cxInt n);
    void Append(const cxMatrixF &v);
    void Remove(cxInt idx);
    void Remove(cxInt idx,cxInt n);
    const cxMatrixF *Buffer()const ;
    cxMatrixF &At(cxInt idx);
    const cxInt Size() const;
    cxBool IsEmpty();
};

CX_CPP_END

#endif


