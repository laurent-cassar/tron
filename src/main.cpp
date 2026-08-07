#include <windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "kernel32.lib")

static ID3D11Device*           g_device = nullptr;
static ID3D11DeviceContext*    g_context = nullptr;
static IDXGISwapChain*         g_swapChain = nullptr;
static ID3D11RenderTargetView* g_renderTargetView = nullptr;
static ID3D11DepthStencilView* g_depthStencilView = nullptr;
static ID3D11VertexShader*     g_vertexShader = nullptr;
static ID3D11PixelShader*      g_pixelShader = nullptr;
static ID3D11InputLayout*      g_inputLayout = nullptr;
static ID3D11Buffer*           g_vertexBuffer = nullptr;
static ID3D11Buffer*           g_constantBuffer = nullptr;
static ID3D11RasterizerState*  g_rasterizerState = nullptr;
static const UINT WINDOW_WIDTH = 1280;
static const UINT WINDOW_HEIGHT = 720;

#define SAFE_RELEASE(p) if((p)){ (p)->Release(); (p)=nullptr; }

bool ResizeSwapChain(UINT width, UINT height)
{
    if (!g_swapChain) return false;

    SAFE_RELEASE(g_renderTargetView);
    SAFE_RELEASE(g_depthStencilView);

    HRESULT hr = g_swapChain->ResizeBuffers(0, width, height, DXGI_FORMAT_UNKNOWN, 0);
    if (FAILED(hr)) return false;

    ID3D11Texture2D* backBuffer = nullptr;
    hr = g_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
    if (FAILED(hr)) return false;

    hr = g_device->CreateRenderTargetView(backBuffer, nullptr, &g_renderTargetView);
    backBuffer->Release();
    if (FAILED(hr)) return false;

    D3D11_TEXTURE2D_DESC depthDesc = {};
    depthDesc.Width = width;
    depthDesc.Height = height;
    depthDesc.MipLevels = 1;
    depthDesc.ArraySize = 1;
    depthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthDesc.SampleDesc.Count = 1;
    depthDesc.Usage = D3D11_USAGE_DEFAULT;
    depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

    ID3D11Texture2D* depthTex = nullptr;
    hr = g_device->CreateTexture2D(&depthDesc, nullptr, &depthTex);
    if (FAILED(hr)) return false;

    hr = g_device->CreateDepthStencilView(depthTex, nullptr, &g_depthStencilView);
    depthTex->Release();
    if (FAILED(hr)) return false;

    g_context->OMSetRenderTargets(1, &g_renderTargetView, g_depthStencilView);

    D3D11_VIEWPORT vp = {};
    vp.Width = static_cast<FLOAT>(width);
    vp.Height = static_cast<FLOAT>(height);
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    g_context->RSSetViewports(1, &vp);

    return true;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    case WM_KEYDOWN:
        if (wParam == VK_ESCAPE) { PostQuitMessage(0); return 0; }
        break;
    case WM_SIZE:
        if (g_swapChain && wParam != SIZE_MINIMIZED)
        {
            UINT width = LOWORD(lParam);
            UINT height = HIWORD(lParam);
            ResizeSwapChain(width, height);
        }
        break;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

bool InitDirect3D(HWND hwnd)
{
    DXGI_SWAP_CHAIN_DESC scd = {};
    scd.BufferCount = 1;
    scd.BufferDesc.Width = WINDOW_WIDTH;
    scd.BufferDesc.Height = WINDOW_HEIGHT;
    scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    scd.BufferDesc.RefreshRate.Numerator = 60;
    scd.BufferDesc.RefreshRate.Denominator = 1;
    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    scd.OutputWindow = hwnd;
    scd.SampleDesc.Count = 1;
    scd.Windowed = TRUE;
    scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;
    HRESULT hr = D3D11CreateDeviceAndSwapChain(
        nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0,
        &featureLevel, 1, D3D11_SDK_VERSION,
        &scd, &g_swapChain, &g_device, nullptr, &g_context);
    if (FAILED(hr)) return false;

    if (!ResizeSwapChain(WINDOW_WIDTH, WINDOW_HEIGHT)) return false;

    ID3DBlob* vsBlob = nullptr;
    ID3DBlob* psBlob = nullptr;

    // Compile vertex shader
    hr = D3DCompileFromFile(L"VertexShader.hlsl", nullptr, nullptr, "main", "vs_5_0", 0, 0, &vsBlob, nullptr);
    if (FAILED(hr)) return false;
    hr = g_device->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), nullptr, &g_vertexShader);
    if (FAILED(hr)) { SAFE_RELEASE(vsBlob); return false; }

    // Compile pixel shader
    hr = D3DCompileFromFile(L"PixelShader.hlsl", nullptr, nullptr, "main", "ps_5_0", 0, 0, &psBlob, nullptr);
    if (FAILED(hr)) { SAFE_RELEASE(vsBlob); return false; }
    hr = g_device->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), nullptr, &g_pixelShader);
    if (FAILED(hr)) { SAFE_RELEASE(vsBlob); SAFE_RELEASE(psBlob); return false; }

    // Create input layout
    D3D11_INPUT_ELEMENT_DESC layout[] = {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"COLOR",    0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
    };
    hr = g_device->CreateInputLayout(layout, 2, vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), &g_inputLayout);
    SAFE_RELEASE(vsBlob);
    SAFE_RELEASE(psBlob);
    if (FAILED(hr)) return false;

    // Create constant buffer (identity matrix)
    D3D11_BUFFER_DESC cbDesc = {};
    cbDesc.Usage = D3D11_USAGE_DEFAULT;
    cbDesc.ByteWidth = sizeof(float) * 16;
    cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    cbDesc.CPUAccessFlags = 0;

    float identityMatrix[16] = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };

    D3D11_SUBRESOURCE_DATA cbInitData = {};
    cbInitData.pSysMem = identityMatrix;
    hr = g_device->CreateBuffer(&cbDesc, &cbInitData, &g_constantBuffer);
    if (FAILED(hr)) return false;

    // Create triangle vertex buffer (NDC coordinates: -1.0 to 1.0)
    struct Vertex { float pos[3]; float col[3]; };
    Vertex vertices[] = {
        { {  0.0f,  0.5f, 0.0f }, { 1.0f, 0.0f, 0.0f } },  // Red top
        { {  0.5f, -0.5f, 0.0f }, { 0.0f, 1.0f, 0.0f } },  // Green bottom-right
        { { -0.5f, -0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f } }   // Blue bottom-left
    };

    D3D11_BUFFER_DESC bd = {};
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(vertices);
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = 0;

    D3D11_SUBRESOURCE_DATA init = {};
    init.pSysMem = vertices;
    hr = g_device->CreateBuffer(&bd, &init, &g_vertexBuffer);
    if (FAILED(hr)) return false;

    // Create rasterizer state (fill mode)
    D3D11_RASTERIZER_DESC rastDesc = {};
    rastDesc.FillMode = D3D11_FILL_SOLID;
    rastDesc.CullMode = D3D11_CULL_BACK;
    rastDesc.FrontCounterClockwise = FALSE;
    rastDesc.DepthBias = 0;
    rastDesc.DepthBiasClamp = 0.0f;
    rastDesc.SlopeScaledDepthBias = 0.0f;
    rastDesc.DepthClipEnable = TRUE;
    rastDesc.ScissorEnable = FALSE;
    rastDesc.MultisampleEnable = FALSE;
    rastDesc.AntialiasedLineEnable = FALSE;

    hr = g_device->CreateRasterizerState(&rastDesc, &g_rasterizerState);
    if (FAILED(hr)) return false;

    return true;
}

