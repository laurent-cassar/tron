#include "Transform.h"

using namespace DirectX;

Transform::Transform()
    : m_position(Vector3::Zero()), m_rotation(Vector4::Identity()), m_scale(Vector3::One())
{
}

void Transform::SetPosition(const Vector3 &position)
{
    m_position = position;
}

void Transform::SetRotation(const Vector4 &rotationQuaternion)
{
    m_rotation = rotationQuaternion;
}

void Transform::SetScale(const Vector3 &scale)
{
    m_scale = scale;
}

void Transform::Translate(const Vector3 &delta)
{
    m_position += delta;
}

Vector3 Transform::GetForward() const
{
    XMVECTOR rotation = m_rotation.ToXM();
    XMVECTOR forward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
    return Vector3::FromXM(XMVector3Rotate(forward, rotation));
}

Vector3 Transform::GetRight() const
{
    XMVECTOR rotation = m_rotation.ToXM();
    XMVECTOR right = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
    return Vector3::FromXM(XMVector3Rotate(right, rotation));
}

Vector3 Transform::GetUp() const
{
    XMVECTOR rotation = m_rotation.ToXM();
    XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
    return Vector3::FromXM(XMVector3Rotate(up, rotation));
}

Matrix4x4 Transform::GetWorldMatrix() const
{
    XMVECTOR scaleXM = XMVectorSet(m_scale.x, m_scale.y, m_scale.z, 0.0f);
    XMVECTOR rotationXM = m_rotation.ToXM();
    XMVECTOR positionXM = XMVectorSet(m_position.x, m_position.y, m_position.z, 0.0f);

    XMMATRIX world = XMMatrixScalingFromVector(scaleXM) * XMMatrixRotationQuaternion(rotationXM) * XMMatrixTranslationFromVector(positionXM);

    return Matrix4x4::FromXM(world);
}
