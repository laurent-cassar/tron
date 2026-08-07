#pragma once

#include <DirectXMath.h>
#include <array>
#include "Component.h"   // classe de base des composants, à adapter selon le travail des potos
#include "transform.h"    // Camera dépend du transform du même GameObject
#include "GameObject.h"   // pour AttachTo() et GetComponent<T>() temporaire

enum class CameraMode
{
    Free, //libre
    FirstPerson, //suit la cible
    ThirdPerson //look at
};

class Camera : public Component
{
public:
    Camera();
    Camera(float fovYRadians, float aspectRatio, float nearZ, float farZ);
    ~Camera() override = default;

   
    void Init() override;

    // partie matrice
    DirectX::XMMATRIX GetViewMatrix() const;
    DirectX::XMMATRIX GetViewMatrixLookAt(const DirectX::XMFLOAT3& targetWorldPos,
        const DirectX::XMFLOAT3& worldUp = { 0.0f, 1.0f, 0.0f }) const;
    DirectX::XMMATRIX GetProjectionMatrix() const;
    DirectX::XMMATRIX GetViewProjectionMatrix() const;

    // projection
    void SetFOV(float fovYRadians);
    void SetAspectRatio(float aspectRatio);
    void SetAspectRatio(unsigned int screenWidth, unsigned int screenHeight);
    void SetNearFar(float nearZ, float farZ);

    float GetFOV() const { return m_fovY; }
    float GetAspectRatio() const { return m_aspectRatio; }
    float GetNearZ() const { return m_nearZ; }
    float GetFarZ() const { return m_farZ; }

    // --- Déplacement à appeler avec les input
    void MoveForward(float amount);
    void MoveRight(float amount);
    void MoveUp(float amount);
    void Rotate(float pitchDelta, float yawDelta); // en radians controlé par la souris

    // attachement à objet
    void AttachTo(GameObject* target, CameraMode mode);
    void Detach();
    void UpdateAttachment(); // à appeler chaque frame si la caméra est attachée

    CameraMode GetMode() const { return m_mode; }
    GameObject* GetAttachTarget() const { return m_attachTarget; }

    void SetThirdPersonOffset(const DirectX::XMFLOAT3& offset); 
    void SetFirstPersonOffset(const DirectX::XMFLOAT3& offset); 

    // --- Frustum culling(in work)
    std::array<DirectX::XMFLOAT4, 6> GetFrustumPlanes() const;

    // Camera active ou non 
    bool IsMainCamera() const { return m_isMain; }
    void SetAsMainCamera(bool isMain) { m_isMain = isMain; }

private:
    Transform* GetTransform() const; 

private:
    float m_fovY = DirectX::XM_PIDIV4;   
    float m_aspectRatio = 16.0f / 9.0f;
    float m_nearZ = 0.1f;
    float m_farZ = 1000.0f;

    float m_pitch = 0.0f; 
    float m_yaw = 0.0f;   

    bool m_isMain = false;

    // Attache à une cible
    GameObject* m_attachTarget = nullptr;
    CameraMode m_mode = CameraMode::Free;
    DirectX::XMFLOAT3 m_thirdPersonOffset{ 0.0f, 3.0f, -8.0f };
    DirectX::XMFLOAT3 m_firstPersonOffset{ 0.0f, 0.5f, 0.0f };

    mutable Transform* m_cachedTransform = nullptr;
};