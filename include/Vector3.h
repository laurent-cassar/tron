#pragma once

#include <directxmath/DirectXMath.h>

struct Vector3
{
    float x, y, z;

    Vector3() : x(0.0f), y(0.0f), z(0.0f) {}
    Vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}

    static Vector3 Zero() { return Vector3(0.0f, 0.0f, 0.0f); }
    static Vector3 One() { return Vector3(1.0f, 1.0f, 1.0f); }
    static Vector3 Up() { return Vector3(0.0f, 1.0f, 0.0f); }
    static Vector3 Forward() { return Vector3(0.0f, 0.0f, 1.0f); }
    static Vector3 Right() { return Vector3(1.0f, 0.0f, 0.0f); }

    Vector3 operator+(const Vector3 &other) const { return Vector3(x + other.x, y + other.y, z + other.z); }
    Vector3 operator-(const Vector3 &other) const { return Vector3(x - other.x, y - other.y, z - other.z); }
    Vector3 operator*(float scalar) const { return Vector3(x * scalar, y * scalar, z * scalar); }

    Vector3 &operator+=(const Vector3 &other)
    {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }

    float Length() const
    {
        DirectX::XMVECTOR v = ToXM();
        return DirectX::XMVectorGetX(DirectX::XMVector3Length(v));
    }

    Vector3 Normalized() const
    {
        DirectX::XMVECTOR v = ToXM();
        DirectX::XMVECTOR n = DirectX::XMVector3Normalize(v);
        return FromXM(n);
    }

    static float Dot(const Vector3 &a, const Vector3 &b)
    {
        DirectX::XMVECTOR result = DirectX::XMVector3Dot(a.ToXM(), b.ToXM());
        return DirectX::XMVectorGetX(result);
    }

    static Vector3 Cross(const Vector3 &a, const Vector3 &b)
    {
        return FromXM(DirectX::XMVector3Cross(a.ToXM(), b.ToXM()));
    }

    DirectX::XMVECTOR ToXM() const { return DirectX::XMVectorSet(x, y, z, 0.0f); }
    static Vector3 FromXM(DirectX::FXMVECTOR v)
    {
        DirectX::XMFLOAT3 f;
        DirectX::XMStoreFloat3(&f, v);
        return Vector3(f.x, f.y, f.z);
    }
};
