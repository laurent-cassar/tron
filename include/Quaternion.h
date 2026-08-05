#pragma once

#include "Vector3.h"
#include "Matrix4x4.h"

struct Quaternion
{
    float w, x, y, z;

    Quaternion() : w(1.0f), x(0.0f), y(0.0f), z(0.0f) {}
    Quaternion(float _w, float _x, float _y, float _z) : w(_w), x(_x), y(_y), z(_z) {}

    static Quaternion Identity() { return Quaternion(1.0f, 0.0f, 0.0f, 0.0f); }

    static Quaternion FromAxisAngle(const Vector3 &axis, float angleRadians);

    static Quaternion FromEulerAngles(float pitchX, float yawY, float rollZ);

    Quaternion operator*(const Quaternion &other) const;

    float LengthSquared() const;
    float Length() const;

    Quaternion Normalized() const;

    void Normalize();

    Quaternion Conjugate() const;

    Quaternion Inverse() const;

    static float Dot(const Quaternion &a, const Quaternion &b);

    Vector3 RotateVector(const Vector3 &v) const;

    Matrix4x4 ToMatrix4x4() const;

    static Quaternion FromMatrix4x4(const Matrix4x4 &mat);
};