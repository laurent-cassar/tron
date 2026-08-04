# Compilation (invite de commande, g++, sans CMake)

Prérequis : MinGW-w64 (g++) installé sous Windows, avec les headers/libs Direct3D
(fournis avec le SDK Windows / MinGW-w64 récent).

Depuis l'invite de commande, dans le dossier du projet :

```
g++ -Iinclude src/main.cpp src/Camera.cpp src/Transform.cpp -o CameraApp.exe -ld3d11 -ldxgi -ld3dcompiler -ldxguid -luser32 -lgdi32 -std=c++17
```

Puis lancer l'exécutable :

```
CameraApp.exe
```

## Contenu

- `Camera.h` / `Camera.cpp` : gestion de la caméra (position, rotation, matrices
  View & Projection).
- `main.cpp` : fenêtre Win32 + initialisation Direct3D (Device, Context,
  SwapChain, Back Buffer, RTV, Depth-Stencil texture + DSV, liaison à
  l'Output-Merger, Viewport) + boucle CLEAR / DISPLAY, avec les déplacements
  caméra au clavier (Z/Q/S/D ou W/A/S/D selon disposition, + E/Q pour monter/descendre).

## Prochaines étapes

- Ajouter les shaders HLSL (Vertex Shader / Pixel Shader) et l'Input Layout.
- Créer un mesh (Vertex Buffer + Index Buffer) et un buffer de matrices
  (constant buffer) pour envoyer World/View/Projection au Vertex Shader.
- Ajouter le DRAW dans `RenderFrame()`.

## Pour compiler les fichiers de test
```
g++ -Iinclude test/Tests.cpp src/Camera.cpp src/Transform.cpp -o Tests.exe -std=c++17
```

Puis lancer l'executable

```
test.exe
```