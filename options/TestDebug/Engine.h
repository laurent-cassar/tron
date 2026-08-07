#pragma once
#include "Component.h"
#include <vector>
#include <memory>

// Structure Vertex fournie dans l'exemple du prof
struct Vertex {
    float position[3];
    float color[3];
};

// Classe Mesh gérant les buffers (utilisée par le prof)
class Mesh {
public:
    Mesh(std::initializer_list<Vertex> vertices, std::initializer_list<unsigned int> indices) 
        : m_Vertices(vertices), m_Indices(indices) {}
private:
    std::vector<Vertex> m_Vertices;
    std::vector<unsigned int> m_Indices;
    // Plus tard, ton collègue Direct3D y ajoutera les ID3D11Buffer*
};

// 1. Le Composant Caméra
class CameraComponent : public Component {
public:
    // Données de la caméra (View/Projection matrices)
};

// 2. Le Composant Mesh
class MeshComponent : public Component {
public:
    MeshComponent(std::unique_ptr<Mesh> mesh) : m_Mesh(std::move(mesh)) {}
    Mesh* GetMesh() const { return m_Mesh.get(); }
private:
    std::unique_ptr<Mesh> m_Mesh;
};