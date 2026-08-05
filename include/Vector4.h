#pragma once

#include <directxmath/DirectXMath.h>

struct Vector4
{
    float x, y, z, w;

    Vector4() : x(0.0f), y(0.0f), z(0.0f), w(1.0f) {}
    Vector4(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w) {}

    static Vector4 Identity() { return Vector4(0.0f, 0.0f, 0.0f, 1.0f); }

    // Construit un quaternion à partir d'un axe et d'un angle (radians)
    static Vector4 FromAxisAngle(const Vector3 &axis, float angleRadians)
    {
        DirectX::XMVECTOR axisXM = axis.ToXM();
        DirectX::XMVECTOR q = DirectX::XMQuaternionRotationAxis(axisXM, angleRadians);
        return FromXM(q);
    }

    static Vector4 FromEuler(float pitch, float yaw, float roll)
    {
        DirectX::XMVECTOR q = DirectX::XMQuaternionRotationRollPitchYaw(pitch, yaw, roll);
        return FromXM(q);
    }

    DirectX::XMVECTOR ToXM() const { return DirectX::XMVectorSet(x, y, z, w); }
    static Vector4 FromXM(DirectX::FXMVECTOR v)
    {
        DirectX::XMFLOAT4 f;
        DirectX::XMStoreFloat4(&f, v);
        return Vector4(f.x, f.y, f.z, f.w);
    }
};
