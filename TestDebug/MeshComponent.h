#pragma once
#include "Component.h"
#include "Mesh.h"
#include <memory>

class MeshComponent : public Component {
public:
    MeshComponent(std::unique_ptr<Mesh> mesh) : m_Mesh(std::move(mesh)) {}
    Mesh* GetMesh() const { return m_Mesh.get(); }
    
private:
    std::unique_ptr<Mesh> m_Mesh;
};