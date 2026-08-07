#pragma once

#include "Transform.h"
#include "Matrix4.h"

class Camera
{
public:
    Camera();

    void setPerspective(float fovYRadians, float aspectRatio, float nearZ, float farZ);

    void update();

    void moveForward(float amount);
    void moveRight(float amount);
    void moveUp(float amount);

    void rotate(float deltaYaw, float deltaPitch);

    Matrix4 getViewMatrix() const;
    const Matrix4& getProjectionMatrix() const { return m_projection; }

    Vector3 getPosition() const { return m_transform.getPosition(); }
    Transform& getTransform() { return m_transform; }
    const Transform& getTransform() const { return m_transform; }

private:
    Transform m_transform;
    float m_yaw;
    float m_pitch;
    Matrix4 m_projection;
};