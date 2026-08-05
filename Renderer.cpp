#include "Renderer.h"
#include <d3dcompiler.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

// ---------------------------------------------------------------
// Carte "Initialisation Direct3D" : device, swap chain, render target, depth buffer
// ---------------------------------------------------------------
bool Renderer::InitD3D(HWND hwnd, int width, int height)
{
    m_width = width;
    m_height = height;

    DXGI_SWAP_CHAIN_DESC scd = {};
    scd.BufferCount = 1;
    scd.BufferDesc.Width = width;
    scd.BufferDesc.Height = height;
    scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    scd.BufferDesc.RefreshRate.Numerator = 60;
    scd.BufferDesc.RefreshRate.Denominator = 1;
    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    scd.OutputWindow = hwnd;
    scd.SampleDesc.Count = 1;
    scd.Windowed = TRUE;

    UINT flags = 0;
#ifdef _DEBUG
    flags |= D3D11_CREATE_DEVICE_DEBUG; // messages d'erreur detailles dans la fenetre Sortie
#endif

    // <-- C'est ICI qu'on choisit Direct3D 11 (le feature level 11_0).
    D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;

    HRESULT hr = D3D11CreateDeviceAndSwapChain(
        nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, flags,
        &featureLevel, 1, D3D11_SDK_VERSION,
        &scd, &m_swapChain, &m_device, nullptr, &m_context);
    if (FAILED(hr)) return false;

    ComPtr<ID3D11Texture2D> backBuffer;
    m_swapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer));
    m_device->CreateRenderTargetView(backBuffer.Get(), nullptr, &m_rtv);

    // Depth buffer : necessaire quand plusieurs objets se chevauchent
    D3D11_TEXTURE2D_DESC depthDesc = {};
    depthDesc.Width = width;
    depthDesc.Height = height;
    depthDesc.MipLevels = 1;
    depthDesc.ArraySize = 1;
    depthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthDesc.SampleDesc.Count = 1;
    depthDesc.Usage = D3D11_USAGE_DEFAULT;
    depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

    ComPtr<ID3D11Texture2D> depthTexture;
    m_device->CreateTexture2D(&depthDesc, nullptr, &depthTexture);
    m_device->CreateDepthStencilView(depthTexture.Get(), nullptr, &m_dsv);

    m_context->OMSetRenderTargets(1, m_rtv.GetAddressOf(), m_dsv.Get());
    return true;
}

// ---------------------------------------------------------------
// Carte "Pipeline Graphique & HLSL" : shaders + input layout
// ---------------------------------------------------------------
bool Renderer::InitPipeline()
{
    ComPtr<ID3DBlob> vsBlob, psBlob, errorBlob;

    HRESULT hr = D3DCompileFromFile(L"VertexShader.hlsl", nullptr, nullptr,
        "main", "vs_5_0", 0, 0, &vsBlob, &errorBlob);
    if (FAILED(hr))
    {
        if (errorBlob) OutputDebugStringA((char*)errorBlob->GetBufferPointer());
        return false;
    }
    m_device->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), nullptr, &m_vertexShader);

    hr = D3DCompileFromFile(L"PixelShader.hlsl", nullptr, nullptr,
        "main", "ps_5_0", 0, 0, &psBlob, &errorBlob);
    if (FAILED(hr))
    {
        if (errorBlob) OutputDebugStringA((char*)errorBlob->GetBufferPointer());
        return false;
    }
    m_device->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), nullptr, &m_pixelShader);

    // Doit correspondre exactement a la struct VS_INPUT du VertexShader.hlsl
    D3D11_INPUT_ELEMENT_DESC layout[] = {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR",    0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };
    HRESULT ilhr = m_device->CreateInputLayout(layout, 2, vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), &m_inputLayout);
    return SUCCEEDED(ilhr);
}

