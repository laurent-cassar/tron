#include "Camera.h"
#include <algorithm> 
#include <cmath>     

using namespace DirectX;

Camera::Camera() //constructeur sans param
    : m_fovY(XM_PIDIV4), m_aspectRatio(16.0f / 9.0f), m_nearZ(0.1f), m_farZ(1000.0f)
{
}
//avec param(modifiable)
Camera::Camera(float fovYRadians, float aspectRatio, float nearZ, float farZ)
    : m_fovY(fovYRadians), m_aspectRatio(aspectRatio), m_nearZ(nearZ), m_farZ(farZ)
{
}

void Camera::Init()
{
    // Récupère et met en cache le transform du même GameObject dès l'init
    m_cachedTransform = GetTransform();
}

Transform* Camera::GetTransform() const
{
    if (!m_cachedTransform)
    {
        // à adapter selon le travail des potos
        m_cachedTransform = gameObject->GetComponent<Transform>();
    }
    return m_cachedTransform;
}

// matrices
XMMATRIX Camera::GetViewMatrix() const
{
    Transform* t = GetTransform();

    XMVECTOR position = XMLoadFloat3(&t->GetPosition());//convertie le XMFLOAT en XMVECTOR

    XMFLOAT3 fwd = t->GetForward();//recup la direction vers laquelle la cam regarde
    XMFLOAT3 up = t->GetUp();//recup vecteur haut

    XMVECTOR forward = XMLoadFloat3(&fwd);//convertir le XMFLOAT EN XMVECTOR
    XMVECTOR upVec = XMLoadFloat3(&up);

    return XMMatrixLookToLH(position, forward, upVec);//return la viewMatrix
}

XMMATRIX Camera::GetViewMatrixLookAt(const XMFLOAT3& targetWorldPos, const XMFLOAT3& worldUp) const
{
    XMVECTOR eye = XMLoadFloat3(&GetTransform()->GetPosition());//recup la position de la cam
    XMVECTOR target = XMLoadFloat3(&targetWorldPos);//converti la position de l'objet qu'on veut regarder
    XMVECTOR up = XMLoadFloat3(&worldUp); //même principe pour indiquer le haut 
    return XMMatrixLookAtLH(eye, target, up);
}

XMMATRIX Camera::GetProjectionMatrix() const
{
    return XMMatrixPerspectiveFovLH(m_fovY, m_aspectRatio, m_nearZ, m_farZ);
}

XMMATRIX Camera::GetViewProjectionMatrix() const
{
    return GetViewMatrix() * GetProjectionMatrix();
}

void Camera::SetFOV(float fovYRadians)
{
    m_fovY = fovYRadians;
}

void Camera::SetAspectRatio(float aspectRatio)
{
    m_aspectRatio = aspectRatio;
}

void Camera::SetAspectRatio(unsigned int screenWidth, unsigned int screenHeight)
{
    if (screenHeight == 0) return;
    m_aspectRatio = static_cast<float>(screenWidth) / static_cast<float>(screenHeight);
}

void Camera::SetNearFar(float nearZ, float farZ)
{
    m_nearZ = nearZ;
    m_farZ = farZ;
}

//deplacement
void Camera::MoveForward(float amount)
{
    Transform* t = GetTransform();
    XMFLOAT3 fwd = t->GetForward();
    XMFLOAT3 pos = t->GetPosition();

    XMFLOAT3 newPos
    {
        pos.x + fwd.x * amount,
        pos.y + fwd.y * amount,
        pos.z + fwd.z * amount
    };
    t->SetPosition(newPos);
}

void Camera::MoveRight(float amount)
{
    Transform* t = GetTransform();
    XMFLOAT3 right = t->GetRight();
    XMFLOAT3 pos = t->GetPosition();

    XMFLOAT3 newPos
    {
        pos.x + right.x * amount,
        pos.y + right.y * amount,
        pos.z + right.z * amount
    };
    t->SetPosition(newPos);
}

void Camera::MoveUp(float amount)
{
    Transform* t = GetTransform();
    XMFLOAT3 pos = t->GetPosition();
    pos.y += amount; // déplacement vertical sur le monde
    t->SetPosition(pos);
}

