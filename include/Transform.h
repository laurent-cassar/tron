#pragma once

#include "Vector3.h"
#include "Matrix4.h"

class Transform
{
public:
    Transform();
    
    void setPosition(const Vector3& pos) { m_position = pos; }
    void setRotation(const Vector3& rot) { m_rotation = rot; }
    void setScale(const Vector3& scl) { m_scale = scl; }
    
    Vector3 getPosition() const { return m_position; }
    Vector3 getRotation() const { return m_rotation; }
    Vector3 getScale() const { return m_scale; }
    
    void translate(const Vector3& offset) { m_position = m_position + offset; }
    void rotate(const Vector3& delta) { m_rotation = m_rotation + delta; }
    
    Vector3 forward() const;
    Vector3 right() const;
    Vector3 up() const;
    
    Matrix4 getMatrix() const;

private:
    Vector3 m_position;
    Vector3 m_rotation;
    Vector3 m_scale;
};
