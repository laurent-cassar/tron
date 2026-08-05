#pragma once
// MeshComponent : encapsule la geometrie d'un objet (vertex buffer + index buffer).
// Objectif : separer les donnees de mesh du Renderer, pour pouvoir a terme
// attacher un MeshComponent a une Entity dans un systeme Transform / Camera
// commun avec le reste de l'equipe.

#include <d3d11.h>
#include <wrl/client.h>
#include <DirectXMath.h>
#include <vector>

using Microsoft::WRL::ComPtr;
using namespace DirectX;

struct Vertex
{
    XMFLOAT3 pos;
    XMFLOAT3 color;
};

class MeshComponent
{
public:
    // Cree les buffers GPU a partir de listes de sommets/indices fournies.
    bool Create(ID3D11Device* device, const std::vector<Vertex>& vertices, const std::vector<UINT>& indices);

    // Raccourci : genere directement un cube unitaire (remplace l'ancien Renderer::InitMesh).
    bool CreateUnitCube(ID3D11Device* device);

    // Bind le vertex buffer + index buffer sur le contexte, pret pour DrawIndexed.
    void Bind(ID3D11DeviceContext* context) const;

    UINT GetIndexCount() const { return m_indexCount; }

private:
    ComPtr<ID3D11Buffer> m_vertexBuffer;
    ComPtr<ID3D11Buffer> m_indexBuffer;
    UINT m_indexCount = 0;
    UINT m_stride = sizeof(Vertex);
};
