#pragma once

#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4x4.h"

class Transform
{
public:
    Transform();

    void SetPosition(const Vector3 &position);
    void SetRotation(const Vector4 &rotationQuaternion);
    void SetScale(const Vector3 &scale);

    void Translate(const Vector3 &delta);

    const Vector3 &GetPosition() const { return m_position; }
    const Vector4 &GetRotation() const { return m_rotation; }
    const Vector3 &GetScale() const { return m_scale; }

    Vector3 GetForward() const;
    Vector3 GetRight() const;
    Vector3 GetUp() const;

    Matrix4x4 GetWorldMatrix() const;

private:
    Vector3 m_position;
    Vector4 m_rotation;
    Vector3 m_scale;
};
