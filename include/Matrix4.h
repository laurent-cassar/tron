#pragma once
#include "Vector3.h"
#include <cmath>
#include <cstring>

class Matrix4
{
public:
    float m[4][4];

    Matrix4()
    {
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                m[i][j] = (i == j) ? 1.0f : 0.0f;
    }

    static Matrix4 Identity()
    {
        return Matrix4();
    }

    static Matrix4 Translation(const Vector3& t)
    {
        Matrix4 mat;
        mat.m[3][0] = t.x;
        mat.m[3][1] = t.y;
        mat.m[3][2] = t.z;
        return mat;
    }

    static Matrix4 Scaling(const Vector3& s)
    {
        Matrix4 mat;
        mat.m[0][0] = s.x;
        mat.m[1][1] = s.y;
        mat.m[2][2] = s.z;
        return mat;
    }

    static Matrix4 RotationX(float rad)
    {
        Matrix4 mat;
        float c = cosf(rad);
        float sn = sinf(rad);
        mat.m[1][1] = c;
        mat.m[1][2] = sn;
        mat.m[2][1] = -sn;
        mat.m[2][2] = c;
        return mat;
    }

    static Matrix4 RotationY(float rad)
    {
        Matrix4 mat;
        float c = cosf(rad);
        float sn = sinf(rad);
        mat.m[0][0] = c;
        mat.m[0][2] = -sn;
        mat.m[2][0] = sn;
        mat.m[2][2] = c;
        return mat;
    }

    static Matrix4 RotationZ(float rad)
    {
        Matrix4 mat;
        float c = cosf(rad);
        float sn = sinf(rad);
        mat.m[0][0] = c;
        mat.m[0][1] = sn;
        mat.m[1][0] = -sn;
        mat.m[1][1] = c;
        return mat;
    }

    static Matrix4 Perspective(float fovY, float aspect, float nearZ, float farZ)
    {
        Matrix4 mat;
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                mat.m[i][j] = 0.0f;

        float cotangent = 1.0f / tanf(fovY * 0.5f);
        mat.m[0][0] = cotangent / aspect;
        mat.m[1][1] = cotangent;
        mat.m[2][2] = (farZ + nearZ) / (nearZ - farZ);
        mat.m[2][3] = -1.0f;
        mat.m[3][2] = (2.0f * farZ * nearZ) / (nearZ - farZ);
        return mat;
    }

    static Matrix4 LookAt(const Vector3& eye, const Vector3& center, const Vector3& up)
    {
        Vector3 f = (center - eye).normalized();
        Vector3 s = f.cross(up).normalized();
        Vector3 u = s.cross(f);

        Matrix4 mat;
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                mat.m[i][j] = 0.0f;

        mat.m[0][0] = s.x;
        mat.m[1][0] = s.y;
        mat.m[2][0] = s.z;
        mat.m[0][1] = u.x;
        mat.m[1][1] = u.y;
        mat.m[2][1] = u.z;
        mat.m[0][2] = -f.x;
        mat.m[1][2] = -f.y;
        mat.m[2][2] = -f.z;
        mat.m[3][0] = -s.dot(eye);
        mat.m[3][1] = -u.dot(eye);
        mat.m[3][2] = f.dot(eye);
        mat.m[3][3] = 1.0f;
        return mat;
    }

    Matrix4 operator*(const Matrix4& other) const
    {
        Matrix4 result;
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                result.m[i][j] = 0.0f;

        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                for (int k = 0; k < 4; k++)
                    result.m[i][j] += m[i][k] * other.m[k][j];

        return result;
    }
};