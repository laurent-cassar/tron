#pragma once

#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4x4.h"

// Transform : décrit où se trouve un objet dans le monde 3D, comment il est
// orienté, et à quelle échelle. C'est la brique de base réutilisée par
// TOUT ce qui existe dans la scène : la Camera, les meshes, les lumières...
//
// Sa seule vraie responsabilité : combiner Position / Rotation / Scale en
// une World Matrix (Scale * Rotation * Translation), au format attendu par
// Direct3D pour être envoyée au Vertex Shader (cf slides World/View/Projection).
class Transform
{
public:
    Transform();

    void SetPosition(const Vector3& position);
    void SetRotation(const Vector4& rotationQuaternion);
    void SetScale(const Vector3& scale);

    void Translate(const Vector3& delta);

    const Vector3& GetPosition() const { return m_position; }
    const Vector4& GetRotation() const { return m_rotation; }
    const Vector3& GetScale() const { return m_scale; }

    // Vecteurs directionnels dérivés de la rotation actuelle
    // (utile pour les déplacements caméra "relatifs" : avancer, aller à droite, etc.)
    Vector3 GetForward() const;
    Vector3 GetRight() const;
    Vector3 GetUp() const;

    // Combine Position/Rotation/Scale en une seule matrice (World Matrix)
    Matrix4x4 GetWorldMatrix() const;

private:
    Vector3 m_position;
    Vector4 m_rotation; // quaternion
    Vector3 m_scale;
};
