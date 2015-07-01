//
//  cxMatrixF.cpp
//  cxEngineCore
//
//  Created by xuhua on 6/14/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#include <string.h>
#include "cxPoint2F.h"
#include "cxPoint3F.h"
#include "cxMatrixF.h"

CX_CPP_BEGIN

void cxMatrixF::Identity()
{
    kmMat4Identity(&mat4);
}

cxMatrixF::cxMatrixF()
{
    Identity();
}

cxMatrixF::cxMatrixF(const cxMatrixF &v)
{
    kmMat4Fill(&mat4, v.mat4.mat);
}

cxMatrixF::cxMatrixF(kmMat4 *data)
{
    kmMat4Fill(&mat4, &data->mat[0]);
}

cxMatrixF cxMatrixF::Inverse() const
{
    cxMatrixF out;
    kmMat4Inverse(&out.mat4, &mat4);
    return out;
}

cxFloat cxMatrixF::At(cxInt idx) const
{
    CX_ASSERT(idx >= 0 && idx < 16, "idx error");
    return mat4.mat[idx];
}

cxMatrixF cxMatrixF::Transpose() const
{
    cxMatrixF out;
    kmMat4Transpose(&out.mat4, &mat4);
    return out;
}

cxBool cxMatrixF::IsIdentity() const
{
    return kmMat4IsIdentity(&mat4);
}

void cxMatrixF::Rotation(const cxPoint3F &axis,cxFloat radians)
{
    Rotation(axis.x, axis.y, axis.z, radians);
}

void cxMatrixF::Rotation(const cxPoint2F &axis,cxFloat radians)
{
    Rotation(axis.x, axis.y, 0.0f, radians);
}

void cxMatrixF::Rotation(cxFloat x,cxFloat y,cxFloat z,cxFloat radians)
{
    kmMat4 out;
    kmVec3 ap;
    kmVec3Fill(&ap, x, y, z);
    kmMat4RotationAxisAngle(&out, &ap, radians);
    kmMat4Multiply(&mat4, &mat4, &out);
}

void cxMatrixF::RotationX(cxFloat radians)
{
    kmMat4 out;
    kmMat4RotationX(&out, radians);
    kmMat4Multiply(&mat4, &mat4, &out);
}

void cxMatrixF::RotationY(cxFloat radians)
{
    kmMat4 out;
    kmMat4RotationY(&out, radians);
    kmMat4Multiply(&mat4, &mat4, &out);
}

void cxMatrixF::RotationZ(cxFloat radians)
{
    kmMat4 out;
    kmMat4RotationZ(&out, radians);
    kmMat4Multiply(&mat4, &mat4, &out);
}

void *cxMatrixF::Data()
{
    return mat4.mat;
}

void cxMatrixF::Scaling(const cxPoint2F &v)
{
    Scaling(v.x, v.y, 1.0f);
}

void cxMatrixF::Scaling(const cxPoint3F &v)
{
    Scaling(v.x, v.y, v.z);
}

void cxMatrixF::Scaling(cxFloat x,cxFloat y,cxFloat z)
{
    kmMat4 out;
    kmMat4Scaling(&out, x, y, z);
    kmMat4Multiply(&mat4, &mat4, &out);
}

void cxMatrixF::InitTrans(const cxPoint2F &v)
{
    kmMat4Translation(&mat4, v.x, v.y, 0.0f);
}

void cxMatrixF::InitTrans(cxFloat x,cxFloat y,cxFloat z)
{
    kmMat4Identity(&mat4);
    kmMat4Translation(&mat4, x, y, z);
}

void cxMatrixF::Translation(const cxPoint2F &v)
{
    Translation(v.x,v.y,0.0f);
}

void cxMatrixF::Translation(const cxPoint3F &v)
{
    Translation(v.x,v.y,v.z);
}

void cxMatrixF::Translation(cxFloat x,cxFloat y,cxFloat z)
{
    kmMat4 out;
    kmMat4Translation(&out, x, y, z);
    kmMat4Multiply(&mat4, &mat4, &out);
}

cxBool cxMatrixF::operator==(const cxMatrixF &v) const
{
    return kmMat4AreEqual(&mat4, &v.mat4);
}

cxBool cxMatrixF::operator!=(const cxMatrixF &v) const
{
    return !kmMat4AreEqual(&mat4, &v.mat4);
}

cxMatrixF cxMatrixF::operator*(const cxMatrixF &v) const
{
    cxMatrixF out;
    kmMat4Multiply(&out.mat4, &mat4, &v.mat4);
    return out;
}

cxMatrixF &cxMatrixF::operator*=(const cxMatrixF &v)
{
    kmMat4Multiply(&mat4, &mat4, &v.mat4);
    return *this;
}


cxMatrixArray::cxMatrixArray()
{
    
}

cxMatrixArray::~cxMatrixArray()
{
    
}

void cxMatrixArray::Append(cxInt n)
{
    for(cxInt i=0;i<n;i++){
        push_back(cxMatrixF());
    }
}

void cxMatrixArray::Append(const cxMatrixF &v)
{
    push_back(v);
}

void cxMatrixArray::Remove(cxInt idx)
{
    erase(begin() + idx);
}

void cxMatrixArray::Remove(cxInt idx,cxInt n)
{
    erase(begin() + idx, begin() + idx + n);
}

const cxMatrixF *cxMatrixArray::Buffer() const
{
    return data();
}

cxMatrixF &cxMatrixArray::At(cxInt idx)
{
    return at(idx);
}

const cxInt cxMatrixArray::Size() const
{
    return (cxInt)size();
}

cxBool cxMatrixArray::IsEmpty()
{
    return empty();
}

CX_CPP_END