void Camera::Rotate(float pitchDelta, float yawDelta)
{
    m_pitch += pitchDelta;
    m_yaw += yawDelta;

    // Empêche le retournement complet de la cam
    constexpr float pitchLimit = XM_PIDIV2 - 0.01f;
    m_pitch = std::clamp(m_pitch, -pitchLimit, pitchLimit);

    // Reconstruit un quaternion propre à partir de pitch/yaw(hautbas,droitegauche) 
    XMVECTOR quat = XMQuaternionRotationRollPitchYaw(m_pitch, m_yaw, 0.0f);
    XMFLOAT4 q;
    XMStoreFloat4(&q, quat);

    GetTransform()->SetRotation(q);
}

//attachement à un objet
void Camera::AttachTo(GameObject* target, CameraMode mode)
{
    m_attachTarget = target;
    m_mode = mode;
}

void Camera::Detach()
{
    m_attachTarget = nullptr;
    m_mode = CameraMode::Free;
}

void Camera::SetThirdPersonOffset(const XMFLOAT3& offset)
{
    m_thirdPersonOffset = offset;
}

void Camera::SetFirstPersonOffset(const XMFLOAT3& offset)
{
    m_firstPersonOffset = offset;
}

void Camera::UpdateAttachment()
{
    if (!m_attachTarget || m_mode == CameraMode::Free)
        return; // rien à faire, la caméra se gère toute seule (via MoveForward/Rotate)

    Transform* targetTransform = m_attachTarget->GetComponent<Transform>();
    Transform* myTransform = GetTransform();

    XMFLOAT3 targetPos = targetTransform->GetWorldPosition();
    XMFLOAT3 targetFwd = targetTransform->GetForward();
    XMFLOAT3 targetUp = targetTransform->GetUp();
    XMFLOAT3 targetRight = targetTransform->GetRight();

    if (m_mode == CameraMode::FirstPerson)//set la cam en mode fps
    {
        XMVECTOR pos = XMLoadFloat3(&targetPos)
            + XMLoadFloat3(&targetRight) * m_firstPersonOffset.x
            + XMLoadFloat3(&targetUp) * m_firstPersonOffset.y
            + XMLoadFloat3(&targetFwd) * m_firstPersonOffset.z;

        XMFLOAT3 finalPos;
        XMStoreFloat3(&finalPos, pos);
        myTransform->SetPosition(finalPos);
        myTransform->SetRotation(targetTransform->GetRotation()); // même orientation que la cible
    }
    else if (m_mode == CameraMode::ThirdPerson)//set la cam en thirdperson avec le look at
    {
        XMVECTOR pos = XMLoadFloat3(&targetPos)
            + XMLoadFloat3(&targetRight) * m_thirdPersonOffset.x
            + XMLoadFloat3(&targetUp) * m_thirdPersonOffset.y
            + XMLoadFloat3(&targetFwd) * m_thirdPersonOffset.z;

        XMFLOAT3 finalPos;
        XMStoreFloat3(&finalPos, pos);
        myTransform->SetPosition(finalPos);
    }
}

//fustrum culling temporaire (in work)
std::array<XMFLOAT4, 6> Camera::GetFrustumPlanes() const
{
    std::array<XMFLOAT4, 6> planes{};

    XMMATRIX vp = GetViewProjectionMatrix();
    XMFLOAT4X4 m;
    XMStoreFloat4x4(&m, vp);

    /*planes[0] = {m._14 + m._11, m._24 + m._21, m._34 + m._31, m._44 + m._41}; // Left
    planes[1] = { m._14 - m._11, m._24 - m._21, m._34 - m._31, m._44 - m._41 }; // Right
    planes[2] = { m._14 + m._12, m._24 + m._22, m._34 + m._32, m._44 + m._42 }; // Bottom
    planes[3] = { m._14 - m._12, m._24 - m._22, m._34 - m._32, m._44 - m._42 }; // Top
    planes[4] = { m._13,          m._23,          m._33,          m._43 };          // Near
    planes[5] = { m._14 - m._13, m._24 - m._23, m._34 - m._33, m._44 - m._43 };*/

    /* Normalisation de chaque plan
    for (auto& p : planes)
    {
        float length = std::sqrt(p.x * p.x + p.y * p.y + p.z * p.z);
        if (length > 0.0001f)
        {
            p.x /= length;
            p.y /= length;
            p.z /= length;
            p.w /= length;
        }
    }

    return planes;*/
}