// ---------------------------------------------------------------
// Carte "Rendu de Meshes Proceduraux" : un cube genere en code (8 sommets, 36 indices)
// La geometrie elle-meme est desormais encapsulee dans MeshComponent (voir
// MeshComponent.h/.cpp), afin de pouvoir plus tard etre attachee a une Entity
// dans le systeme Transform / Camera commun avec le reste de l'equipe.
// ---------------------------------------------------------------
bool Renderer::InitMesh()
{
    if (!m_mesh.CreateUnitCube(m_device.Get())) return false;

    // Constant buffer pour la matrice WVP (temporaire : remplace plus tard par le
    // vrai systeme Transform / Camera du reste de l'equipe)
    D3D11_BUFFER_DESC cbd = {};
    cbd.Usage = D3D11_USAGE_DYNAMIC;
    cbd.ByteWidth = sizeof(TransformBuffer);
    cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    hr = m_device->CreateBuffer(&cbd, nullptr, &m_constantBuffer);
    return SUCCEEDED(hr);
}

// ---------------------------------------------------------------
// Carte "Configuration du Rasterizer" : culling, fill mode, viewport
// ---------------------------------------------------------------
bool Renderer::InitRasterizer()
{
    D3D11_RASTERIZER_DESC rd = {};
    rd.FillMode = D3D11_FILL_SOLID;
    rd.CullMode = D3D11_CULL_BACK;       // n'affiche pas les faces qui nous tournent le dos
    rd.FrontCounterClockwise = FALSE;    // sens horaire = face avant (convention par defaut D3D)
    rd.DepthClipEnable = TRUE;

    HRESULT hr = m_device->CreateRasterizerState(&rd, &m_rasterState);
    if (FAILED(hr)) return false;
    m_context->RSSetState(m_rasterState.Get());

    D3D11_VIEWPORT vp = {};
    vp.Width = (float)m_width;
    vp.Height = (float)m_height;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    m_context->RSSetViewports(1, &vp);

    return true;
}

// ---------------------------------------------------------------
bool Renderer::Init(HWND hwnd, int width, int height)
{
    if (!InitD3D(hwnd, width, height)) return false;
    if (!InitPipeline())               return false;
    if (!InitMesh())                   return false;
    if (!InitRasterizer())             return false;
    return true;
}

void Renderer::Render()
{
    float clearColor[4] = { 0.05f, 0.05f, 0.08f, 1.0f };
    m_context->ClearRenderTargetView(m_rtv.Get(), clearColor);
    m_context->ClearDepthStencilView(m_dsv.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);

    // --- Bloc temporaire : world/view/projection a la main.
    // Sera remplace par le Transform et la Camera geres par le reste de l'equipe.
    m_rotation += 0.01f;
    XMMATRIX world = XMMatrixRotationY(m_rotation);
    XMMATRIX view  = XMMatrixLookAtLH(XMVectorSet(0, 1.5f, -4, 0), XMVectorSet(0, 0, 0, 0), XMVectorSet(0, 1, 0, 0));
    XMMATRIX proj  = XMMatrixPerspectiveFovLH(XM_PIDIV4, (float)m_width / (float)m_height, 0.1f, 100.0f);

    D3D11_MAPPED_SUBRESOURCE mapped;
    m_context->Map(m_constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
    TransformBuffer* buf = (TransformBuffer*)mapped.pData;
    buf->WVP = XMMatrixTranspose(world * view * proj);
    m_context->Unmap(m_constantBuffer.Get(), 0);

    m_mesh.Bind(m_context.Get());
    m_context->IASetInputLayout(m_inputLayout.Get());
    m_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    m_context->VSSetShader(m_vertexShader.Get(), nullptr, 0);
    m_context->VSSetConstantBuffers(0, 1, m_constantBuffer.GetAddressOf());
    m_context->PSSetShader(m_pixelShader.Get(), nullptr, 0);

    m_context->DrawIndexed(m_mesh.GetIndexCount(), 0, 0);

    m_swapChain->Present(1, 0);
}

void Renderer::Shutdown()
{
    if (m_context) m_context->ClearState();
}
