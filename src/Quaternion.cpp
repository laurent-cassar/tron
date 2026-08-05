#include "Quaternion.h"
#include <cmath>

Quaternion Quaternion::FromAxisAngle(const Vector3 &axis, float angleRadians)
{
    Vector3 a = axis.Normalized();
    float halfAngle = angleRadians * 0.5f;
    float s = std::sin(halfAngle);

    return Quaternion(std::cos(halfAngle), a.x * s, a.y * s, a.z * s);
}

Quaternion Quaternion::FromEulerAngles(float pitchX, float yawY, float rollZ)
{
    Quaternion qPitch = FromAxisAngle(Vector3::Right(), pitchX);
    Quaternion qYaw = FromAxisAngle(Vector3::Up(), yawY);
    Quaternion qRoll = FromAxisAngle(Vector3::Forward(), rollZ);

    return (qYaw * qPitch * qRoll).Normalized();
}

Quaternion Quaternion::operator*(const Quaternion &other) const
{
    const Quaternion &a = *this;
    const Quaternion &b = other;

    return Quaternion(
        a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z, // w
        a.w * b.x + a.x * b.w + a.y * b.z - a.z * b.y, // x
        a.w * b.y - a.x * b.z + a.y * b.w + a.z * b.x, // y
        a.w * b.z + a.x * b.y - a.y * b.x + a.z * b.w  // z
    );
}

float Quaternion::LengthSquared() const
{
    return w * w + x * x + y * y + z * z;
}

float Quaternion::Length() const
{
    return std::sqrt(LengthSquared());
}

Quaternion Quaternion::Normalized() const
{
    float len = Length();
    if (len < 1e-6f)
    {
        return Quaternion::Identity();
    }

    float invLen = 1.0f / len;
    return Quaternion(w * invLen, x * invLen, y * invLen, z * invLen);
}

void Quaternion::Normalize()
{
    *this = Normalized();
}

Quaternion Quaternion::Conjugate() const
{
    return Quaternion(w, -x, -y, -z);
}

Quaternion Quaternion::Inverse() const
{
    float lenSq = LengthSquared();
    if (lenSq < 1e-12f)
    {
        return Quaternion::Identity();
    }

    Quaternion conj = Conjugate();
    float invLenSq = 1.0f / lenSq;
    return Quaternion(conj.w * invLenSq, conj.x * invLenSq, conj.y * invLenSq, conj.z * invLenSq);
}

float Quaternion::Dot(const Quaternion &a, const Quaternion &b)
{
    return a.w * b.w + a.x * b.x + a.y * b.y + a.z * b.z;
}

Vector3 Quaternion::RotateVector(const Vector3 &v) const
{
    Quaternion p(0.0f, v.x, v.y, v.z);
    Quaternion result = (*this) * p * Conjugate();
    return Vector3(result.x, result.y, result.z);
}

Matrix4x4 Quaternion::ToMatrix4x4() const
{
    Quaternion q = Normalized();

    Matrix4x4 result;

    result.m.m[0][0] = 1.0f - 2.0f * (q.y * q.y + q.z * q.z);
    result.m.m[0][1] = 2.0f * (q.x * q.y + q.w * q.z);
    result.m.m[0][2] = 2.0f * (q.x * q.z - q.w * q.y);
    result.m.m[0][3] = 0.0f;

    result.m.m[1][0] = 2.0f * (q.x * q.y - q.w * q.z);
    result.m.m[1][1] = 1.0f - 2.0f * (q.x * q.x + q.z * q.z);
    result.m.m[1][2] = 2.0f * (q.y * q.z + q.w * q.x);
    result.m.m[1][3] = 0.0f;

    result.m.m[2][0] = 2.0f * (q.x * q.z + q.w * q.y);
    result.m.m[2][1] = 2.0f * (q.y * q.z - q.w * q.x);
    result.m.m[2][2] = 1.0f - 2.0f * (q.x * q.x + q.y * q.y);
    result.m.m[2][3] = 0.0f;

    result.m.m[3][0] = 0.0f;
    result.m.m[3][1] = 0.0f;
    result.m.m[3][2] = 0.0f;
    result.m.m[3][3] = 1.0f;

    return result;
}

Quaternion Quaternion::FromMatrix4x4(const Matrix4x4 &mat)
{
    const auto &m = mat.m.m;
    float trace = m[0][0] + m[1][1] + m[2][2];

    Quaternion q;

    if (trace > 0.0f)
    {
        float s = std::sqrt(trace + 1.0f) * 2.0f; // s = 4 * w
        q.w = 0.25f * s;
        q.x = (m[1][2] - m[2][1]) / s;
        q.y = (m[2][0] - m[0][2]) / s;
        q.z = (m[0][1] - m[1][0]) / s;
    }
    else if (m[0][0] > m[1][1] && m[0][0] > m[2][2])
    {
        float s = std::sqrt(1.0f + m[0][0] - m[1][1] - m[2][2]) * 2.0f; // s = 4 * x
        q.w = (m[1][2] - m[2][1]) / s;
        q.x = 0.25f * s;
        q.y = (m[0][1] + m[1][0]) / s;
        q.z = (m[0][2] + m[2][0]) / s;
    }
    else if (m[1][1] > m[2][2])
    {
        float s = std::sqrt(1.0f + m[1][1] - m[0][0] - m[2][2]) * 2.0f; // s = 4 * y
        q.w = (m[2][0] - m[0][2]) / s;
        q.x = (m[0][1] + m[1][0]) / s;
        q.y = 0.25f * s;
        q.z = (m[1][2] + m[2][1]) / s;
    }
    else
    {
        float s = std::sqrt(1.0f + m[2][2] - m[0][0] - m[1][1]) * 2.0f; // s = 4 * z
        q.w = (m[0][1] - m[1][0]) / s;
        q.x = (m[0][2] + m[2][0]) / s;
        q.y = (m[1][2] + m[2][1]) / s;
        q.z = 0.25f * s;
    }

    return q.Normalized();
}