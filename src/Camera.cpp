#include "Camera.h"

using namespace DirectX;

Camera::Camera()
    : m_transform(), m_yaw(0.0f), m_pitch(0.0f), m_view(Matrix4x4::Identity()), m_projection(Matrix4x4::Identity())
{
    m_transform.SetPosition(Vector3(0.0f, 0.0f, -5.0f));
    Update();
}

void Camera::SetProjection(float fovYRadians, float aspectRatio, float nearZ, float farZ)
{
    m_projection = Matrix4x4::FromXM(XMMatrixPerspectiveFovLH(fovYRadians, aspectRatio, nearZ, farZ));
}

void Camera::Rotate(float deltaYaw, float deltaPitch)
{
    m_yaw += deltaYaw;
    m_pitch += deltaPitch;

    const float limit = XM_PIDIV2 - 0.01f;
    if (m_pitch > limit)
        m_pitch = limit;
    if (m_pitch < -limit)
        m_pitch = -limit;
}

void Camera::MoveForward(float amount)
{
    m_transform.Translate(m_transform.GetForward() * amount);
}

void Camera::MoveRight(float amount)
{
    m_transform.Translate(m_transform.GetRight() * amount);
}

void Camera::MoveUp(float amount)
{
    m_transform.Translate(m_transform.GetUp() * amount);
}

void Camera::Update()
{
    m_transform.SetRotation(Vector4::FromEuler(m_pitch, m_yaw, 0.0f));

    Vector3 eye = m_transform.GetPosition();
    Vector3 forward = m_transform.GetForward();
    Vector3 up = m_transform.GetUp();

    m_view = Matrix4x4::FromXM(
        XMMatrixLookToLH(eye.ToXM(), forward.ToXM(), up.ToXM()));
}