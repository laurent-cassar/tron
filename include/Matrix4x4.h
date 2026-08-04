#pragma once

#include <directxmath/DirectXMath.h>

// Matrix4x4 : wrapper autour de XMFLOAT4X4/XMMATRIX.
// Sert à représenter la World Matrix (Transform), la View Matrix et
// la Projection Matrix (Camera) avant de les envoyer au Vertex Shader.
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

    static Matrix4x4 Multiply(const Matrix4x4& a, const Matrix4x4& b)
    {
        return FromXM(DirectX::XMMatrixMultiply(a.ToXM(), b.ToXM()));
    }
};
