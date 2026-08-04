#pragma once

#include "Transform.h"

// Camera : utilise un Transform pour sa position/orientation dans le monde,
// et calcule à partir de ça la View Matrix (+ conserve la Projection Matrix).
//
// yaw/pitch restent gérés ici (pas dans Transform) car ce sont des entrées
// utilisateur "brutes" (souris) plus simples à manipuler qu'un quaternion
// directement ; Camera les convertit en quaternion et les pousse dans le
// Transform à chaque frame.
class Camera
{
public:
    Camera();

    // A appeler une fois au lancement (ou au resize de la fenêtre)
    void SetProjection(float fovYRadians, float aspectRatio, float nearZ, float farZ);

    // A appeler chaque frame, avant le bind du buffer de matrices au Vertex Shader
    void Update();

    // Déplacements relatifs à l'orientation de la caméra (axes locaux du Transform)
    void MoveForward(float amount);
    void MoveRight(float amount);
    void MoveUp(float amount);

    // Rotation (souris) en radians
    void Rotate(float deltaYaw, float deltaPitch);

    const Matrix4x4 &GetViewMatrix() const { return m_view; }
    const Matrix4x4 &GetProjectionMatrix() const { return m_projection; }

    const Vector3 &GetPosition() const { return m_transform.GetPosition(); }
    Transform &GetTransform() { return m_transform; }
    const Transform &GetTransform() const { return m_transform; }

private:
    Transform m_transform;

    float m_yaw;   // rotation autour de Y (gauche/droite)
    float m_pitch; // rotation autour de X (haut/bas)

    Matrix4x4 m_view;
    Matrix4x4 m_projection;
};