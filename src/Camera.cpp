#include "Camera.h"

Camera::Camera() : m_yaw(0.0f), m_pitch(0.0f)
{
    m_projection = Matrix4::Identity();
}

void Camera::setPerspective(float fovYRadians, float aspectRatio, float nearZ, float farZ)
{
    m_projection = Matrix4::Perspective(fovYRadians, aspectRatio, nearZ, farZ);
}

void Camera::update()
{
    m_transform.setRotation(Vector3(m_pitch, m_yaw, 0.0f));
}

void Camera::moveForward(float amount)
{
    m_transform.translate(m_transform.forward() * amount);
}

void Camera::moveRight(float amount)
{
    m_transform.translate(m_transform.right() * amount);
}

void Camera::moveUp(float amount)
{
    Vector3 worldUp(0.0f, 1.0f, 0.0f);
    m_transform.translate(worldUp * amount);
}

void Camera::rotate(float deltaYaw, float deltaPitch)
{
    m_yaw += deltaYaw;
    m_pitch += deltaPitch;
    
    if (m_pitch > 89.9f) m_pitch = 89.9f;
    if (m_pitch < -89.9f) m_pitch = -89.9f;
}

Matrix4 Camera::getViewMatrix() const
{
    Vector3 eye = m_transform.getPosition();
    Vector3 center = eye + m_transform.forward();
    Vector3 up = m_transform.up();
    return Matrix4::LookAt(eye, center, up);
}