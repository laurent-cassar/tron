#pragma once
// Regroupe tout le pipeline Direct3D : device, shaders, mesh, rasterizer.
// Couvre les cartes : Initialisation Direct3D / Pipeline Graphique & HLSL /
// Rendu de Meshes Proceduraux / Configuration du Rasterizer.

#include <Windows.h>
#include <d3d11.h>
#include <wrl/client.h>
#include <DirectXMath.h>
#include "MeshComponent.h"

using Microsoft::WRL::ComPtr;
using namespace DirectX;

class Renderer
{
public:
    bool Init(HWND hwnd, int width, int height);
    void Render();
    void Shutdown();

private:
    bool InitD3D(HWND hwnd, int width, int height);
    bool InitPipeline();
    bool InitMesh();
    bool InitRasterizer();

    struct TransformBuffer
    {
        XMMATRIX WVP;
    };

    ComPtr<ID3D11Device>            m_device;
    ComPtr<ID3D11DeviceContext>     m_context;
    ComPtr<IDXGISwapChain>          m_swapChain;
    ComPtr<ID3D11RenderTargetView>  m_rtv;
    ComPtr<ID3D11DepthStencilView>  m_dsv;

    ComPtr<ID3D11VertexShader>      m_vertexShader;
    ComPtr<ID3D11PixelShader>       m_pixelShader;
    ComPtr<ID3D11InputLayout>       m_inputLayout;

    ComPtr<ID3D11Buffer>            m_constantBuffer;

    ComPtr<ID3D11RasterizerState>   m_rasterState;

    MeshComponent m_mesh; // remplace les anciens m_vertexBuffer / m_indexBuffer / m_indexCount

    int   m_width = 0;
    int   m_height = 0;
    float m_rotation = 0.0f;
};