void CleanupDirect3D()
{
    SAFE_RELEASE(g_rasterizerState);
    SAFE_RELEASE(g_constantBuffer);
    SAFE_RELEASE(g_vertexBuffer);
    SAFE_RELEASE(g_inputLayout);
    SAFE_RELEASE(g_vertexShader);
    SAFE_RELEASE(g_pixelShader);
    SAFE_RELEASE(g_depthStencilView);
    SAFE_RELEASE(g_renderTargetView);
    SAFE_RELEASE(g_swapChain);
    SAFE_RELEASE(g_context);
    SAFE_RELEASE(g_device);
}

void RenderFrame()
{
    if (!g_context || !g_renderTargetView || !g_depthStencilView) return;

    const float clearColor[4] = { 0.05f, 0.05f, 0.1f, 1.0f };
    g_context->ClearRenderTargetView(g_renderTargetView, clearColor);
    g_context->ClearDepthStencilView(g_depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

    // Bind all pipeline state
    g_context->IASetInputLayout(g_inputLayout);
    
    UINT stride = sizeof(float) * 6;  // 3 floats pos + 3 floats color
    UINT offset = 0;
    g_context->IASetVertexBuffers(0, 1, &g_vertexBuffer, &stride, &offset);
    g_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    
    g_context->VSSetShader(g_vertexShader, nullptr, 0);
    g_context->VSSetConstantBuffers(0, 1, &g_constantBuffer);
    
    g_context->RSSetState(g_rasterizerState);
    
    g_context->PSSetShader(g_pixelShader, nullptr, 0);

    // Draw the triangle (3 vertices)
    g_context->Draw(3, 0);

    g_swapChain->Present(1, 0);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
{
    WNDCLASSEXW wc = {};
    wc.cbSize = sizeof(WNDCLASSEXW);
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"TRONBootstrapWindow";
    RegisterClassExW(&wc);

    RECT rc = { 0, 0, static_cast<LONG>(WINDOW_WIDTH), static_cast<LONG>(WINDOW_HEIGHT) };
    AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

    HWND hwnd = CreateWindowExW(0, L"TRONBootstrapWindow", L"TRON - Minimal D3D11", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, hInstance, nullptr);
    if (!hwnd) return -1;
    ShowWindow(hwnd, SW_SHOW);

    if (!InitDirect3D(hwnd)) { CleanupDirect3D(); return -1; }

    MSG msg = {};
    while (msg.message != WM_QUIT)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) { TranslateMessage(&msg); DispatchMessage(&msg); }
        else { RenderFrame(); }
    }

    CleanupDirect3D();
    return 0;
}

