#pragma once

#include <directxmath/DirectXMath.h>
#include "Vector3.h"

struct Matrix4x4
{
    DirectX::XMFLOAT4X4 m;

    Matrix4x4()
    {
        DirectX::XMStoreFloat4x4(&m, DirectX::XMMatrixIdentity());
    }

    static Matrix4x4 Identity() { return Matrix4x4(); }

    static Matrix4x4 FromXM(DirectX::FXMMATRIX mat)
    {
        Matrix4x4 result;
        DirectX::XMStoreFloat4x4(&result.m, mat);
        return result;
    }

    DirectX::XMMATRIX ToXM() const
    {
        return DirectX::XMLoadFloat4x4(&m);
    }

    static Matrix4x4 Multiply(const Matrix4x4 &a, const Matrix4x4 &b)
    {
        return FromXM(DirectX::XMMatrixMultiply(a.ToXM(), b.ToXM()));
    }

    Vector3 TransformVector(const Vector3 &v) const
    {
        return Vector3(
            v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0],
            v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1],
            v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2]);
    }
};
