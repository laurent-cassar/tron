#include "MeshComponent.h"

bool MeshComponent::Create(ID3D11Device* device, const std::vector<Vertex>& vertices, const std::vector<UINT>& indices)
{
    m_indexCount = static_cast<UINT>(indices.size());

    D3D11_BUFFER_DESC vbd = {};
    vbd.Usage = D3D11_USAGE_DEFAULT;
    vbd.ByteWidth = static_cast<UINT>(sizeof(Vertex) * vertices.size());
    vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    D3D11_SUBRESOURCE_DATA vinit = { vertices.data() };
    HRESULT hr = device->CreateBuffer(&vbd, &vinit, &m_vertexBuffer);
    if (FAILED(hr)) return false;

    D3D11_BUFFER_DESC ibd = {};
    ibd.Usage = D3D11_USAGE_DEFAULT;
    ibd.ByteWidth = static_cast<UINT>(sizeof(UINT) * indices.size());
    ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    D3D11_SUBRESOURCE_DATA iinit = { indices.data() };
    hr = device->CreateBuffer(&ibd, &iinit, &m_indexBuffer);
    if (FAILED(hr)) return false;

    return true;
}

bool MeshComponent::CreateUnitCube(ID3D11Device* device)
{
    // Meme geometrie que l'ancien Renderer::InitMesh (8 sommets, 36 indices).
    std::vector<Vertex> vertices = {
        { XMFLOAT3(-0.5f,-0.5f,-0.5f), XMFLOAT3(0,0,0) }, // 0
        { XMFLOAT3( 0.5f,-0.5f,-0.5f), XMFLOAT3(1,0,0) }, // 1
        { XMFLOAT3( 0.5f, 0.5f,-0.5f), XMFLOAT3(1,1,0) }, // 2
        { XMFLOAT3(-0.5f, 0.5f,-0.5f), XMFLOAT3(0,1,0) }, // 3
        { XMFLOAT3(-0.5f,-0.5f, 0.5f), XMFLOAT3(0,0,1) }, // 4
        { XMFLOAT3( 0.5f,-0.5f, 0.5f), XMFLOAT3(1,0,1) }, // 5
        { XMFLOAT3( 0.5f, 0.5f, 0.5f), XMFLOAT3(1,1,1) }, // 6
        { XMFLOAT3(-0.5f, 0.5f, 0.5f), XMFLOAT3(0,1,1) }, // 7
    };

    // Ordre horaire vu de l'exterieur sur chaque face (requis par CullMode Back)
    std::vector<UINT> indices = {
        0,3,2, 0,2,1,   // face avant  (-Z)
        4,5,6, 4,6,7,   // face arriere (+Z)
        0,4,7, 0,7,3,   // face gauche (-X)
        1,2,6, 1,6,5,   // face droite (+X)
        3,7,6, 3,6,2,   // face du dessus (+Y)
        0,1,5, 0,5,4    // face du dessous (-Y)
    };

    return Create(device, vertices, indices);
}

void MeshComponent::Bind(ID3D11DeviceContext* context) const
{
    UINT offset = 0;
    context->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &m_stride, &offset);
    context->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
}
