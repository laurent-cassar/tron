#include "Transform.h"

Transform::Transform() : m_position(0, 0, 0), m_rotation(0, 0, 0), m_scale(1, 1, 1) {}

Vector3 Transform::forward() const
{
    float yaw = m_rotation.y * 3.14159f / 180.0f;
    float pitch = m_rotation.x * 3.14159f / 180.0f;
    return Vector3(
        sinf(yaw) * cosf(pitch),
        -sinf(pitch),
        cosf(yaw) * cosf(pitch)
    ).normalized();
}

Vector3 Transform::right() const
{
    return forward().cross(Vector3(0, 1, 0)).normalized();
}

Vector3 Transform::up() const
{
    return right().cross(forward()).normalized();
}

Matrix4 Transform::getMatrix() const
{
    float yaw = m_rotation.y * 3.14159f / 180.0f;
    float pitch = m_rotation.x * 3.14159f / 180.0f;
    float roll = m_rotation.z * 3.14159f / 180.0f;

    Matrix4 rotX = Matrix4::RotationX(pitch);
    Matrix4 rotY = Matrix4::RotationY(yaw);
    Matrix4 rotZ = Matrix4::RotationZ(roll);

    Matrix4 rot = rotY * rotX * rotZ;
    Matrix4 scale = Matrix4::Scaling(m_scale);
    Matrix4 trans = Matrix4::Translation(m_position);

    return trans * rot * scale;
}
