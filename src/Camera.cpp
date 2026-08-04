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
    // Direct3D attend une matrice column-major par défaut (cf slide "World/View/Projection")
    m_projection = Matrix4x4::FromXM(XMMatrixPerspectiveFovLH(fovYRadians, aspectRatio, nearZ, farZ));
}

void Camera::Rotate(float deltaYaw, float deltaPitch)
{
    m_yaw += deltaYaw;
    m_pitch += deltaPitch;

    // On limite le pitch pour éviter de "retourner" la caméra
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
    // yaw/pitch -> quaternion, poussé dans le Transform
    m_transform.SetRotation(Vector4::FromEuler(m_pitch, m_yaw, 0.0f));

    Vector3 eye = m_transform.GetPosition();
    Vector3 forward = m_transform.GetForward();
    Vector3 up = m_transform.GetUp();

    // LookTo (pas LookAt) : on donne une direction, pas un point cible,
    // ce qui colle bien à un Transform piloté par rotation.
    m_view = Matrix4x4::FromXM(
        XMMatrixLookToLH(eye.ToXM(), forward.ToXM(), up.ToXM()));
}