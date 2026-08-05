# Compilation

Prérequis : MinGW-w64 (g++) installé sous Windows, avec les headers/libs Direct3D
(fournis avec le SDK Windows / MinGW-w64 récent).

Depuis l'invite de commande, dans le dossier du projet :

```
g++ -Iinclude src/main.cpp src/Camera.cpp src/Transform.cpp src/Quaternion.cpp -o CameraApp.exe -ld3d11 -ldxgi -ld3dcompiler -ldxguid -luser32 -lgdi32 -std=c++17
```

Puis lancer l'exécutable :

```
CameraApp.exe
```

## Pour compiler les fichiers de test
```
g++ -Iinclude tests/Tests.cpp src/Camera.cpp src/Transform.cpp src/Quaternion.cpp -o Tests.exe -std=c++17
```

Puis lancer l'executable

```
Tests.exe
```
