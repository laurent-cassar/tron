#include <windows.h>
#include <d3d11.h>
#include <directxmath/DirectXMath.h>
#include <cstdio>

#include "Camera.h"

#pragma comment(lib, "d3d11.lib")

// ---------------------------------------------------------------------
// Variables globales D3D
// ---------------------------------------------------------------------
static ID3D11Device *g_device = nullptr;
static ID3D11DeviceContext *g_context = nullptr;
static IDXGISwapChain *g_swapChain = nullptr;
static ID3D11RenderTargetView *g_renderTargetView = nullptr;
static ID3D11DepthStencilView *g_depthStencilView = nullptr;

static Camera g_camera;

static const UINT WINDOW_WIDTH = 1280;
static const UINT WINDOW_HEIGHT = 720;

// ---------------------------------------------------------------------
// WndProc
// ---------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

// ---------------------------------------------------------------------
// Initialisation Direct3D
// ---------------------------------------------------------------------
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
    scd.SampleDesc.Quality = 0;
    scd.Windowed = TRUE;

    D3D_FEATURE_LEVEL featureLevel;
    HRESULT hr = D3D11CreateDeviceAndSwapChain(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        0,
        nullptr,
        0,
        D3D11_SDK_VERSION,
        &scd,
        &g_swapChain,
        &g_device,
        &featureLevel,
        &g_context);

    if (FAILED(hr))
    {
        printf("Echec de D3D11CreateDeviceAndSwapChain\n");
        return false;
    }

    ID3D11Texture2D *backBuffer = nullptr;
    g_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void **)&backBuffer);

    g_device->CreateRenderTargetView(backBuffer, nullptr, &g_renderTargetView);
    backBuffer->Release();

    D3D11_TEXTURE2D_DESC depthDesc = {};
    depthDesc.Width = WINDOW_WIDTH;
    depthDesc.Height = WINDOW_HEIGHT;
    depthDesc.MipLevels = 1;
    depthDesc.ArraySize = 1;
    depthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthDesc.SampleDesc.Count = 1;
    depthDesc.Usage = D3D11_USAGE_DEFAULT;
    depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

    ID3D11Texture2D *depthStencilBuffer = nullptr;
    g_device->CreateTexture2D(&depthDesc, nullptr, &depthStencilBuffer);

    g_device->CreateDepthStencilView(depthStencilBuffer, nullptr, &g_depthStencilView);
    depthStencilBuffer->Release();

    g_context->OMSetRenderTargets(1, &g_renderTargetView, g_depthStencilView);

    D3D11_VIEWPORT viewport = {};
    viewport.TopLeftX = 0.0f;
    viewport.TopLeftY = 0.0f;
    viewport.Width = static_cast<float>(WINDOW_WIDTH);
    viewport.Height = static_cast<float>(WINDOW_HEIGHT);
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    g_context->RSSetViewports(1, &viewport);

    // Shaders & Input Layout à ajouter une fois la partie caméra validée

    float aspectRatio = static_cast<float>(WINDOW_WIDTH) / static_cast<float>(WINDOW_HEIGHT);
    g_camera.SetProjection(DirectX::XM_PIDIV4, aspectRatio, 0.1f, 100.0f);

    return true;
}

void CleanupDirect3D()
{
    if (g_depthStencilView)
        g_depthStencilView->Release();
    if (g_renderTargetView)
        g_renderTargetView->Release();
    if (g_swapChain)
        g_swapChain->Release();
    if (g_context)
        g_context->Release();
    if (g_device)
        g_device->Release();
}

// ---------------------------------------------------------------------
// Gestion basique des entrées clavier pour déplacer la caméra
// (à remplacer plus tard par un vrai InputManager)
// ---------------------------------------------------------------------
void HandleCameraInput(float deltaTime)
{
    const float speed = 3.0f * deltaTime;

    if (GetAsyncKeyState('W') & 0x8000)
        g_camera.MoveForward(speed);
    if (GetAsyncKeyState('S') & 0x8000)
        g_camera.MoveForward(-speed);
    if (GetAsyncKeyState('D') & 0x8000)
        g_camera.MoveRight(speed);
    if (GetAsyncKeyState('A') & 0x8000)
        g_camera.MoveRight(-speed);
    if (GetAsyncKeyState('E') & 0x8000)
        g_camera.MoveUp(speed);
    if (GetAsyncKeyState('Q') & 0x8000)
        g_camera.MoveUp(-speed);

    g_camera.Update();
}

// ---------------------------------------------------------------------
// Boucle de rendu
// ---------------------------------------------------------------------
void RenderFrame()
{
    const float clearColor[4] = {0.05f, 0.05f, 0.1f, 1.0f};
    g_context->ClearRenderTargetView(g_renderTargetView, clearColor);
    g_context->ClearDepthStencilView(g_depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

    // DRAW : à ajouter une fois qu'on a un mesh + shaders

    g_swapChain->Present(1, 0);
}

// ---------------------------------------------------------------------
// WinMain
// ---------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
{
    WNDCLASSEX wc = {};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "D3DCameraWindowClass";
    RegisterClassEx(&wc);

    HWND hwnd = CreateWindowEx(
        0,
        "D3DCameraWindowClass",
        "Direct3D - Camera",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        WINDOW_WIDTH, WINDOW_HEIGHT,
        nullptr, nullptr, hInstance, nullptr);

    if (!hwnd)
        return -1;

    ShowWindow(hwnd, SW_SHOW);

    if (!InitDirect3D(hwnd))
    {
        return -1;
    }

    MSG msg = {};
    while (msg.message != WM_QUIT)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            HandleCameraInput(0.016f);
            RenderFrame();
        }
    }

    CleanupDirect3D();
    return 0;
}
