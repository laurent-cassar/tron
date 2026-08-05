#pragma once

#include "Transform.h"

class Camera
{
public:
    Camera();

    void SetProjection(float fovYRadians, float aspectRatio, float nearZ, float farZ);

    void Update();

    void MoveForward(float amount);
    void MoveRight(float amount);
    void MoveUp(float amount);

    void Rotate(float deltaYaw, float deltaPitch);

    const Matrix4x4 &GetViewMatrix() const { return m_view; }
    const Matrix4x4 &GetProjectionMatrix() const { return m_projection; }

    const Vector3 &GetPosition() const { return m_transform.GetPosition(); }
    Transform &GetTransform() { return m_transform; }
    const Transform &GetTransform() const { return m_transform; }

private:
    Transform m_transform;

    float m_yaw;
    float m_pitch;

    Matrix4x4 m_view;
    Matrix4x4 m_projection;
};