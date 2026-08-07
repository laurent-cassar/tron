#pragma once
#include <vector>
#include <initializer_list>

// Structure Vertex fournie dans l'exemple du prof
struct Vertex {
    float position[3];
    float color[3];
};

// Classe Mesh gérant les données géométriques
class Mesh {
public:
    Mesh(std::initializer_list<Vertex> vertices, std::initializer_list<unsigned int> indices) 
        : m_Vertices(vertices), m_Indices(indices) {}
        
    // Ton collègue Direct3D ajoutera des getters ici plus tard (ex: GetVertexBuffer())
    
private:
    std::vector<Vertex> m_Vertices;
    std::vector<unsigned int> m_Indices;
};