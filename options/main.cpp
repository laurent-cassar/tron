// Point d'entree : cree la fenetre Win32 et lance la boucle de rendu.
#include <Windows.h>
#include "Renderer.h"

Renderer g_renderer;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (msg == WM_DESTROY)
    {
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
{
    const int width = 1280, height = 720;

    WNDCLASSEX wc = {};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"MoteurWindowClass";
    RegisterClassEx(&wc);

    HWND hwnd = CreateWindowEx(
        0, L"MoteurWindowClass", L"Moteur 3D - Rendu Direct3D",
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, width, height,
        nullptr, nullptr, hInstance, nullptr);

    ShowWindow(hwnd, SW_SHOW);

    if (!g_renderer.Init(hwnd, width, height))
    {
        MessageBox(hwnd, L"Echec de l'initialisation Direct3D", L"Erreur", MB_OK);
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
            g_renderer.Render();
        }
    }

    g_renderer.Shutdown();
    return 0;
}
